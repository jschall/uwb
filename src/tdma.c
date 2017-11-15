#include "tdma.h"
#include <stdlib.h>

static struct dw1000_instance_s uwb_instance;
static struct tdma_spec_s tdma_spec;
static bool data_slot_allocated;
static struct tx_spec_s tx_spec;
static bool send_log_now;
//maintains list of slot to node id map
uint8_t slot_id_list[MAX_NUM_DEVICES];
static uint64_t slot_start_timestamp;
static struct message_spec_s msg;

static bool started_acquiring_medium;
static uint8_t rts_backoff_cnt;
static uint8_t rts_backoff_mask;
static uint32_t defer_acquire_medium_tstart;
static bool schedule_dack;
static uint8_t rts_received_sleep;
static uint16_t num_successful_recieves;
static uint16_t num_successful_transmits;
static uint8_t start_transmit_delay;
static uint16_t num_rts_received;
static uint16_t num_cts_received;
static uint16_t num_ds_received;
static uint16_t num_dack_received;

/*
    Initialize TDMA vars
*/
#define DEBUG_PRINT  0

void tdma_init(uint8_t tx_type, struct tx_spec_s _tx_spec, uint8_t target_body_id)
{
    tdma_spec.slot_size = SLOT_SIZE;
    tdma_spec.tags_online = 0;
    tdma_spec.anchors_online = 1;
    tdma_spec.res_data_slot = 0;
    tdma_spec.req_node_id = 0;
    tdma_spec.cnt = 0;
    tdma_spec.num_slots = 1;
    tdma_spec.target_body_id = target_body_id;
    //Medium Access init
    started_acquiring_medium = false;
    rts_backoff_mask = INITIAL_BACKOFF_MASK;
    rts_backoff_cnt = 0;
    defer_acquire_medium_tstart = 0;
    schedule_dack = false;
    rts_received_sleep = 0;
    start_transmit_delay = 0;

    num_successful_recieves = 0;
    num_successful_transmits = 0;
    num_rts_received = 0;
    num_cts_received = 0;
    num_ds_received = 0;
    num_dack_received = 0;
    
    //seed random number generator
    srand(tx_spec.node_id);

    memcpy(&tx_spec, &_tx_spec, sizeof(tx_spec));
    tx_spec.body_id = _tx_spec.node_id;
    memset(slot_id_list, 0, sizeof(slot_id_list));
    if (tx_type == TDMA_SUPERVISOR) {
        twr_init(tx_spec.node_id, 0);
    }
    dw1000_init(&uwb_instance, 3, BOARD_PAL_LINE_SPI3_UWB_CS, BOARD_PAL_LINE_UWB_NRST, tx_spec.ant_delay);
    dw1000_rx_enable(&uwb_instance);
}

/*

    Supervisor Runner

*/
static void super_update_start_slot(void)
{
    if (start_transmit_delay == 0) {
        return;
    } else if (start_transmit_delay != 1) {
        start_transmit_delay--;
        return;
    }
    start_transmit_delay = 0;
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
    setup_ranging_pkt_for_tx(tdma_spec.num_slots, scheduled_time, &msg);
    
    if(!dw1000_scheduled_transmit(&uwb_instance, scheduled_time, 
        MSG_HEADER_SIZE + MSG_PAYLOAD_SIZE(5), &msg, true)) {
        uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "Super", "Failed to transmit Start!");
    }
}

static void handle_request_slot(struct message_spec_s *msg)
{
    for (uint8_t i = 0; i < (tdma_spec.num_slots+1); i++) {
        if(slot_id_list[i] == msg->tx_spec.node_id) {
            tdma_spec.req_node_id = msg->tx_spec.node_id;
            tdma_spec.res_data_slot = i + 1;
            return;
        }
    }
    if (tdma_spec.num_slots > MAX_NUM_DEVICES) {
        //We only support upto MAX_NUM_DEVICES Devices
        return;
    }

    if (msg->tx_spec.type == TAG) {
        tdma_spec.tags_online++;
    } else {
        tdma_spec.anchors_online++;
    }
    tdma_spec.req_node_id = msg->tx_spec.node_id;
    tdma_spec.res_data_slot = tdma_spec.num_slots;
    tdma_spec.num_slots++;
    //record allocated slot id
    slot_id_list[tdma_spec.res_data_slot] = msg->tx_spec.node_id;
    uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "Super: ", "New Node Discovered");

}

// Contention based scheme for InterBody communication
static bool is_medium_access_msg(struct message_spec_s *msg)
{
    if ( (((uint16_t*)msg)[0] == RTS_MAGIC) ||
         (((uint16_t*)msg)[0] == CTS_MAGIC) ||
         (((uint16_t*)msg)[0] == DS_MAGIC) ||
         (((uint16_t*)msg)[0] == DACK_MAGIC)) {
        return true;
    }
    return false;
}

static void try_acquiring_medium()
{
    if (rts_received_sleep != 0) {
        //don't send rts for now
        rts_received_sleep--;
        return;
    }
    if (rts_backoff_cnt != 0) {
        rts_backoff_cnt--;
        return;
    }
    struct body_comm_pkt pkt;
    pkt.target_body_id = tdma_spec.target_body_id;
    pkt.body_id = tx_spec.body_id;
    pkt.magic = RTS_MAGIC;
    dw1000_disable_transceiver(&uwb_instance);
    dw1000_transmit(&uwb_instance, sizeof(struct body_comm_pkt), &pkt, true);
    //update backoff cnt for next try
    rts_backoff_cnt = rand() & rts_backoff_mask;
    rts_backoff_mask = (rts_backoff_mask << 1) | 1;
}

static void start_acquiring_medium()
{
    rts_backoff_cnt = rand() & INITIAL_BACKOFF_MASK;
    rts_backoff_mask = INITIAL_BACKOFF_MASK;
    try_acquiring_medium();
    started_acquiring_medium = true;
}

static void stop_acquiring_medium()
{
    rts_backoff_cnt = rand() & INITIAL_BACKOFF_MASK;
    rts_backoff_mask = INITIAL_BACKOFF_MASK;
    defer_acquire_medium_tstart = millis();
    started_acquiring_medium = false;
}

static void handle_comm_req_slot(struct body_comm_pkt *pkt) {
    switch (pkt->magic) {
        case RTS_MAGIC:
            // Ready to Send Received check if it's for us
            if (pkt->target_body_id == tx_spec.body_id) {
                //send clear to send pkt
                pkt->magic = CTS_MAGIC;
                pkt->target_body_id = pkt->body_id;
                pkt->body_id = tx_spec.body_id;
                dw1000_disable_transceiver(&uwb_instance);
                dw1000_transmit(&uwb_instance, sizeof(struct body_comm_pkt), pkt, true);
            }
            rts_received_sleep = 4; //sleep for 4 loop cycles
            num_rts_received++;
            break;
        case CTS_MAGIC:
            // Clear To Send Received ACK with Data Send
            if (pkt->target_body_id == tx_spec.body_id) {
                //send clear to send pkt
                pkt->magic = DS_MAGIC;
                pkt->target_body_id = pkt->body_id;
                pkt->body_id = tx_spec.body_id;
                dw1000_disable_transceiver(&uwb_instance);
                dw1000_transmit(&uwb_instance, sizeof(struct body_comm_pkt), pkt, true);
            }
            num_cts_received++;
            //start transmit
            start_transmit_delay = 5;
            // We have have won the contention, go silent until next cycle
            stop_acquiring_medium();
            break;
        case DS_MAGIC:
            // Someone has won the contention, go silent until next cycle
            start_transmit_delay = 0;
            stop_acquiring_medium();
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
            num_dack_received++;
            break;
        default:
            return;
    }
}

static void update_data_slot()
{
    //reset msg
    memset(&msg, 0, sizeof(msg));
    //We shall receive a data packet sometime in the middle of this sleep
    struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(msg), &msg);
    //Handle Data Packet
    if (rx_info.err_code == DW1000_RX_ERROR_NONE) {
        if (is_medium_access_msg(&msg)) {
            handle_comm_req_slot((struct body_comm_pkt*)&msg);
        } else if (msg.tx_spec.body_id == tx_spec.body_id) {
            //We belong to same body, do intra body comms if needed
            if (msg.tx_spec.data_slot_id == 255 && 
                msg.tx_spec.body_id == tx_spec.body_id) {
                //handle intra module slot request
                handle_request_slot(&msg);
            } else if (tx_spec.type == TAG) {
                update_tag(&msg, &tx_spec, rx_info.timestamp);
            } else {
                update_anchor(&msg, &tx_spec, rx_info.timestamp);
            }
        } else if (msg.tdma_spec.target_body_id == tx_spec.body_id) {
            //We are receiving data from a body
            schedule_dack = true;
            slot_start_timestamp = rx_info.timestamp - DW1000_SID2ST(msg.tx_spec.data_slot_id);
        } else {
            //Someone is talking and its not to us, go to sleep
            //if we haven't
            if (started_acquiring_medium) {
                stop_acquiring_medium();
            }
        }
    }
    if (started_acquiring_medium) {
        //We need to be in continuous lookout for messages
        //even if we didn't send anything last cycle
        dw1000_rx_enable(&uwb_instance);
    }
    if (schedule_dack) {
        //check if its fine to setup DACK transmit
        if ((dw1000_get_sys_time(&uwb_instance) - slot_start_timestamp) >= 
            (DW1000_SID2ST(6) - ARB_TIME_SYS_TICKS)) {
            struct body_comm_pkt pkt;
            //send clear to send pkt
            pkt.magic = DACK_MAGIC;
            //This is for everyone to consume
            pkt.target_body_id = tdma_spec.target_body_id;
            pkt.body_id = tx_spec.body_id;
            dw1000_disable_transceiver(&uwb_instance);
            dw1000_transmit(&uwb_instance, sizeof(struct body_comm_pkt), &pkt, true);
            chThdSleepMicroseconds(SLOT_SIZE/4);
            //also try acquiring medium
            start_acquiring_medium();
            schedule_dack = false;
            num_successful_recieves++;
        }
    }
}

void tdma_supervisor_run(void)
{
    uint16_t cnt = 0;
    tx_spec.data_slot_id = 0;
    uint32_t last_perf_print = 0, last_successful_recieves = 0;
    while (true) {
        update_data_slot();
        super_update_start_slot();
        if ((millis() - last_perf_print) > 5000) {

            uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "Super PERF",
                "NID:%x TX: %d RX: %d Rate: %d RTS: %d CTS: %d DS: %d DACK: %d", 
                tx_spec.node_id,
                num_successful_transmits, num_successful_recieves,
                (num_successful_recieves - last_successful_recieves)/5,
                num_rts_received, num_cts_received, num_ds_received, num_dack_received);

            last_successful_recieves = num_successful_recieves;
            last_perf_print = millis();
        }
        //we only send start here incase of faillure or initialisation
        if (((millis() - defer_acquire_medium_tstart) >= DEFER_TIME) || 
            started_acquiring_medium) {
            try_acquiring_medium();
        }
        chThdSleepMicroseconds(SLOT_SIZE/4);
    }
}

/*
    Subordinate Runner
*/
static void req_data_slot(uint8_t num_starts, struct dw1000_rx_frame_info_s rx_info)
{
    static uint8_t skip_req;
    static uint8_t skip_mask = 1;
    if (skip_req != 0) {
        skip_req--;
        return;
    }
    //In case the request fails to receive the supervisor time we skip request
    skip_req = rand() & skip_mask;
    skip_mask = (skip_mask << 1) | 1;

    struct message_spec_s msg;
    uint64_t scheduled_time = (rx_info.timestamp+DW1000_SID2ST(tdma_spec.num_slots))&0xFFFFFFFFFFFFFE00ULL;
    //pack message
    msg.tdma_spec = tdma_spec;
    msg.tx_spec = tx_spec;
    msg.tx_spec.data_slot_id = 255;
    dw1000_disable_transceiver(&uwb_instance);
    if(dw1000_scheduled_transmit(&uwb_instance, scheduled_time, 
        MSG_HEADER_SIZE + MSG_PAYLOAD_SIZE(5), &msg, true)) {
        uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "Sub: ", "Requesting Data Slot @ %u for %x", tdma_spec.num_slots+1, tx_spec.node_id);
    } else {
        uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "Sub: ", "Requesting Data Slot Failed %d", MSG_HEADER_SIZE);
    }
}

static void update_subordinate(void)
{
    //reset msg
    memset(&msg, 0, sizeof(msg));
    struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(msg), &msg);
    static bool transmit_scheduled = false;
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
        if(msg.tx_spec.data_slot_id == 0) { //this is a message from supervisor
            transmit_scheduled = false;
            slot_start_timestamp = rx_info.timestamp;
            tdma_spec = msg.tdma_spec;

            if (!data_slot_allocated && tdma_spec.req_node_id == tx_spec.node_id) {
                tx_spec.data_slot_id = tdma_spec.res_data_slot;
                data_slot_allocated = true;
                twr_init(tx_spec.node_id, tx_spec.data_slot_id);
            }
            //check if we should ask for data slot, if yes request
            if(!data_slot_allocated) {
                req_data_slot(tdma_spec.cnt, rx_info);
            }
        }

        if(!data_slot_allocated) {
            return;
        }
        //Also setup ranging request of our own
        if (data_slot_allocated) {
            if (tx_spec.type == TAG) {
                update_tag(&msg, &tx_spec, rx_info.timestamp);
            } else {
                update_anchor(&msg, &tx_spec, rx_info.timestamp);
            }
        }
    }
    if (!transmit_scheduled && data_slot_allocated) {
        //check if its fine to setup next transmit
        if ((dw1000_get_sys_time(&uwb_instance) - slot_start_timestamp) >= (DW1000_SID2ST(tx_spec.data_slot_id) - ARB_TIME_SYS_TICKS)) {
            dw1000_disable_transceiver(&uwb_instance);
            //setup future transmit
            uint64_t scheduled_time = (slot_start_timestamp+DW1000_SID2ST(tx_spec.data_slot_id))&0xFFFFFFFFFFFFFE00ULL;
            //create TWR data sets to send
            memset(&msg, 0, sizeof(msg));
            //pack message
            msg.tdma_spec = tdma_spec;
            msg.tx_spec = tx_spec;
            setup_ranging_pkt_for_tx(tdma_spec.num_slots, scheduled_time, &msg);
            //send ranging pkt
            if (dw1000_scheduled_transmit(&uwb_instance, scheduled_time, 
                           MSG_HEADER_SIZE + MSG_PAYLOAD_SIZE(5), &msg, true)) {
                tx_spec.pkt_cnt++;
            }
            transmit_scheduled = true;
        }
    }
}

void tdma_subordinate_run(void)
{
    while (true) {
        update_subordinate();
        chThdSleepMicroseconds(SLOT_SIZE/4);
    }
}

/*

    TDMA Sniffer Runner

*/

static void print_info(struct message_spec_s *msg, struct dw1000_rx_frame_info_s rx_info)
{
    if(!send_log_now) {
        return;
    }

    if(is_medium_access_msg(msg)) {
        struct body_comm_pkt *pkt = (struct body_comm_pkt *)msg;
        switch (pkt->magic) {
            case RTS_MAGIC:
                uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "RTS_MAGIC",
                    "T:%.0f BID: %x TID: %x", rx_info.timestamp/UWB_SYS_TICKS, pkt->body_id, pkt->target_body_id);
                break;
            case CTS_MAGIC:
                uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "CTS_MAGIC",
                    "T:%.0f BID: %x TID: %x", rx_info.timestamp/UWB_SYS_TICKS, pkt->body_id, pkt->target_body_id);
                break;
            case DS_MAGIC:
                uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "DS_MAGIC",
                    "T:%.0f BID: %x TID: %x", rx_info.timestamp/UWB_SYS_TICKS, pkt->body_id, pkt->target_body_id);
                break;
            case DACK_MAGIC:
                uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "DACK_MAGIC",
                    "T:%.0f BID: %x TID: %x", rx_info.timestamp/UWB_SYS_TICKS, pkt->body_id, pkt->target_body_id);
                break;
        };
    }else {    //print_tdma_spec();
        for (uint8_t i = 0; i < msg->tdma_spec.num_slots; i++) {
            uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "TWR", 
                "NID: %x RX_LEN: %d NUM_SLOTS: %d T:%.0f Status: %d Range: %f",
                msg->tx_spec.node_id, rx_info.len, msg->tdma_spec.num_slots, rx_info.timestamp/UWB_SYS_TICKS, 
                msg->ds_twr_data[i].trip_status, msg->ds_twr_data[i].tprop);
        }
    }
}

void tdma_sniffer_run(void)
{
    uint32_t cnt = 0;
    dw1000_rx_enable(&uwb_instance);
    while(true) {
        memset(&msg, 0, sizeof(msg));
        struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(msg), &msg);
        if(rx_info.err_code == DW1000_RX_ERROR_NONE) {
            cnt++;
            //process_twr(&msg.ds_twr_data, &msg.tx_spec);
            //tdma_spec = msg.tdma_spec;
            print_info(&msg, rx_info);
        }
        if(cnt % 100 <= 10) {
            send_log_now = true;
        } else if (send_log_now) {
            uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "TWR", "\n\n\n");
            send_log_now = false;
        }
        chThdSleepMicroseconds(SLOT_SIZE/8);
    }
}
