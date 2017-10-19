/*
 *    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "ch.h"
#include "hal.h"
#include <common/timing.h>
#include <common/helpers.h>
#include "dw1000.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <common/param.h>
#include <common/uavcan.h>
#include "uavcan_node.h"

static void ss_twr_initiator_run(void) {
    struct dw1000_instance_s uwb_instance;
    dw1000_init(&uwb_instance, 3, BOARD_PAL_LINE_SPI3_UWB_CS, BOARD_PAL_LINE_UWB_NRST);
    uint32_t tprev_us = 0;
    uint8_t i=0;
    while (true) {
        uint32_t tnow_us = micros();
        if (tnow_us-tprev_us > 200000) {
            uavcan_acquire();
            tprev_us = tnow_us;
            char msg[50];
            int n = sprintf(msg, "message %u", i);
            dw1000_transmit(&uwb_instance, n, msg, true);
//             uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "init tx", msg);

            while(true) {
                if (micros()-tnow_us > 5000) {
//                     uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "init rx t/o", "");
                    dw1000_disable_transceiver(&uwb_instance);
                    break;
                }

                char rxbuf[50];
                struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(rxbuf)-1, rxbuf);
                if (rx_info.err_code == DW1000_RX_ERROR_NONE) {
                    dw1000_disable_transceiver(&uwb_instance);

                    uint64_t tround = (rx_info.timestamp-dw1000_get_tx_stamp(&uwb_instance));
                    uint32_t treply;
                    memcpy(&treply, rxbuf, sizeof(treply));
                    double clksca = (double)rx_info.rx_ttcko/(double)rx_info.rx_ttcki;
                    double tprop = 0.5*(tround-treply);
                    sprintf(msg, "%f %f", tprop, clksca);
//                     uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "init rx", msg);
                    break;
                }
            }

            i++;
            uavcan_release();
        }
        chThdSleepMicroseconds(1000);
    }
}

static void ss_twr_responder_run(void) {
    struct dw1000_instance_s uwb_instance;
    dw1000_init(&uwb_instance, 3, BOARD_PAL_LINE_SPI3_UWB_CS, BOARD_PAL_LINE_UWB_NRST);
    dw1000_rx_enable(&uwb_instance);
    uint32_t tprev_us = 0;
    while (true) {
        char rxbuf[50];
        struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(rxbuf)-1, rxbuf);
        if (rx_info.err_code == DW1000_RX_ERROR_NONE) {
            uavcan_acquire();
            dw1000_disable_transceiver(&uwb_instance);
            uint64_t scheduled_time = (rx_info.timestamp+65536ULL*1000ULL)&0xFFFFFFFFFFFFFE00ULL;
            uint32_t dly = (uint32_t)(scheduled_time-rx_info.timestamp);
            if (dw1000_scheduled_transmit(&uwb_instance, scheduled_time, sizeof(dly), &dly, true)) {
                rxbuf[rx_info.len] = 0;
//                 uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "resp", rxbuf);
            } else {
                uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "resp fail", "");
            }
            uavcan_release();
        }
    }
}

static void receiver_run(void) {
    struct dw1000_instance_s uwb_instance;
    dw1000_init(&uwb_instance, 3, BOARD_PAL_LINE_SPI3_UWB_CS, BOARD_PAL_LINE_UWB_NRST);
    dw1000_rx_enable(&uwb_instance);
    uint32_t tprev_us = 0;
    while (true) {
        uint32_t tnow_us = micros();
        if (tnow_us-tprev_us > 100000) {
            tprev_us = tnow_us;
            char rxbuf[50];
            struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(rxbuf)-1, rxbuf);
            if (rx_info.err_code == DW1000_RX_ERROR_NONE) {
                rxbuf[rx_info.len] = 0;
                char msg[100];
                sprintf(msg, "%llu %e %s", rx_info.timestamp, ((float)rx_info.rx_ttcko)/((float)rx_info.rx_ttcki), rxbuf);
                uavcan_acquire();
                uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "rx", msg);
                uavcan_release();
            }
        }
        chThdSleepMicroseconds(1000);
    }
}

static void transmitter_run(void) {
    struct dw1000_instance_s uwb_instance;
    dw1000_init(&uwb_instance, 3, BOARD_PAL_LINE_SPI3_UWB_CS, BOARD_PAL_LINE_UWB_NRST);
    uint32_t tprev_us = 0;
    uint8_t i=0;
    while (true) {
        uint32_t tnow_us = micros();
        if (tnow_us-tprev_us > 200000) {
            tprev_us = tnow_us;
            char msg[50];
            int n = snprintf(msg, sizeof(msg), "message %u", i);
            if (n > 0 && n<sizeof(msg)) {
                dw1000_transmit(&uwb_instance, n, msg, false);
                uavcan_acquire();
                uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "tx", msg);
                uavcan_release();
            }
            i++;
        }
        chThdSleepMicroseconds(1000);
    }
}

PARAM_DEFINE_FLOAT32_PARAM_STATIC(param_a, "a", 4, 3, 5)
PARAM_DEFINE_INT64_PARAM_STATIC(param_b, "b", 7, 7, 7)
PARAM_DEFINE_INT32_PARAM_STATIC(param_c, "c", 7, 7, 7)
PARAM_DEFINE_INT16_PARAM_STATIC(param_d, "d", 7, 7, 7)
PARAM_DEFINE_INT8_PARAM_STATIC(param_e, "e", 7, 7, 7)
PARAM_DEFINE_UINT32_PARAM_STATIC(param_f, "f", 7, 7, 7)
PARAM_DEFINE_UINT16_PARAM_STATIC(param_g, "g", 7, 7, 7)
PARAM_DEFINE_UINT8_PARAM_STATIC(param_h, "h", 7, 7, 7)
PARAM_DEFINE_STRING_PARAM_STATIC(param_j, "j", "blah", 128)
PARAM_DEFINE_BOOL_PARAM_STATIC(param_i, "i", true)

#include <omd_uavcan/omd_uavcan.h>
#include <uavcan.protocol.NodeStatus.h>

static uint32_t encode_nodestatus(void* in, void* out) {
    return encode_uavcan_protocol_NodeStatus(out, in);
}

static struct omd_uavcan_message_descriptor_s uavcan_protocol_NodeStatus_descriptor = {
    UAVCAN_PROTOCOL_NODESTATUS_DT_SIG,
    UAVCAN_PROTOCOL_NODESTATUS_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_protocol_NodeStatus_s),
    UAVCAN_PROTOCOL_NODESTATUS_MAX_PACK_SIZE,
    encode_nodestatus,
    decode_uavcan_protocol_NodeStatus
};

static void nodestatus_handler(struct omd_uavcan_deserialized_message_s* msg_wrapper, void* ctx) {
    struct uavcan_protocol_NodeStatus_s* msg = (struct uavcan_protocol_NodeStatus_s*)msg_wrapper->msg;

    chSysHalt("oh shit, i don't know what to do");
}

RUN_BEFORE(OMD_UAVCAN_INIT) {
    const CANConfig cancfg = {
        CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
        CAN_BTR_SJW(0) | CAN_BTR_TS2(2-1) |
        CAN_BTR_TS1(15-1) | CAN_BTR_BRP((STM32_PCLK1/18)/1000000 - 1)
    };

    canStart(&CAND1, &cancfg);
}

int main(void) {
    omd_uavcan_set_node_id(0, 42);

    struct uavcan_protocol_NodeStatus_s msg;
    msg.uptime_sec = 0;
    msg.health = UAVCAN_PROTOCOL_NODESTATUS_HEALTH_OK;
    msg.mode = UAVCAN_PROTOCOL_NODESTATUS_MODE_OPERATIONAL;
    msg.sub_mode = 0;
    msg.vendor_specific_status_code = 0;

    while(true) {
        msg.uptime_sec++;
        omd_uavcan_broadcast(0, &uavcan_protocol_NodeStatus_descriptor, CANARD_TRANSFER_PRIORITY_LOW, &msg);
        chThdSleepMilliseconds(1000);
    }

//     struct pubsub_listener_s nodestatus_listener;
//     struct pubsub_listener_s param_getset_req_listener;
//
//     omd_uavcan_subscribe(&nodestatus_listener, &uavcan_protocol_NodeStatus_descriptor, nodestatus_handler, NULL);
//     omd_uavcan_subscribe(&param_getset_req_listener, &uavcan_protocol_param_GetSet_req_descriptor, );
//
//     const struct pubsub_listener_s** listener_list = {&nodestatus_listener, &param_getset_req_listener};
//     pubsub_multiple_listener_handle_until_timeout(sizeof(listener_list)/sizeof(listener_list[0]), listener_list, TIME_INFINITE);

//     uavcan_node_init();

//     param_print_table();

//     uint8_t unique_id[12];
//     board_get_unique_id(unique_id, sizeof(unique_id));

//     if (unique_id[10] == 0x1F) {
//         ss_twr_initiator_run();
//     } else {
//         ss_twr_responder_run();
//     }

    return 0;
}
