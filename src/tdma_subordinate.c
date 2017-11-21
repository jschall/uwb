#include "tdma.h"
#include <stdlib.h>

static struct tdma_spec_s tdma_spec;
static bool data_slot_allocated;
static struct tx_spec_s tx_spec;

static struct worker_thread_timer_task_s main_task;
static struct worker_thread_timer_task_s status_task;

static uint32_t last_receive;
static uint32_t last_transmit;
static bool transmit_complete;
static bool transmit_scheduled;
volatile static uint64_t scheduled_time;
volatile static uint64_t curr_time;

//maintains list of slot to node id map
static uint64_t slot_start_timestamp;
static struct message_spec_s msg;


#define RX_TIMEOUT 20   //20ms Dw1000 timeout
#define TX_TIMEOUT 2    //2ms Dw1000 Tx timeout

static void handle_receive_event(void);
static void transmit_loop(struct worker_thread_timer_task_s* task);

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
    if ((millis() - last_receive) > RX_TIMEOUT) {
        if (dw1000_get_status(&uwb_instance) & DW1000_IRQ_MASK(DW1000_SYS_STATUS_RXOVRR)) {
            dw1000_disable_transceiver(&uwb_instance);
            dw1000_swap_rx_buffers(&uwb_instance);
            dw1000_rx_softreset(&uwb_instance);
            // Receiver must be reset to exit errored state
            dw1000_rx_enable(&uwb_instance);
        }
        last_receive = millis();
    }

    if ((millis() - last_transmit) > TX_TIMEOUT) {
        transmit_complete = true;
    }
}

/**
 * @brief   Initialisation of the module for communication methods.
 *
 * @param[in] _tx_spec  transceiver specific initial specifications
 * @param[in] target_body_id body id with which the rangin will be done
 */
void tdma_subordinate_init(struct tx_spec_s _tx_spec, struct worker_thread_s* worker_thread, struct worker_thread_s* listener_thread)
{

    //seed random number generator
    srand(((uint32_t)tx_spec.node_id) | ((uint32_t)tx_spec.body_id<<16));

    last_receive = 0;
    last_transmit = 0;
    transmit_scheduled = true;
    data_slot_allocated = false;
    memcpy(&tx_spec, &_tx_spec, sizeof(tx_spec));
    tx_spec.type = TDMA_SUBORDINATE;


    dw1000_init(&uwb_instance, 3, BOARD_PAL_LINE_SPI3_UWB_CS, BOARD_PAL_LINE_UWB_NRST, tx_spec.ant_delay);
    //register irq listener task
    dw1000_setup_irq(&uwb_instance, FW_EXT_IRQ_PORT(GPIOA), 1, DW1000_IRQ_MASK(DW1000_SYS_STATUS_LDEDONE) | 
                                                               DW1000_IRQ_MASK(DW1000_SYS_STATUS_RXOVRR) |
                                                               DW1000_IRQ_MASK(DW1000_SYS_STATUS_TXFRS));

    pubsub_init_and_register_listener(uwb_instance.irq_topic, &uwb_instance.irq_listener, dw1000_sys_status_handler, NULL);
    worker_thread_add_listener_task(listener_thread, &uwb_instance.irq_listener_task, &uwb_instance.irq_listener);
    worker_thread_add_timer_task(worker_thread, &status_task, status_checker, NULL, MS2ST(2), true);
    worker_thread_add_timer_task(worker_thread, &main_task, transmit_loop, NULL, US2ST(SLOT_SIZE/4), true);

    dw1000_rx_enable(&uwb_instance);
}

//
//    Subordinate Runner
//
static void req_data_slot(struct dw1000_rx_frame_info_s rx_info)
{
    static uint8_t skip_req;
    static uint8_t skip_mask = 1;
    if (skip_req != 0) {
        skip_req--;
        return;
    }
    //In case the request fails to receive the supervisor time we skip request
    skip_req = rand() & (uint8_t)skip_mask;
    skip_mask = (skip_mask << 1) | 1;

    struct message_spec_s msg;
    scheduled_time = (rx_info.timestamp+DW1000_SID2ST(tdma_spec.num_slots))&0xFFFFFFFFFFFFFE00ULL;
    curr_time = dw1000_get_sys_time(&uwb_instance);
    //pack message
    msg.tdma_spec = tdma_spec;
    msg.tx_spec = tx_spec;
    msg.tx_spec.data_slot_id = 255;
    dw1000_disable_transceiver(&uwb_instance);
    if(dw1000_scheduled_transmit(&uwb_instance, scheduled_time, 
        MSG_HEADER_SIZE, &msg, true)) {
        uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "Sub: ", "Requesting Data Slot @ %u for %x", tdma_spec.num_slots+1, tx_spec.node_id);
    } else {
        uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "Sub: ", "Requesting Data Slot Failed %d", MSG_HEADER_SIZE);
    }
}

static void handle_receive_event(void)
{
    //reset msg
    memset(&msg, 0, sizeof(msg));
    struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(msg), &msg);
    if(rx_info.err_code == DW1000_RX_ERROR_NONE) {
        if (is_medium_access_msg(&msg)) {
            //this is supervisor business do nothing
            return;
        }

        if (msg.tx_spec.data_slot_id >= MAX_NUM_DEVICES) {
            //probably a device requesting data slot
            return;
        }
        if (msg.tdma_spec.num_slots - 1 < tx_spec.data_slot_id) {
            data_slot_allocated = false;
        }
        if((msg.tx_spec.data_slot_id == 0) && (msg.tx_spec.body_id == tx_spec.body_id)) { //this is a message from our body's supervisor
            transmit_scheduled = false;
            slot_start_timestamp = rx_info.timestamp;
            tdma_spec = msg.tdma_spec;

            if (!data_slot_allocated && tdma_spec.req_node_id == tx_spec.node_id) {
                tx_spec.data_slot_id = tdma_spec.res_data_slot;
                data_slot_allocated = true;
                twr_init();
            }
            //check if we should ask for data slot, if yes request
            if(!data_slot_allocated && transmit_complete) {
                req_data_slot(rx_info);
            }
        }

        if(!data_slot_allocated) {
            return;
        }
        //Also setup ranging request of our own
        if (data_slot_allocated) {
            if (tx_spec.ant_delay_cal && msg.tx_spec.ant_delay_cal && (msg.tx_spec.body_id == tx_spec.body_id)) {
                update_twr_cal_rx(&msg, &tx_spec, rx_info.timestamp);
            } else if(msg.tdma_spec.target_body_id == tx_spec.body_id) {
                //we are receiving TWR request
                update_twr_rx(&msg, &tx_spec, rx_info.timestamp);
            }
        }
    }
}

static void transmit_loop(struct worker_thread_timer_task_s* task)
{
    (void)task;
    if ((!transmit_scheduled) && data_slot_allocated) {
        //we will schedule our transmit right before
        //previous timeslot is over
        if ((dw1000_get_sys_time(&uwb_instance) - slot_start_timestamp) >= (DW1000_SID2ST(tx_spec.data_slot_id) - ARB_TIME_SYS_TICKS)) {
            dw1000_disable_transceiver(&uwb_instance);
            //setup future transmit
            uint64_t scheduled_time = (slot_start_timestamp+DW1000_SID2ST(tx_spec.data_slot_id))&0xFFFFFFFFFFFFFE00ULL;
            //create TWR data sets to send
            memset(&msg, 0, sizeof(msg));
            //pack message
            msg.tdma_spec = tdma_spec;
            msg.tx_spec = tx_spec;
            if (tx_spec.ant_delay_cal) {
                update_twr_cal_tx(&msg, scheduled_time + tx_spec.ant_delay);
            } else {
                update_twr_tx(&msg, scheduled_time + tx_spec.ant_delay);
            }
            //send ranging pkt
            if (dw1000_scheduled_transmit(&uwb_instance, scheduled_time, 
                           MSG_HEADER_SIZE + MSG_PAYLOAD_SIZE(5), &msg, true)) {
                tx_spec.pkt_cnt++;
            }
            transmit_scheduled = true;
        }
    }
}
