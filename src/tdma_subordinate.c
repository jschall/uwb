#include "tdma.h"
#include <stdlib.h>

static struct dw1000_instance_s uwb_instance;
static struct tdma_spec_s tdma_spec;
static bool data_slot_allocated;
static struct tx_spec_s tx_spec;

//maintains list of slot to node id map
static uint64_t slot_start_timestamp;
static struct message_spec_s msg;

/**
 * @brief   Initialisation of the module for communication methods.
 *
 * @param[in] _tx_spec  transceiver specific initial specifications
 * @param[in] target_body_id body id with which the rangin will be done
 */
void tdma_subordinate_init(struct tx_spec_s _tx_spec)
{

    //seed random number generator
    srand(((uint32_t)tx_spec.node_id) | ((uint32_t)tx_spec.body_id<<16));

    memcpy(&tx_spec, &_tx_spec, sizeof(tx_spec));
    tx_spec.body_id = _tx_spec.node_id;
    tx_spec.type = TDMA_SUBORDINATE;

    dw1000_init(&uwb_instance, 3, BOARD_PAL_LINE_SPI3_UWB_CS, BOARD_PAL_LINE_UWB_NRST, tx_spec.ant_delay);

    dw1000_rx_enable(&uwb_instance);
}

/*
    Subordinate Runner
*/
static void req_data_slot(struct dw1000_rx_frame_info_s rx_info)
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
                req_data_slot(rx_info);
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
