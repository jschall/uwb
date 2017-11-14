#include "tdma.h"
#include <stdlib.h>

static struct dw1000_instance_s uwb_instance;
static struct tdma_spec_s tdma_spec;
static uint8_t curr_slot;
static bool data_slot_allocated;
static struct tx_spec_s tx_spec;
static bool send_log_now;
//maintains list of slot to node id map
uint8_t slot_id_list[MAX_NUM_DEVICES];
static uint64_t slot_start_timestamp;
static struct message_spec_s msg;


/*
    Initialize TDMA vars
*/
#define DEBUG_PRINT  0

void tdma_init(uint8_t tx_type, struct tx_spec_s _tx_spec)
{
    tdma_spec.slot_size = SLOT_SIZE;
    tdma_spec.tags_online = 0;
    tdma_spec.anchors_online = 1;
    tdma_spec.res_data_slot = 0;
    tdma_spec.req_node_id = 0;
    tdma_spec.cnt = 0;
    tdma_spec.num_slots = 1;
    //seed random number generator
    srand(tx_spec.node_id);

    memcpy(&tx_spec, &_tx_spec, sizeof(tx_spec));
    curr_slot = START_SLOT;
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
    slot_start_timestamp = scheduled_time;
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

static void update_data_slot()
{
    //reset msg
    memset(&msg, 0, sizeof(msg));
    //We shall receive a data packet sometime in the middle of this sleep
    struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(msg), &msg);
    //Handle Data Packet
    if (rx_info.err_code == DW1000_RX_ERROR_NONE) {
        if (msg.tx_spec.data_slot_id == 255) {
            handle_request_slot(&msg);
        } else if (tx_spec.type == TAG) {
            update_tag(&msg, &tx_spec, rx_info.timestamp);
        } else {
            update_anchor(&msg, &tx_spec, rx_info.timestamp);
        }
    }
}

void tdma_supervisor_run(void)
{
    uint16_t cnt = 0;
    tx_spec.data_slot_id = 0;
    uint32_t last_start_sent = 0;
    while (true) {
        //we only send start here incase of faillure or initialisation
        if ((millis() - last_start_sent) >= (SLOT_SIZE/100)) {
            // For now Send Start packet after every SLOT_SIZE*10 duration
            // Later on we will do this Contention based
            super_update_start_slot();
            last_start_sent = millis();
        }
        update_data_slot();
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
    //print_tdma_spec();
    for (uint8_t i = 0; i < msg->tdma_spec.num_slots; i++) {
        uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "TWR", 
            "NID: %x RX_LEN: %d NUM_SLOTS: %d T:%.0f Status: %d Range: %f",
            msg->tx_spec.node_id, rx_info.len, msg->tdma_spec.num_slots, rx_info.timestamp/UWB_SYS_TICKS, 
            msg->ds_twr_data[i].trip_status, msg->ds_twr_data[i].tprop);
    }
}

void tdma_sniffer_run(void)
{
    uint32_t cnt = 0;
    curr_slot = 0;
    dw1000_rx_enable(&uwb_instance);
    while(true) {
        memset(&msg, 0, sizeof(msg));
        struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(msg), &msg);
        if(rx_info.err_code == DW1000_RX_ERROR_NONE) {
            cnt++;
            //process_twr(&msg.ds_twr_data, &msg.tx_spec);
            tdma_spec = msg.tdma_spec;
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
