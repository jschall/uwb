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

static void rx_task_func(struct worker_thread_timer_task_s* task) {
    (void)task;
    uint8_t buf[128];
    struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(buf), &buf);

    if (rx_info.err_code == DW1000_RX_ERROR_NONE && !memcmp(buf, "foo", strlen("foo"))) {
//         dw1000_disable_transceiver(&uwb_instance);
//         dw1000_rx_enable(&uwb_instance);
        uavcan_send_debug_msg(LOG_LEVEL_DEBUG, "", "");
        uavcan_send_debug_msg(LOG_LEVEL_DEBUG, "", "std_noise=%u", rx_info.std_noise);
        uavcan_send_debug_msg(LOG_LEVEL_DEBUG, "", "fp_ampl1=%u, fp_ampl2=%u, fp_ampl3=%u", rx_info.fp_ampl1, rx_info.fp_ampl2, rx_info.fp_ampl3);
        uavcan_send_debug_msg(LOG_LEVEL_DEBUG, "", "cir_pwr=%u, rxpacc_corrected=%u", rx_info.cir_pwr, rx_info.rxpacc_corrected);
        uavcan_send_debug_msg(LOG_LEVEL_DEBUG, "", "rssi_est=%f", rx_info.rssi_est);
        uavcan_send_debug_msg(LOG_LEVEL_DEBUG, "", "fp_rssi_est=%f", rx_info.fp_rssi_est);
    }
}

int main(void) {
    dw1000_init(&uwb_instance, 3, BOARD_PAL_LINE_SPI3_UWB_CS, BOARD_PAL_LINE_UWB_NRST, 0);

    if (device_is_transmitter) {
        while (true) {
            dw1000_transmit(&uwb_instance, strlen("foo"), "foo", false);
            chThdSleep(S2ST(1));
        }
    } else {
        dw1000_rx_enable(&uwb_instance);
        worker_thread_add_timer_task(&lpwork_thread, &interval_task, rx_task_func, NULL, MS2ST(10), true);
    }

    chThdSleep(TIME_INFINITE);

    return 0;
}
