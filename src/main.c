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


int main(void) {
    halInit();
    chSysInit();
    timing_init();

    uavcan_node_init();

    while(true) {
        chThdSleepMicroseconds(1000);
    }

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
