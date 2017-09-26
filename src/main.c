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
#include "app_descriptor.h"
#include <common/profiLED.h>
#include <common/timing.h>
#include <common/helpers.h>
#include <common/can.h>
#include <common/uavcan.h>
#include <common/shared_boot_msg.h>
#include "dw1000.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

#define CANBUS_AUTOBAUD_SWITCH_INTERVAL_US 1000000
#define CANBUS_AUTOBAUD_TIMEOUT_US 10000000

static enum shared_msg_t shared_msgid;
static union shared_msg_payload_u shared_msg;
static bool shared_msg_valid;

static void fill_shared_canbus_info(struct shared_canbus_info_s* canbus_info) {
    canbus_info->local_node_id = uavcan_get_node_id();

    if (canbus_get_confirmed_baudrate()) {
        canbus_info->baudrate = canbus_get_confirmed_baudrate();
    } else if (shared_msg_valid && canbus_baudrate_valid(shared_msg.canbus_info.baudrate)) {
        canbus_info->baudrate = canbus_info->baudrate;
    } else {
        canbus_info->baudrate = 0;
    }
}

static void set_uavcan_node_info(void) {
    struct uavcan_node_info_s uavcan_node_info;
    memset(&uavcan_node_info, 0, sizeof(uavcan_node_info));

    if (shared_msg_valid && shared_msgid == SHARED_MSG_BOOT_INFO && shared_msg.boot_info_msg.hw_info) {
        uavcan_node_info.hw_name = shared_msg.boot_info_msg.hw_info->hw_name;
        uavcan_node_info.hw_major_version = shared_msg.boot_info_msg.hw_info->hw_major_version;
        uavcan_node_info.hw_minor_version = shared_msg.boot_info_msg.hw_info->hw_minor_version;
    }

    uavcan_node_info.sw_major_version = shared_app_descriptor.major_version;
    uavcan_node_info.sw_minor_version = shared_app_descriptor.minor_version;
    uavcan_node_info.sw_vcs_commit_available = true;
    uavcan_node_info.sw_vcs_commit = shared_app_descriptor.vcs_commit;
    uavcan_node_info.sw_image_crc_available = true;
    uavcan_node_info.sw_image_crc = shared_app_descriptor.image_crc;

    uavcan_set_node_info(uavcan_node_info);
}

static void restart_request_handler(struct uavcan_transfer_info_s transfer_info, uint64_t magic) {
    if (magic == 0xACCE551B1E) {
        uavcan_send_restart_response(&transfer_info, true);
        uint32_t tbegin_us = micros();
        while (micros()-tbegin_us < 1000) {
            uavcan_update();
        }

        union shared_msg_payload_u msg;
        fill_shared_canbus_info(&msg.canbus_info);
        shared_msg_finalize_and_write(SHARED_MSG_CANBUS_INFO, &msg);
        NVIC_SystemReset();
    } else {
        uavcan_send_restart_response(&transfer_info, false);
    }
}

static void file_beginfirmwareupdate_handler(struct uavcan_transfer_info_s transfer_info, uint8_t source_node_id, const char* path) {
    if (source_node_id == 0) {
        source_node_id = transfer_info.remote_node_id;
    }

    union shared_msg_payload_u msg;
    fill_shared_canbus_info(&msg.firmwareupdate_msg.canbus_info);

    if (source_node_id > 0 && source_node_id <= 127) {
        msg.firmwareupdate_msg.source_node_id = source_node_id;
    } else {
        msg.firmwareupdate_msg.source_node_id = 0;
    }

    strcpy(msg.firmwareupdate_msg.path, path);

    shared_msg_finalize_and_write(SHARED_MSG_FIRMWAREUPDATE, &msg);

    uavcan_send_file_beginfirmwareupdate_response(&transfer_info, UAVCAN_BEGINFIRMWAREUPDATE_ERROR_OK, "");

    uint32_t tbegin_us = micros();
    while (micros()-tbegin_us < 100000) {
        uavcan_update();
    }

    NVIC_SystemReset();
}

static void uavcan_ready_handler(void) {}

static bool canbus_autobaud_running;
static struct canbus_autobaud_state_s autobaud_state;
static void on_canbus_baudrate_confirmed(uint32_t canbus_baud);

static void begin_canbus_autobaud(void) {
    uint32_t canbus_baud;
    if (shared_msg_valid && canbus_baudrate_valid(shared_msg.canbus_info.baudrate)) {
        canbus_baud = shared_msg.canbus_info.baudrate;
    } else if (canbus_baudrate_valid(shared_get_parameters(&shared_app_descriptor)->canbus_baudrate)) {
        canbus_baud = shared_get_parameters(&shared_app_descriptor)->canbus_baudrate;
    } else {
        canbus_baud = 1000000;
    }

    bool canbus_autobaud_enable;
    if (shared_msg_valid && canbus_baudrate_valid(shared_msg.canbus_info.baudrate)) {
        canbus_autobaud_enable = false;
    } else if (shared_get_parameters(&shared_app_descriptor)->canbus_disable_auto_baud) {
        canbus_autobaud_enable = false;
    } else {
        canbus_autobaud_enable = true;
    }

    if (canbus_autobaud_enable) {
        canbus_autobaud_start(&autobaud_state, canbus_baud, CANBUS_AUTOBAUD_SWITCH_INTERVAL_US);
        canbus_autobaud_running = true;
    } else {
        on_canbus_baudrate_confirmed(canbus_baud);
    }
}

static void update_canbus_autobaud(void) {
    if (!canbus_autobaud_running) {
        return;
    }

    uint32_t canbus_baud = canbus_autobaud_update(&autobaud_state);
    if (autobaud_state.success) {
        on_canbus_baudrate_confirmed(canbus_baud);
        canbus_autobaud_running = false;
    }

}

static void on_canbus_baudrate_confirmed(uint32_t canbus_baud) {
    canbus_init(canbus_baud, false);
    uavcan_init();

    set_uavcan_node_info();
    uavcan_set_uavcan_ready_cb(uavcan_ready_handler);
    uavcan_set_restart_cb(restart_request_handler);
    uavcan_set_file_beginfirmwareupdate_cb(file_beginfirmwareupdate_handler);
    uavcan_set_node_mode(UAVCAN_MODE_OPERATIONAL);

    if (shared_msg_valid && shared_msg.canbus_info.local_node_id > 0 && shared_msg.canbus_info.local_node_id <= 127) {
        uavcan_set_node_id(shared_msg.canbus_info.local_node_id);
    } else if (shared_get_parameters(&shared_app_descriptor)->canbus_local_node_id > 0 && shared_get_parameters(&shared_app_descriptor)->canbus_local_node_id <= 127) {
        uavcan_set_node_id(shared_msg.canbus_info.local_node_id);
    }
}

static void ss_twr_initiator_run(void) {
    struct dw1000_instance_s uwb_instance;
    dw1000_init(&uwb_instance, 3, BOARD_PAL_LINE_SPI3_UWB_CS, BOARD_PAL_LINE_UWB_NRST);
    uint32_t tprev_us = 0;
    uint8_t i=0;
    while (true) {
        update_canbus_autobaud();
        uavcan_update();
        uint32_t tnow_us = micros();
        if (tnow_us-tprev_us > 200000) {
            tprev_us = tnow_us;
            char msg[50];
            int n = sprintf(msg, "message %u", i);
            dw1000_transmit(&uwb_instance, n, msg, true);
//             uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "init tx", msg);

            while(true) {
                if (micros()-tnow_us > 5000) {
                    uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "init rx t/o", "");
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
                    uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "init rx", msg);
                    break;
                }
            }

            i++;
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
        update_canbus_autobaud();
        uavcan_update();
        char rxbuf[50];
        struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(rxbuf)-1, rxbuf);
        if (rx_info.err_code == DW1000_RX_ERROR_NONE) {
            dw1000_disable_transceiver(&uwb_instance);
            uint64_t scheduled_time = (rx_info.timestamp+65536ULL*1000ULL)&0xFFFFFFFFFFFFFE00ULL;
            uint32_t dly = (uint32_t)(scheduled_time-rx_info.timestamp);
            if (dw1000_scheduled_transmit(&uwb_instance, scheduled_time, sizeof(dly), &dly, true)) {
                rxbuf[rx_info.len] = 0;
//                 uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "resp", rxbuf);
            } else {
                uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "resp fail", "");
            }
        }
    }
}

static void receiver_run(void) {
    struct dw1000_instance_s uwb_instance;
    dw1000_init(&uwb_instance, 3, BOARD_PAL_LINE_SPI3_UWB_CS, BOARD_PAL_LINE_UWB_NRST);
    dw1000_rx_enable(&uwb_instance);
    uint32_t tprev_us = 0;
    while (true) {
        update_canbus_autobaud();
        uavcan_update();
        uint32_t tnow_us = micros();
        if (tnow_us-tprev_us > 100000) {
            tprev_us = tnow_us;
            char rxbuf[50];
            struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(rxbuf)-1, rxbuf);
            if (rx_info.err_code == DW1000_RX_ERROR_NONE) {
                rxbuf[rx_info.len] = 0;
                char msg[100];
                sprintf(msg, "%llu %e %s", rx_info.timestamp, ((float)rx_info.rx_ttcko)/((float)rx_info.rx_ttcki), rxbuf);
                uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "rx", msg);
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
        update_canbus_autobaud();
        uavcan_update();
        uint32_t tnow_us = micros();
        if (tnow_us-tprev_us > 200000) {
            tprev_us = tnow_us;
            char msg[50];
            int n = snprintf(msg, sizeof(msg), "message %u", i);
            if (n > 0 && n<sizeof(msg)) {
                dw1000_transmit(&uwb_instance, n, msg, false);
                uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "tx", msg);
            }
            i++;
        }
        chThdSleepMicroseconds(1000);
    }
}

int main(void) {
    halInit();
    chSysInit();
    timing_init();

    shared_msg_valid = shared_msg_check_and_retreive(&shared_msgid, &shared_msg);
    shared_msg_clear();

    begin_canbus_autobaud();

    uint8_t unique_id[12];
    board_get_unique_id(unique_id, sizeof(unique_id));

    if (unique_id[10] == 0x1F) {
        ss_twr_initiator_run();
    } else {
        ss_twr_responder_run();
    }

    return 0;
}
