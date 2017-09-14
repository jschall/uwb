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
#include <common/profiLED.h>

int main(void) {
    halInit();
    chSysInit();

    struct profiLED_instance_s profileds;
    profiLED_init(&profileds, 3, PAL_LINE(GPIOA, 15), true, 4);

    while (true) {
        for (uint8_t i=0; i<profileds.num_leds; i++) {
            profiLED_set_color_hex(&profileds, i, 0x050000);
        }
        profiLED_update(&profileds);
        palTogglePad(GPIOA, 1);
        chThdSleepMilliseconds(500);


        for (uint8_t i=0; i<profileds.num_leds; i++) {
            profiLED_set_color_hex(&profileds, i, 0x000005);
        }
        palTogglePad(GPIOA, 1);
        profiLED_update(&profileds);
        chThdSleepMilliseconds(500);

        for (uint8_t i=0; i<profileds.num_leds; i++) {
            profiLED_set_color_hex(&profileds, i, 0x000500);
        }
        palTogglePad(GPIOA, 1);
        profiLED_update(&profileds);
        chThdSleepMilliseconds(500);
    }
    return 0;
}
