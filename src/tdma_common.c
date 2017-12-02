#include "tdma.h"
#include <stdlib.h>

static bool send_log_now;
struct dw1000_instance_s uwb_instance;
static struct message_spec_s msg;

//
//    Check if the msg received is actually a ping for medium access scheme routine
//
bool is_medium_access_msg(struct message_spec_s *_msg)
{
    if ( (((uint16_t*)_msg)[0] == RTS_MAGIC) ||
         (((uint16_t*)_msg)[0] == CTS_MAGIC) ||
         (((uint16_t*)_msg)[0] == DS_MAGIC) ||
         (((uint16_t*)_msg)[0] == DACK_MAGIC)) {
        return true;
    }
    return false;
}

static void print_info(struct message_spec_s *_msg, struct dw1000_rx_frame_info_s rx_info)
{
    if(!send_log_now) {
        return;
    }

    if(is_medium_access_msg(_msg)) {
        struct body_comm_pkt *pkt = (struct body_comm_pkt *)_msg;
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
        for (uint8_t i = 0; i < _msg->tdma_spec.num_slots; i++) {
            uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "TWR", 
                "NID: %x NUM_SLOTS: %d Range: %ld",
                _msg->tx_spec.node_id, _msg->tdma_spec.num_slots, (uint32_t)_msg->ds_twr_data[i].tprop);
        }
    }
}



//    TDMA Sniffer Runner


void tdma_sniffer_run(void)
{
    uint32_t cnt = 0;
    dw1000_init(&uwb_instance, 3, BOARD_PAL_LINE_SPI3_UWB_CS, BOARD_PAL_LINE_UWB_NRST, 0);
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
        if(cnt % 100 <= 30) {
            send_log_now = true;
        } else if (send_log_now) {
            uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "TWR", "\n\n\n");
            send_log_now = false;
        }
        chThdSleepMicroseconds(SLOT_SIZE/8);
    }
}
