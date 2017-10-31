#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.protocol.file.Path.h>

#define UAVCAN_PROTOCOL_FILE_BEGINFIRMWAREUPDATE_REQ_MAX_PACK_SIZE 202
#define UAVCAN_PROTOCOL_FILE_BEGINFIRMWAREUPDATE_REQ_DT_SIG 0xB7D725DF72724126
#define UAVCAN_PROTOCOL_FILE_BEGINFIRMWAREUPDATE_REQ_DT_ID 40

struct uavcan_protocol_file_BeginFirmwareUpdate_req_s {
    uint8_t source_node_id;
    struct uavcan_protocol_file_Path_s image_file_remote_path;
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_file_BeginFirmwareUpdate_req_descriptor;

uint32_t encode_uavcan_protocol_file_BeginFirmwareUpdate_req(struct uavcan_protocol_file_BeginFirmwareUpdate_req_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_file_BeginFirmwareUpdate_req(const CanardRxTransfer* transfer, struct uavcan_protocol_file_BeginFirmwareUpdate_req_s* msg);
void _encode_uavcan_protocol_file_BeginFirmwareUpdate_req(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_file_BeginFirmwareUpdate_req_s* msg, bool tao);
void _decode_uavcan_protocol_file_BeginFirmwareUpdate_req(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_file_BeginFirmwareUpdate_req_s* msg, bool tao);
