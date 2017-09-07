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
// #include <common/profiLED_gen.h>
//
// #define NUM_PROFILEDS 4
//
// static uint8_t txbuf[NUM_PROFILEDS*4+7];
//
// static const SPIConfig ledSPIConfig =
// {
//     NULL,
//     0,
//     0,
//     0,
//     SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
// };
//
// static void setLEDColor(uint32_t color) {
//     struct profiLED_gen_color_s colors[NUM_PROFILEDS];
//     uint8_t i;
//     for (i=0; i<NUM_PROFILEDS; i++) {
//         profiLED_gen_make_brg_color_hex(color, &colors[i]);
//     }
//
//     volatile uint32_t buf_len = profiLED_gen_write_buf(NUM_PROFILEDS, colors, txbuf, sizeof(txbuf));
//
//     spiAcquireBus(&SPID3);
//     palSetPad(GPIOA, 15);
//     spiStart(&SPID3, &ledSPIConfig);
//
//     spiSend(&SPID3, buf_len, txbuf);
//
//     palClearPad(GPIOA, 15);
//     spiReleaseBus(&SPID3);
// }
//
// /*
//  * Green LED blinker thread, times are in milliseconds.
//  */
// static THD_WORKING_AREA(waBlinkerThread, 128);
// static THD_FUNCTION(BlinkerThread, arg) {
//     (void)arg;
//     chRegSetThreadName("blinker");
//
//     while (true) {
//         setLEDColor(0xff0000);
//         palSetPad(GPIOA, 1);
//         chThdSleepMilliseconds(500);
//         setLEDColor(0x0000ff);
//         palClearPad(GPIOA, 1);
//         chThdSleepMilliseconds(500);
//     }
// }

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
//     spiStart(&SPID3, &ledSPIConfig);

    palSetPadMode(GPIOB, 3, PAL_MODE_ALTERNATE(6) | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_PUPDR_PULLDOWN);       /* New SCK.     */
    palSetPadMode(GPIOB, 4, PAL_MODE_ALTERNATE(6) | PAL_STM32_OSPEED_HIGHEST);       /* New MISO.    */
    palSetPadMode(GPIOB, 5, PAL_MODE_ALTERNATE(6) | PAL_STM32_OSPEED_HIGHEST);       /* New MOSI.    */
    palSetPadMode(GPIOA, 15, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);       /* New CS.      */
    palClearPad(GPIOA, 15);
    palSetPad(GPIOA, 1);
    /*
     * Creates the blinker thread.
     */
//     chThdCreateStatic(waBlinkerThread, sizeof(waBlinkerThread), NORMALPRIO, BlinkerThread, NULL);

    struct profiLED_instance_s profileds;
    profiLED_init(&profileds, &SPID3, PAL_LINE(GPIOA, 15), true, 4);

    /*
     * Normal main() thread activity, in this demo it does nothing except
     * sleeping in a loop and check the button state.
     */
    while (true) {
        for (uint8_t i=0; i<profileds.num_leds; i++) {
            profiLED_set_color_hex(&profileds, i, 0xff0000);
        }
        profiLED_update(&profileds);
        chThdSleepMilliseconds(500);

        for (uint8_t i=0; i<profileds.num_leds; i++) {
            profiLED_set_color_hex(&profileds, i, 0x0000ff);
        }
        profiLED_update(&profileds);
        chThdSleepMilliseconds(500);
    }
    return 0;
}
