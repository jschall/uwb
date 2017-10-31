#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.protocol.file.Path.h>

#define UAVCAN_PROTOCOL_FILE_DELETE_REQ_MAX_PACK_SIZE 201
#define UAVCAN_PROTOCOL_FILE_DELETE_REQ_DT_SIG 0x78648C99170B47AA
#define UAVCAN_PROTOCOL_FILE_DELETE_REQ_DT_ID 47

struct uavcan_protocol_file_Delete_req_s {
    struct uavcan_protocol_file_Path_s path;
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_file_Delete_req_descriptor;

uint32_t encode_uavcan_protocol_file_Delete_req(struct uavcan_protocol_file_Delete_req_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_file_Delete_req(const CanardRxTransfer* transfer, struct uavcan_protocol_file_Delete_req_s* msg);
void _encode_uavcan_protocol_file_Delete_req(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_file_Delete_req_s* msg, bool tao);
void _decode_uavcan_protocol_file_Delete_req(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_file_Delete_req_s* msg, bool tao);
