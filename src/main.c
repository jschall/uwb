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
#include <string.h>

PARAM_DEFINE_BOOL_PARAM_STATIC(device_is_transmitter, "tx", true)

static struct dw1000_instance_s uwb_instance;
static struct worker_thread_timer_task_s interval_task;

#define NUM_SAMPLES 200
static struct {
    float rssi;
    float fp_rssi;
    float std_noise;
} samples[NUM_SAMPLES];
static uint8_t samples_collected;

static void rx_task_func(struct worker_thread_timer_task_s* task) {
    (void)task;
    uint8_t buf[128];
    struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(buf), &buf);

    if (rx_info.err_code == DW1000_RX_ERROR_NONE && !memcmp(buf, "foo", strlen("foo"))) {
        if (rx_info.cir_pwr == 0) {
            dw1000_disable_transceiver(&uwb_instance);
            dw1000_rx_enable(&uwb_instance);
            return;
        }
        samples[samples_collected].rssi = rx_info.rssi_est;
        samples[samples_collected].fp_rssi = rx_info.fp_rssi_est;
        samples[samples_collected].std_noise = rx_info.std_noise;
        samples_collected++;
        if (samples_collected >= NUM_SAMPLES) {
            samples_collected = 0;
            
            float rssi_avg = 0;
            float fp_rssi_avg = 0;
            float std_noise = 0;
            for (uint8_t i=0; i<NUM_SAMPLES; i++) {
                rssi_avg += samples[i].rssi;
                fp_rssi_avg += samples[i].fp_rssi;
                std_noise += samples[i].std_noise;
            }
            rssi_avg /= NUM_SAMPLES;
            fp_rssi_avg /= NUM_SAMPLES;
            std_noise /= NUM_SAMPLES;
            
            uavcan_send_debug_msg(LOG_LEVEL_DEBUG, "", "rssi=%.2f fp_rssi=%.2f diff=%.2f noise=%.2f", rssi_avg, fp_rssi_avg, rssi_avg-fp_rssi_avg, std_noise);
        }
    }
}

int main(void) {
    dw1000_init(&uwb_instance, 3, BOARD_PAL_LINE_SPI3_UWB_CS, BOARD_PAL_LINE_UWB_NRST, 0);

    if (device_is_transmitter) {
        while (true) {
            dw1000_transmit(&uwb_instance, strlen("foo"), "foo", false);
            chThdSleep(MS2ST(10));
        }
    } else {
        dw1000_rx_enable(&uwb_instance);
        worker_thread_add_timer_task(&lpwork_thread, &interval_task, rx_task_func, NULL, MS2ST(1), true);
    }

    chThdSleep(TIME_INFINITE);

    return 0;
}
