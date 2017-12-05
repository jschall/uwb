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
#include <modules/timing/timing.h>
#include <common/helpers.h>
#include <modules/driver_dw1000/dw1000.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <modules/param/param.h>
#include <modules/uavcan/uavcan.h>
#include "tdma.h"
#include <modules/worker_thread/worker_thread.h>


/*
PARAM_DEFINE_FLOAT32_PARAM_STATIC(param_a, "a", 4, 3, 5)
PARAM_DEFINE_INT64_PARAM_STATIC(param_b, "b", 7, 7, 7)
PARAM_DEFINE_INT32_PARAM_STATIC(param_c, "c", 7, 7, 7)
PARAM_DEFINE_INT16_PARAM_STATIC(param_d, "d", 7, 7, 7)
PARAM_DEFINE_INT8_PARAM_STATIC(param_e, "e", 7, 7, 7)
PARAM_DEFINE_UINT32_PARAM_STATIC(param_f, "f", 7, 7, 7)
PARAM_DEFINE_UINT16_PARAM_STATIC(param_g, "g", 7, 7, 7)
PARAM_DEFINE_STRING_PARAM_STATIC(param_j, "j", "blah", 128)
PARAM_DEFINE_BOOL_PARAM_STATIC(param_i, "i", true)
*/
PARAM_DEFINE_UINT8_PARAM_STATIC(param_tdma_unit_type, "TDMA_UNIT_TYPE", ANCHOR, 0, 1)
PARAM_DEFINE_UINT8_PARAM_STATIC(param_tdma_tx_type, "TDMA_TX_TYPE", TDMA_SUPERVISOR, 0, 2)
PARAM_DEFINE_UINT8_PARAM_STATIC(param_tdma_tbody_id, "TDMA_TBODY_ID", 0xFF, 0, 0xFF)
PARAM_DEFINE_UINT8_PARAM_STATIC(param_tdma_body_id, "TDMA_BODY_ID", 0xFF, 0, 0xFF)
PARAM_DEFINE_UINT32_PARAM_STATIC(param_ant_delay, "ANTENNA_DELAY", 0, 0, UINT32_MAX)
PARAM_DEFINE_BOOL_PARAM_STATIC(param_ant_delay_cal, "ANT_DELAY_CAL", false)
PARAM_DEFINE_FLOAT32_PARAM_STATIC(param_anchor_pos_x, "ANCHOR_POS_X", 0.0f, -500.0f, 500.0f)
PARAM_DEFINE_FLOAT32_PARAM_STATIC(param_anchor_pos_y, "ANCHOR_POS_Y", 0.0f, -500.0f, 500.0f)
PARAM_DEFINE_FLOAT32_PARAM_STATIC(param_anchor_pos_z, "ANCHOR_POS_Z", 0.0f, -500.0f, 500.0f)

struct worker_thread_s uwb_listener_thread;

RUN_ON(WORKER_THREADS_START) {
    worker_thread_init(&uwb_listener_thread,"uwb_Listener", 1024, LOWPRIO);      //All timer repeatitive task go in here
}


RUN_BEFORE(INIT_END) {
    uint8_t unique_id[12];
    board_get_unique_id(unique_id, sizeof(unique_id));
    struct tx_spec_s tx_spec_init;
    tx_spec_init.type = param_tdma_unit_type;
    tx_spec_init.node_id = unique_id[10];
    tx_spec_init.body_id = param_tdma_body_id;
    tx_spec_init.ant_delay_cal_status = param_ant_delay_cal;
    tx_spec_init.ant_delay = param_ant_delay;
    tx_spec_init.pkt_cnt = 0;
    tx_spec_init.data_slot_id = 255;
    tx_spec_init.body_pos[0] = param_anchor_pos_x;
    tx_spec_init.body_pos[1] = param_anchor_pos_y;
    tx_spec_init.body_pos[2] = param_anchor_pos_z;
    

    if (param_tdma_tx_type == TDMA_SUPERVISOR) { // we are tdma supervisor
        tdma_supervisor_init(tx_spec_init, param_tdma_tbody_id, &lpwork_thread, &uwb_listener_thread);
    } else if (param_tdma_tx_type == TDMA_SUBORDINATE) {
        tdma_subordinate_init(tx_spec_init, &lpwork_thread, &uwb_listener_thread);
    } else {
        tdma_sniffer_run();
    }
}

int main() {
    chSysLock();
    chThdSleepS(TIME_INFINITE);
    chSysUnlock();
    return 0;
}