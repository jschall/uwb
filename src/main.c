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

#include <ch.h>
#include <modules/dw1000/dw1000.h>
#include <modules/uavcan_debug/uavcan_debug.h>
#include <modules/lpwork_thread/lpwork_thread.h>
#include <modules/param/param.h>
#include <common/helpers.h>
#include <string.h>

#define TREPLY 70000000

struct __attribute__((packed)) ds_twr_msg_s {
    uint8_t seq;
    int32_t tround1;
    int32_t treply2;
};

#define SAMPLES_SIZE 50
static float samples[SAMPLES_SIZE];
static uint32_t samples_collected;

PARAM_DEFINE_BOOL_PARAM_STATIC(device_is_initiator, "initiator", true)
PARAM_DEFINE_UINT32_PARAM_STATIC(twait, "twait", 1000, 0, 1000000)

static struct dw1000_instance_s uwb_instance;

static void responder_run(void) {
    struct ds_twr_msg_s msg;
    struct dw1000_rx_frame_info_s rx_info;
    uint64_t t1, t3, t6;
    dw1000_rx_enable(&uwb_instance);
    while (true) {
        rx_info = dw1000_receive(&uwb_instance, sizeof(msg), &msg);
        if (rx_info.err_code == DW1000_RX_ERROR_NONE && rx_info.len == sizeof(msg) && msg.seq == 0) {
            msg.seq++;
            t1 = rx_info.timestamp;
            t3 = (rx_info.timestamp + TREPLY) & ~0x1ffULL;
            dw1000_disable_transceiver(&uwb_instance);
            dw1000_scheduled_transmit(&uwb_instance, rx_info.timestamp+TREPLY, sizeof(msg), &msg, true);

            chThdSleep(twait);
            rx_info = dw1000_receive(&uwb_instance, sizeof(msg), &msg);
            if (rx_info.err_code == DW1000_RX_ERROR_NONE && rx_info.len == sizeof(msg) && msg.seq == 2) {
                t6 = rx_info.timestamp;
                uint64_t treply1 = t3-t1;
                uint64_t tround1 = msg.tround1;
                uint64_t treply2 = msg.treply2;
                uint64_t tround2 = t6-t3;
                uint64_t tprop = (tround1*tround2-treply1*treply2)/(tround1+tround2+treply1+treply2);
                float dist = TIME_TO_METERS*tprop;
                samples[samples_collected] = dist;
                samples_collected++;
                if (samples_collected >= SAMPLES_SIZE) {
                    float range_mean = 0;
                    float range_sigma = 0;
                    for (uint32_t i=0; i<samples_collected; i++) {
                        range_mean += samples[i];
                    }
                    range_mean /= samples_collected;
                    for (uint32_t i=0; i<samples_collected; i++) {
                        range_sigma += SQ(samples[i]-range_mean);
                    }
                    range_sigma = sqrtf(range_sigma/(samples_collected-1));

                    uavcan_send_debug_msg(LOG_LEVEL_DEBUG, "responder", "mean = %.3fm, sigma = %.3fm", range_mean-153.7f, range_sigma);

                    samples_collected = 0;
                }
            }
        }

        chThdSleep(US2ST(10));
    }
}

static void initiator_run(void) {
    struct ds_twr_msg_s msg;
    struct dw1000_rx_frame_info_s rx_info;
    uint64_t t0, t4, t5;
    while (true) {
        memset(&msg, 0, sizeof(msg));
        dw1000_disable_transceiver(&uwb_instance);
        dw1000_transmit(&uwb_instance, sizeof(msg), &msg, true);
        chThdSleep(twait);
        rx_info = dw1000_receive(&uwb_instance, sizeof(msg), &msg);
        if (rx_info.err_code == DW1000_RX_ERROR_NONE && rx_info.len == sizeof(msg) && msg.seq == 1) {
            t0 = dw1000_get_tx_stamp(&uwb_instance);
            t4 = rx_info.timestamp;
            t5 = (rx_info.timestamp + TREPLY) & ~0x1ffULL;
            msg.tround1 = t4-t0;
            msg.treply2 = t5-t4;
            msg.seq++;
            dw1000_disable_transceiver(&uwb_instance);
            dw1000_scheduled_transmit(&uwb_instance, rx_info.timestamp+TREPLY, sizeof(msg), &msg, true);
        }

        chThdSleep(MS2ST(25));
    }
}

int main(void) {
    dw1000_init(&uwb_instance, 3, BOARD_PAL_LINE_SPI3_UWB_CS, BOARD_PAL_LINE_UWB_NRST, 0);

    if (device_is_initiator) {
        initiator_run();
    } else {
        responder_run();
    }

    chThdSleep(TIME_INFINITE);

    return 0;
}
