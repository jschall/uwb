#include "uavcan_node.h"
#include "app_descriptor.h"
#include <common/can.h>
#include <common/uavcan.h>
#include <common/shared_boot_msg.h>
#include <common/timing.h>
#include <common/helpers.h>
#include <common/param.h>
#include <string.h>

#include <ch.h>

#define CANBUS_AUTOBAUD_SWITCH_INTERVAL_US 1000000
#define CANBUS_AUTOBAUD_TIMEOUT_US 10000000

static enum shared_msg_t shared_msgid;
static union shared_msg_payload_u shared_msg;
static bool shared_msg_valid;

static void begin_canbus_autobaud(void);
static void update_canbus_autobaud(void);

static THD_WORKING_AREA(uavcanNodeThread_wa, 2048);
static THD_FUNCTION(UavcanNodeThread, arg) {
    (void)arg;

    shared_msg_valid = shared_msg_check_and_retreive(&shared_msgid, &shared_msg);
    shared_msg_clear();

    param_init();

    begin_canbus_autobaud();
    while(true) {
        update_canbus_autobaud();
        uavcan_acquire();
        uavcan_update();
        uavcan_release();
        chThdSleepMicroseconds(100);
    }
}

void uavcan_node_init(void) {
    chThdCreateStatic(uavcanNodeThread_wa, sizeof(uavcanNodeThread_wa), NORMALPRIO, UavcanNodeThread, NULL);
}

static void uavcan_node_send_param_getset_response(struct uavcan_transfer_info_s* transfer_info, uint16_t param_idx) {
    struct uavcan_param_getset_response_s response;
    param_make_uavcan_getset_response(param_idx, &response);
    uavcan_acquire();
    uavcan_send_param_getset_response(transfer_info, &response);
    uavcan_release();
}

static void uavcan_node_param_getset_request_handler(struct uavcan_transfer_info_s transfer_info, struct uavcan_param_getset_request_s* request) {
    param_acquire();

    int16_t param_idx = request->index;
    if (request->name_len > 0) {
        // If the name field is not empty, we are to prefer it over the param index field
        param_idx = param_get_index_by_name(request->name_len, (char*)request->name);
    }

    if (param_idx >= 0 && param_idx < param_get_num_params_registered()) {
        // Param exists
        switch(request->value.type) {
            case UAVCAN_PARAM_VALUE_TYPE_EMPTY: {
                // get request
                uavcan_node_send_param_getset_response(&transfer_info, param_idx);
                break;
            }
            case UAVCAN_PARAM_VALUE_TYPE_INT64: {
                // set request: int64
                param_set_by_index_integer(param_idx, request->value.integer_value);
                uavcan_node_send_param_getset_response(&transfer_info, param_idx);
                break;
            }
            case UAVCAN_PARAM_VALUE_TYPE_FLOAT32: {
                // set request: float
                param_set_by_index_float32(param_idx, request->value.real_value);
                uavcan_node_send_param_getset_response(&transfer_info, param_idx);
                break;
            }
            case UAVCAN_PARAM_VALUE_TYPE_BOOL: {
                // set request: bool
                param_set_by_index_bool(param_idx, request->value.boolean_value);
                uavcan_node_send_param_getset_response(&transfer_info, param_idx);
                break;
            }
            case UAVCAN_PARAM_VALUE_TYPE_STRING: {
                // set request: string
                param_set_by_index_string(param_idx, request->value.string_value_len, (char*)request->value.string_value);
                uavcan_node_send_param_getset_response(&transfer_info, param_idx);
                break;
            }
        }
    }

    param_release();
}

static void uavcan_node_send_param_executeopcode_response(struct uavcan_transfer_info_s* transfer_info, bool ok) {
    struct uavcan_param_executeopcode_response_s response = {0,ok};
    uavcan_acquire();
    uavcan_send_param_executeopcode_response(transfer_info, &response);
    uavcan_release();
}

static void uavcan_node_param_executeopcode_request_handler(struct uavcan_transfer_info_s transfer_info, struct uavcan_param_executeopcode_request_s* request) {
    param_acquire();
    bool success = false;
    switch (request->opcode) {
        case UAVCAN_PARAM_EXECUTEOPCODE_OPCODE_SAVE:
            success = param_store_all();
            break;
        case UAVCAN_PARAM_EXECUTEOPCODE_OPCODE_ERASE:
            success = param_erase();
            break;
    }
    param_release();

    uavcan_node_send_param_executeopcode_response(&transfer_info, success);
}

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
    uavcan_acquire();
    uavcan_init();

    set_uavcan_node_info();
    uavcan_set_uavcan_ready_cb(uavcan_ready_handler);
    uavcan_set_restart_cb(restart_request_handler);
    uavcan_set_file_beginfirmwareupdate_cb(file_beginfirmwareupdate_handler);
    uavcan_set_param_getset_request_cb(uavcan_node_param_getset_request_handler);
    uavcan_set_param_executeopcode_request_cb(uavcan_node_param_executeopcode_request_handler);
    uavcan_set_node_mode(UAVCAN_MODE_OPERATIONAL);

    if (shared_msg_valid && shared_msg.canbus_info.local_node_id > 0 && shared_msg.canbus_info.local_node_id <= 127) {
        uavcan_set_node_id(shared_msg.canbus_info.local_node_id);
    } else if (shared_get_parameters(&shared_app_descriptor)->canbus_local_node_id > 0 && shared_get_parameters(&shared_app_descriptor)->canbus_local_node_id <= 127) {
        uavcan_set_node_id(shared_msg.canbus_info.local_node_id);
    }
    uavcan_release();
}
