#include "tdma.h"
#include <stdlib.h>

//Module specifiers
static struct tx_spec_s tx_spec;
static struct tdma_spec_s tdma_spec;
static uint8_t slot_id_list[MAX_NUM_DEVICES];
static struct tx_spec_s tx_spec;
static uint64_t slot_start_timestamp;
static struct message_spec_s msg;


//Debug States
static uint16_t num_successful_recieves;
static uint16_t n1;
static uint16_t n2;

static uint16_t num_subordinate_receives;
static uint16_t num_successful_transmits;
static uint16_t num_rts_received;
static uint16_t num_cts_received;
static uint16_t num_ds_received;
static uint16_t num_dack_received;
static uint16_t num_rx_ovrr;
static uint16_t num_rts_sent;
static uint16_t num_cts_sent;
static uint32_t last_data_sent = 0;
static uint32_t data_period = 0;
static uint32_t last_cal_init = 0;
static uint32_t cal_period = 0;
static float range;
static bool new_range;

static struct worker_thread_timer_task_s main_task;
static struct worker_thread_timer_task_s status_task;

//TRX State handles
static uint32_t last_receive;
static uint32_t last_transmit;
static bool transmit_complete;
static uint8_t transmit_state;
static uint64_t last_start_send;

//Collision avoidance media acquire handles
static uint8_t rts_received_sleep;
static bool started_acquiring_medium;
static uint8_t rts_backoff_cnt;
static uint8_t rts_backoff_mask;
static uint32_t defer_acquire_medium_tstart;
static bool schedule_dack;
static bool receive_acquiring_medium;


static struct worker_thread_s *lpworker_thread;
static struct worker_thread_s *listener_thread;

static void handle_receive_event(void);
static void transmit_loop(struct worker_thread_timer_task_s* task);
static void super_update_start_slot(void);
static void start_acquiring_medium(void);
static void initiate_cal_run(void);


#define RX_TIMEOUT 20   //20ms Dw1000 timeout
#define TX_TIMEOUT 2    //2ms Dw1000 Tx timeout

//
//    Supervisor Methods


static void dw1000_sys_status_handler(size_t msg_size, const void* buf, void* ctx)
{
    (void)msg_size;
    (void)buf;
    (void)ctx;
    if (dw1000_get_status(&uwb_instance) & (DW1000_IRQ_MASK(DW1000_SYS_STATUS_LDEDONE) | DW1000_IRQ_MASK(DW1000_SYS_STATUS_RXOVRR))) {
        handle_receive_event();
        dw1000_clear_status(&uwb_instance, DW1000_IRQ_MASK(DW1000_SYS_STATUS_LDEDONE) | DW1000_IRQ_MASK(DW1000_SYS_STATUS_RXOVRR));
        last_receive = millis();
    }
    if (dw1000_get_status(&uwb_instance) & DW1000_IRQ_MASK(DW1000_SYS_STATUS_TXFRS)) {
        transmit_complete = true;
        dw1000_clear_status(&uwb_instance, DW1000_IRQ_MASK(DW1000_SYS_STATUS_TXFRS));
    }
}

static void status_checker(struct worker_thread_timer_task_s* task)
{
    (void)task;
    //If we haven't received anything for more than RX_TIMEOUT we go in manually to check if we are in an error state
    /*if ((millis() - last_receive) > RX_TIMEOUT) {
        if (dw1000_get_status(&uwb_instance) & DW1000_IRQ_MASK(DW1000_SYS_STATUS_RXOVRR)) {
            dw1000_disable_transceiver(&uwb_instance);
            dw1000_swap_rx_buffers(&uwb_instance);
            dw1000_rx_softreset(&uwb_instance);
            // Receiver must be reset to exit errored state
            dw1000_rx_enable(&uwb_instance);
        }
        last_receive = millis();
    }*/
    if ((millis() - last_transmit) > TX_TIMEOUT) {
        transmit_complete = true;
    }
    if(started_acquiring_medium) {
        defer_acquire_medium_tstart = millis();
    }

    if (((millis() - defer_acquire_medium_tstart) >= DEFER_TIME)) {
        receive_acquiring_medium = true;
    }
    if (new_range) {
        if(range < 100.0f) {
            //uavcan_send_debug_keyvalue("Range", range);
        }
        new_range = false;
    }

    if (((millis() - defer_acquire_medium_tstart) >= 2*DEFER_TIME)) {
        start_acquiring_medium();
    }
}
/*
    TDMA Init
*/
void tdma_supervisor_init(struct tx_spec_s _tx_spec, uint8_t target_body_id, struct worker_thread_s* _lpworker_thread, struct worker_thread_s* _listener_thread)
{
    //TDMA Init
    tdma_spec.slot_size = SLOT_SIZE;
    tdma_spec.res_data_slot = 0;
    tdma_spec.req_node_id = 0;
    tdma_spec.cnt = 0;
    tdma_spec.num_slots = 1;
    tdma_spec.target_body_id = target_body_id;

    //Medium Access init
    started_acquiring_medium = true;
    rts_backoff_mask = INITIAL_BACKOFF_MASK;
    rts_backoff_cnt = 0;
    defer_acquire_medium_tstart = 0;
    schedule_dack = false;
    rts_received_sleep = 0;
    receive_acquiring_medium = true;
    last_start_send = 0;
    n1 = 0;
    n2 = 0;
    num_successful_recieves = 0;
    num_successful_transmits = 0;
    num_rts_received = 0;
    num_cts_received = 0;
    num_ds_received = 0;
    num_dack_received = 0;
    last_receive = 0;
    last_transmit = 0;
    num_cts_sent = 0;
    num_rts_sent = 0;
    num_subordinate_receives = 0;

    memcpy(&tx_spec, &_tx_spec, sizeof(tx_spec));

    //seed random number generator
    srand(((uint32_t)tx_spec.node_id) | ((uint32_t)tx_spec.body_id<<16));

    memset(slot_id_list, 0, sizeof(slot_id_list));
    twr_init();

    //Setup DW1000 to our needs
    dw1000_init(&uwb_instance, 3, BOARD_PAL_LINE_SPI3_UWB_CS, BOARD_PAL_LINE_UWB_NRST, tx_spec.ant_delay);

    dw1000_setup_irq(&uwb_instance, FW_EXT_IRQ_PORT(GPIOA), 1, DW1000_IRQ_MASK(DW1000_SYS_STATUS_LDEDONE) | 
                                                               DW1000_IRQ_MASK(DW1000_SYS_STATUS_RXOVRR) |
                                                               DW1000_IRQ_MASK(DW1000_SYS_STATUS_TXFRS));
    //register irq listener task
    listener_thread = _listener_thread;
    lpworker_thread = _lpworker_thread;
    worker_thread_add_listener_task(listener_thread, &uwb_instance.irq_listener_task, uwb_instance.irq_topic, dw1000_sys_status_handler, NULL);

    worker_thread_add_timer_task(lpworker_thread, &status_task, status_checker, NULL, MS2ST(2), true);
    worker_thread_add_timer_task(lpworker_thread, &main_task, transmit_loop, NULL, US2ST(SLOT_SIZE/4), true);

    //since we are supervisor our data_slot_id is 0
    tx_spec.data_slot_id = 0;
    //enable rx now
    dw1000_rx_enable(&uwb_instance);
}

/*
  Sends a start slot message from supervisor.
  This message is regarded as a initiator for
  suboordinates in the body to send data at allocated slots.
 */
static void super_update_start_slot(void)
{
    struct message_spec_s msg;
    tdma_spec.slot_size = SLOT_SIZE;
    tdma_spec.cnt++;
    tx_spec.pkt_cnt++;

    memset(&msg, 0, sizeof(msg));
    //pack message
    msg.tdma_spec = tdma_spec;
    msg.tx_spec = tx_spec;
    
    dw1000_disable_transceiver(&uwb_instance);

    uint64_t scheduled_time = (dw1000_get_sys_time(&uwb_instance)+ARB_TIME_SYS_TICKS)&0xFFFFFFFFFFFFFE00ULL;

    if (tx_spec.ant_delay_cal_status) {
        update_twr_cal_tx(&msg, scheduled_time + tx_spec.ant_delay);
    } else {
        update_twr_tx(&msg, scheduled_time + tx_spec.ant_delay);
    }
    last_start_send = scheduled_time;
    if(!dw1000_scheduled_transmit(&uwb_instance, scheduled_time, 
        MSG_HEADER_SIZE + MSG_PAYLOAD_SIZE(5), &msg, true)) {
        uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "Super", "Failed to transmit Start!");
    } else {
        //tranmit loop waits until DACK is received
        transmit_state = SEND_NONE;
        transmit_complete = false;
        last_transmit = millis();
    }
}

/**
 * Handles request for time slot for modules on the same body.
 *  Request slot is usually at the end of the slot, the suboordinates
 *  only one module can make a request for slot per start message
 *  so subordinates use exponential backoff scheme for requesting data slot.
 *
 */
static void handle_request_slot(void)
{
    for (uint8_t i = 0; i < (tdma_spec.num_slots+1); i++) {
        if(slot_id_list[i] == msg.tx_spec.node_id) {
            tdma_spec.req_node_id = msg.tx_spec.node_id;
            tdma_spec.res_data_slot = i + 1;
            return;
        }
    }
    if (tdma_spec.num_slots > MAX_NUM_DEVICES) {
        //We only support upto MAX_NUM_DEVICES Devices
        return;
    }

    tdma_spec.req_node_id = msg.tx_spec.node_id;
    tdma_spec.res_data_slot = tdma_spec.num_slots;
    tdma_spec.num_slots++;
    //record allocated slot id
    slot_id_list[tdma_spec.res_data_slot] = msg.tx_spec.node_id;
    uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "Super: ", "New Node Discovered");

}

// Contention based scheme for InterBody communication

//
//    Do Contention based medium access, will be initiated by a supervisor on the
//    body to take control of the bus, so that it can talk to opposing body or do 
//    antenna delay calibration routine.

static void try_acquiring_medium(void)
{
    volatile static uint32_t test, test2;
    test = rts_backoff_mask;
    test = rts_backoff_cnt;
    test2 = transmit_state;
    if (rts_received_sleep != 0) {
        //don't send rts for now
        rts_received_sleep--;
        return;
    }
    if (rts_backoff_cnt != 0) {
        rts_backoff_cnt--;
        return;
    }
    if (transmit_state != SEND_NONE) {
        return;
    }
    transmit_state = SEND_RTS;
    //update backoff cnt for next try
    rts_backoff_cnt = (1 + (rand() & rts_backoff_mask))*4;
    rts_backoff_mask = (rts_backoff_mask << 1) | 1;
}

/*
    Initiate Medium Access routine.
    Reset backoff mask to initial range and set random number of thread runs to
    wait before sending Ready To Send
*/
static void start_acquiring_medium(void)
{
    rts_backoff_mask = INITIAL_BACKOFF_MASK;
    rts_backoff_cnt = (1 + (rand() & INITIAL_BACKOFF_MASK))*4;
    try_acquiring_medium();
    started_acquiring_medium = true;
}

/*
    Stop Medium Access routine
    Reset backoff mask here as well, in case we are called due to timeout.
    i.e. no Data ACK was received
*/
static void stop_acquiring_medium(void)
{
    rts_backoff_mask = INITIAL_BACKOFF_MASK;
    rts_backoff_cnt = (1 + (rand() & INITIAL_BACKOFF_MASK))*4;
    started_acquiring_medium = false;
    defer_acquire_medium_tstart = millis();
}

/*
    Handle Media Access messages sent by other supervisors.
*/
static void handle_comm_req_slot(struct body_comm_pkt *pkt)
{
    switch (pkt->magic) {
        case RTS_MAGIC:
            // Ready to Send Received check if it's for us
            if (pkt->target_body_id == tx_spec.body_id && receive_acquiring_medium) {
                transmit_state = SEND_CTS;
            }
            rts_received_sleep = 4; //sleep for 4 loop cycles
            num_rts_received++;
            break;
        case CTS_MAGIC:
            // Clear To Send Received ACK with Data Send
            if (pkt->target_body_id == tx_spec.body_id && receive_acquiring_medium) {
                transmit_state = SEND_DS;
            }
            num_cts_received++;
            // We have have won the contention, go silent until next cycle
            receive_acquiring_medium = false;
            stop_acquiring_medium();
            break;
        case DS_MAGIC:
            // Someone has won the contention, go silent until next cycle
            if(started_acquiring_medium) {
                stop_acquiring_medium();
            }
            receive_acquiring_medium = false;
            num_ds_received++;
            break;
        case DACK_MAGIC:
            //If an ACK to our data, do not try acquiring medium
            //this will give chance to other end to respond
            if (pkt->target_body_id != tx_spec.body_id) {
                start_acquiring_medium();
            } else {
                num_successful_transmits++;
            }
            receive_acquiring_medium = true;
            num_dack_received++;
            break;
        default:
            return;
    }
}

/*
    Regular data receive event handler, any data received goes through
    this method.
*/
static void handle_receive_event(void)
{
    //reset msg
    memset(&msg, 0, sizeof(msg));

    struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(msg), &msg);
    //Handle Data Packet
    if (rx_info.err_code == DW1000_RX_ERROR_RXOVRR) {
        num_rx_ovrr++;
    }
    if (rx_info.err_code == DW1000_RX_ERROR_NONE) {
        if (is_medium_access_msg(&msg)) {
            handle_comm_req_slot((struct body_comm_pkt*)&msg);
        } else if (msg.tx_spec.body_id == tx_spec.body_id) {
            //We belong to same body, do intra body comms if needed
            if (msg.tx_spec.data_slot_id == 255 && 
                msg.tx_spec.body_id == tx_spec.body_id) {
                //handle intra module slot request
                handle_request_slot();
            }
            num_subordinate_receives++;
            //TODO: calibration routine
            if((tx_spec.ant_delay_cal_status > 0) &&
            (msg.tx_spec.ant_delay_cal_status > 0)) {
                update_twr_cal_rx(&msg, &tx_spec, rx_info.timestamp);
            }
        } else if (msg.tdma_spec.target_body_id == tx_spec.body_id) {
            //We are receiving data from a body
            schedule_dack = true;
            //morph the slot start timestamp based on the last received packet from body
            slot_start_timestamp = rx_info.timestamp - DW1000_SID2ST(msg.tx_spec.data_slot_id);
            if (msg.tx_spec.data_slot_id == 0 ) {
                n1++;
            }
            if (msg.tx_spec.data_slot_id == 1) {
                n2++;
            }
            update_twr_rx(&msg, &tx_spec, rx_info.timestamp);
            new_range = true;
            range = msg.ds_twr_data[0].tprop;
        } else {
            //Someone is talking and its not to us, go to sleep
            //if we haven't
            if (started_acquiring_medium) {
                receive_acquiring_medium = false;
                stop_acquiring_medium();
            }
        }
    }
}


//Sends out scheduled transmits
static void transmit_loop(struct worker_thread_timer_task_s* task)
{
    (void)task;
    static uint32_t last_perf_print = 0, last_successful_recieves = 0, perf_cnt = 0;
    static uint64_t last_us, avg_period = 0, max_period = 999999, curr_period;
    perf_cnt++;
    //Transmit Loop Perf Measurement
    curr_period = micros() - last_us;;
    avg_period += curr_period;
    if (curr_period < max_period) {
        max_period = curr_period;
    }
    last_us = micros();
    if ((millis() - last_perf_print) > 5000) {

        uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "\nSuper PERF",
            "\nPERIOD: %lu/%lu RXOVRR: %d", (uint32_t)(avg_period/perf_cnt), (uint32_t)max_period, num_rx_ovrr);
        if (tx_spec.ant_delay_cal_status > 0) {
                print_cal_status(&tdma_spec, &tx_spec);
        } else {
            uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "Super PERF",
                "NID:%x TX: %d RX: %d/%d Rate: %d Range: %ld/%ld", //RTS: %d CTS: %d/%d DS: %d/%d DACK: %d", 
                tx_spec.node_id,
                num_successful_transmits, n1, n2,
                (num_successful_recieves - last_successful_recieves)/5,get_range(0), get_range(1));
                //num_rts_received, num_rts_sent, num_cts_received, num_cts_sent, num_ds_received, num_dack_received);
            
            /*uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "Super PERF","NSR: %d", num_subordinate_receives);*/
        }
        last_successful_recieves = num_successful_recieves;
        last_perf_print = millis();
        perf_cnt = 0;
        max_period = 9999999;
        avg_period = 0;
    }

    if (schedule_dack) {
        //check if its fine to setup DACK transmit
        if ((dw1000_get_sys_time(&uwb_instance) - slot_start_timestamp) >= 
            (DW1000_SID2ST(6) - ARB_TIME_SYS_TICKS)) {
            num_successful_recieves++;
            //this is an important response and trumps over everything else
            transmit_state = SEND_DACK;
            //also try acquiring medium
            start_acquiring_medium();
            schedule_dack = false;
        }
    }

    struct body_comm_pkt pkt;
    if (transmit_complete) {
        switch(transmit_state) {
            case SEND_RTS:
                num_rts_sent++;
                pkt.target_body_id = tdma_spec.target_body_id;
                pkt.body_id = tx_spec.body_id;
                pkt.magic = RTS_MAGIC;
                dw1000_disable_transceiver(&uwb_instance);
                dw1000_transmit(&uwb_instance, sizeof(struct body_comm_pkt), &pkt, true);
                last_transmit = millis();
                transmit_state = SEND_NONE;
                transmit_complete = false;
                break;
            case SEND_CTS:
                data_period = micros() - last_data_sent;
                num_cts_sent++;
                pkt.target_body_id = tdma_spec.target_body_id;
                pkt.body_id = tx_spec.body_id;
                pkt.magic = CTS_MAGIC;
                dw1000_disable_transceiver(&uwb_instance);
                dw1000_transmit(&uwb_instance, sizeof(struct body_comm_pkt), &pkt, true);
                last_transmit = millis();
                transmit_state = SEND_NONE;
                transmit_complete = false;
                break;
            case SEND_DS:
                pkt.target_body_id = tdma_spec.target_body_id;
                pkt.body_id = tx_spec.body_id;
                pkt.magic = DS_MAGIC;
                dw1000_disable_transceiver(&uwb_instance);
                dw1000_transmit(&uwb_instance, sizeof(struct body_comm_pkt), &pkt, true);
                last_transmit = millis();
                //While we send DS activate data transmission wait loop as well
                transmit_state = SEND_DATA;
                transmit_complete = false;
                break;
            case SEND_DATA:
                last_data_sent = micros();
                super_update_start_slot();
                break;
            case SEND_DACK:
                pkt.target_body_id = tdma_spec.target_body_id;
                pkt.body_id = tx_spec.body_id;
                pkt.magic = DACK_MAGIC;
                dw1000_disable_transceiver(&uwb_instance);
                dw1000_transmit(&uwb_instance, sizeof(struct body_comm_pkt), &pkt, true);
                last_transmit = millis();
                transmit_state = SEND_NONE;
                transmit_complete = false;
                break;
            case SEND_NONE:
                break;
        }
    }
    if (started_acquiring_medium && !tx_spec.ant_delay_cal_status) {
        try_acquiring_medium();
    } else if(tx_spec.ant_delay_cal_status) {
        transmit_state = SEND_NONE;
        initiate_cal_run();
    }
}

static void initiate_cal_run(void)
{
    if (transmit_complete && (dw1000_wrap_timestamp(dw1000_get_sys_time(&uwb_instance) - last_start_send) > DW1000_SID2ST(tdma_spec.num_slots+1))) {
        super_update_start_slot();
        cal_period = millis()-last_cal_init;
        last_cal_init = millis();
    }
}
