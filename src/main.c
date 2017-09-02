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

/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waBlinkerThread, 128);
static THD_FUNCTION(BlinkerThread, arg) {

    (void)arg;
    chRegSetThreadName("blinker");
    while (true) {
        palClearPad(GPIOA, 1);
        chThdSleepMilliseconds(1000);
        palSetPad(GPIOA, 1);
        chThdSleepMilliseconds(1000);
    }
}

static SPIConfig led_spicfg = {NULL, GPIOA, 15, 0, 0};

/*
 * Application entry point.
 */
int main(void) {


    /*
     * System initializations.
     * - HAL initialization, this also initializes the configured device drivers
     *   and performs the board-specific initializations.
     * - Kernel initialization, the main() function becomes a thread and the
     *   RTOS is active.
     */
    halInit();
    chSysInit();
    spiInit();

    /*
     * Creates the blinker thread.
     */
    chThdCreateStatic(waBlinkerThread, sizeof(waBlinkerThread), NORMALPRIO, BlinkerThread, NULL);

    /*
     * Normal main() thread activity, in this demo it does nothing except
     * sleeping in a loop and check the button state.
     */
    while (true) {
// //         palSetPad(GPIOA, 1);
         chThdSleepMilliseconds(1100);
    }
    return 0;
}
