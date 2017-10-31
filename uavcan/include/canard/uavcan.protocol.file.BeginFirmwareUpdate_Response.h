#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_FILE_BEGINFIRMWAREUPDATE_RES_MAX_PACK_SIZE 129
#define UAVCAN_PROTOCOL_FILE_BEGINFIRMWAREUPDATE_RES_DT_SIG 0xB7D725DF72724126
#define UAVCAN_PROTOCOL_FILE_BEGINFIRMWAREUPDATE_RES_DT_ID 40

#define UAVCAN_PROTOCOL_FILE_BEGINFIRMWAREUPDATE_RES_ERROR_OK 0
#define UAVCAN_PROTOCOL_FILE_BEGINFIRMWAREUPDATE_RES_ERROR_INVALID_MODE 1
#define UAVCAN_PROTOCOL_FILE_BEGINFIRMWAREUPDATE_RES_ERROR_IN_PROGRESS 2
#define UAVCAN_PROTOCOL_FILE_BEGINFIRMWAREUPDATE_RES_ERROR_UNKNOWN 255

struct uavcan_protocol_file_BeginFirmwareUpdate_res_s {
    uint8_t error;
    struct {uint8_t optional_error_message_len; uint8_t optional_error_message[127];};
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_file_BeginFirmwareUpdate_res_descriptor;

uint32_t encode_uavcan_protocol_file_BeginFirmwareUpdate_res(struct uavcan_protocol_file_BeginFirmwareUpdate_res_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_file_BeginFirmwareUpdate_res(const CanardRxTransfer* transfer, struct uavcan_protocol_file_BeginFirmwareUpdate_res_s* msg);
void _encode_uavcan_protocol_file_BeginFirmwareUpdate_res(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_file_BeginFirmwareUpdate_res_s* msg, bool tao);
void _decode_uavcan_protocol_file_BeginFirmwareUpdate_res(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_file_BeginFirmwareUpdate_res_s* msg, bool tao);
