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
#include <modules/param/param.h>

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
    chThdSleep(TIME_INFINITE);

    return 0;
}
