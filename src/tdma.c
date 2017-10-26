#include "tdma.h"

static struct dw1000_instance_s uwb_instance;
static struct tdma_spec_s tdma_spec;
static bool request_data_slot;
static uint8_t curr_slot;
static bool data_slot_allocated;
static struct tx_spec_s tx_spec;

/*

    Initialize TDMA vars

*/

void tdma_init(uint8_t unique_id)
{
    tdma_spec.slot_size = SLOT_SIZE;
    tdma_spec.num_tx_online = 0;
    tdma_spec.res_data_slot = 0;
    tdma_spec.req_node_id = 0;
    tdma_spec.cnt = 0;
    tx_spec.node_id = unique_id;
    tx_spec.type = STATIC_TAG;
    tx_spec.pkt_cnt = 0;
    tx_spec.data_slot_id = 0;
}


/*

    Supervisor Runner

*/
static void super_update_start_slot()
{
    struct message_spec_s msg;
    tdma_spec.slot_size = SLOT_SIZE;
    tdma_spec.cnt++;
    tx_spec.pkt_cnt++;
    //pack message
    msg.tdma_spec = tdma_spec;
    msg.tx_spec = tx_spec;
    msg.target_node_id = 128; //we are broadcasting
    dw1000_disable_transceiver(&uwb_instance);
    dw1000_transmit(&uwb_instance, sizeof(msg), &msg, true);
    curr_slot = DATA_SLOT;
}


static void handle_request_slot()
{
    struct message_spec_s msg;
    struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(msg), &msg);

    if (rx_info.err_code == DW1000_RX_ERROR_NONE) {
        for (uint8_t i = 0; i < tdma_spec.num_tx_online; i++) {
            if(slot_id_list[i] == msg.tx_spec.node_id) {
                tdma_spec.req_node_id = msg.tx_spec.node_id;
                tdma_spec.res_data_slot = i + 1;
                uavcan_acquire();
                uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "Super: ", "Repeat Slot Request");
                uavcan_release();
                return;               
            }
        }
        if(tdma_spec.num_tx_online > 127) {
            //We only support upto 127 Devices
            return;
        }
        tdma_spec.num_tx_online++;
        tdma_spec.req_node_id = msg.tx_spec.node_id;
        tdma_spec.res_data_slot = tdma_spec.num_tx_online;
        //record allocated slot id
        slot_id_list[tdma_spec.res_data_slot-1] = msg.tx_spec.node_id;
        uavcan_acquire();
        uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "Super: ", "New Node Discovered");
        uavcan_release();
    }
    curr_slot = START_SLOT;
}

static void update_data_slot(bool supervisor)
{
    struct message_spec_s msg;
    static uint8_t slot_cnt = 0;
    if (slot_cnt >= tdma_spec.num_tx_online) {
        slot_cnt = 1;
        curr_slot = REQUEST_SLOT;
        dw1000_rx_enable(&uwb_instance);
        return;
    }
    //We shall receive a data packet sometime in the middle of this sleep
    chThdSleepMicroseconds(1000);
    struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(msg), &msg);
    //Handle Data Packet



    dw1000_rx_enable(&uwb_instance);
    slot_cnt++;
}

void tdma_supervisor_run()
{
    dw1000_init(&uwb_instance, 3, BOARD_PAL_LINE_SPI3_UWB_CS, BOARD_PAL_LINE_UWB_NRST);
    dw1000_rx_enable(&uwb_instance);
    uint16_t cnt = 0;
    char dat[50];
    while (true) {
        switch (curr_slot) {
            case START_SLOT:
                //split into two sleeps, so as to ensure 
                //we receive packets in the center of upcomming sleeps
                chThdSleepMicroseconds(500);
                super_update_start_slot();
                chThdSleepMicroseconds(500);
                break;
            case DATA_SLOT:
                update_data_slot(true);
                break;
            case REQUEST_SLOT:
                //We might receive a request packet sometime in the middle of this sleep
                chThdSleepMicroseconds(1000);
                handle_request_slot();
                break;
            default:
                break;
        };
        //Send Status
        if (cnt == 200) {
            sprintf(dat,"%d/%d %d %d", tx_spec.node_id, tdma_spec.cnt ,tx_spec.pkt_cnt, tdma_spec.num_tx_online);
            uavcan_acquire();
            uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "Super: ", dat);
            uavcan_release();
            cnt = 0;
        }
        cnt++;
    }
}



/*

    Subordinate Runner

*/


static void req_data_slot(struct dw1000_rx_frame_info_s rx_info)
{
    struct message_spec_s msg;
    uint64_t scheduled_time = (rx_info.timestamp+DW1000_SID2ST(tdma_spec.num_tx_online+1))&0xFFFFFFFFFFFFFE00ULL;
    //pack message
    msg.tdma_spec = tdma_spec;
    msg.tx_spec = tx_spec;
    msg.target_node_id = 128; //we are requesting data slot
    uavcan_acquire();
    uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "Sub: ", "Requesting Data Slot");
    uavcan_release();
    dw1000_disable_transceiver(&uwb_instance);
    dw1000_scheduled_transmit(&uwb_instance, scheduled_time, sizeof(msg), &msg, true);
    //Go and wait for time slot allocation
    dw1000_rx_enable(&uwb_instance);
    curr_slot = START_SLOT;
}

static void update_subordinate()
{
    struct message_spec_s msg;
    struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(msg), &msg);

    if(rx_info.err_code == DW1000_RX_ERROR_NONE) {
        if(msg.tx_spec.data_slot_id == 0) { //this is a message from supervisor
            tdma_spec = msg.tdma_spec;
        } else {
            return;
        }
        //check if it's our turn to ask for data slot
        if(tdma_spec.cnt%256 == tx_spec.node_id && !data_slot_allocated) {
            request_data_slot = true;
        }

        if (request_data_slot && tdma_spec.req_node_id == tx_spec.node_id) {
            tx_spec.data_slot_id = tdma_spec.res_data_slot;
            data_slot_allocated = true;
        } else if(request_data_slot) {
            req_data_slot(rx_info);
            request_data_slot = false;
            return;
        }
        if(!data_slot_allocated) {
            return;
        }
        //setup future transmit
        uint64_t scheduled_time = (rx_info.timestamp+DW1000_SID2ST(tx_spec.data_slot_id))&0xFFFFFFFFFFFFFE00ULL;
        //create TWR data set to send
        //pack message
        msg.tdma_spec = tdma_spec;
        msg.tx_spec = tx_spec;
        msg.target_node_id = 1; //target node we are doing twr with
        dw1000_disable_transceiver(&uwb_instance);
        if (dw1000_scheduled_transmit(&uwb_instance, scheduled_time, sizeof(msg), &msg, true)) {
            tx_spec.pkt_cnt++;
        }
        dw1000_rx_enable(&uwb_instance);
    }
}

void tdma_subordinate_run()
{
    dw1000_init(&uwb_instance, 3, BOARD_PAL_LINE_SPI3_UWB_CS, BOARD_PAL_LINE_UWB_NRST);
    dw1000_rx_enable(&uwb_instance);
    uint16_t cnt = 0;
    char dat[50];
    while (true) {
        curr_slot = START_SLOT;
        update_subordinate();
        //Send Status
        if (cnt == 400) {
            sprintf(dat,"%d/%d %d %d", tx_spec.node_id, tdma_spec.cnt ,tx_spec.pkt_cnt, tx_spec.data_slot_id);
            uavcan_acquire();
            uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "Sub: ", dat);
            uavcan_release();
            cnt = 0;
        }
        cnt++;
        chThdSleepMicroseconds(500);
    }
}

/*

    TDMA Sniffer Runner

*/
void tdma_sniffer_run()
{
    dw1000_init(&uwb_instance, 3, BOARD_PAL_LINE_SPI3_UWB_CS, BOARD_PAL_LINE_UWB_NRST);
    dw1000_rx_enable(&uwb_instance);
    struct message_spec_s msg;
    char dat[50];
    uint8_t cnt = 0;
    while(true) {
        struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(msg), &msg);
        if(rx_info.err_code == DW1000_RX_ERROR_NONE && cnt == 200) {
            sprintf(dat,"%d/%d %d %d", msg.tx_spec.node_id, msg.tdma_spec.cnt ,msg.tx_spec.pkt_cnt, msg.tx_spec.data_slot_id);
            uavcan_acquire();
            uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "Sniff: ", dat);
            uavcan_release();
            cnt = 0;
        }
        cnt++;
        chThdSleepMicroseconds(1000);
    }
}
