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
#include "tdma.h"


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
PARAM_DEFINE_UINT8_PARAM_STATIC(tdma_unit_type, "TDMA_UTYPE", 0, 0, 2)

int main(void) {
    halInit();
    chSysInit();
    timing_init();

    uavcan_node_init();


    uint8_t unique_id[12];
    board_get_unique_id(unique_id, sizeof(unique_id));
    if (unique_id[10] == 0x4A) { // we are tdma supervisor
        tdma_init(unique_id[10], TDMA_SUPERVISOR);
        tdma_supervisor_run();
        tdma_unit_type = 1;
    } else if (tdma_unit_type == 0){
        tdma_init(unique_id[10], TDMA_SUBORDINATE);
        tdma_subordinate_run();
    } else {
        tdma_init(unique_id[10], TDMA_SNIFFER);
        tdma_sniffer_run();
    }

    return 0;
}
