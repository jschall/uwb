#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.protocol.file.Path.h>

#define UAVCAN_PROTOCOL_FILE_READ_REQ_MAX_PACK_SIZE 206
#define UAVCAN_PROTOCOL_FILE_READ_REQ_DT_SIG 0x8DCDCA939F33F678
#define UAVCAN_PROTOCOL_FILE_READ_REQ_DT_ID 48

struct uavcan_protocol_file_Read_req_s {
    uint64_t offset;
    struct uavcan_protocol_file_Path_s path;
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_file_Read_req_descriptor;

uint32_t encode_uavcan_protocol_file_Read_req(struct uavcan_protocol_file_Read_req_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_file_Read_req(const CanardRxTransfer* transfer, struct uavcan_protocol_file_Read_req_s* msg);
void _encode_uavcan_protocol_file_Read_req(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_file_Read_req_s* msg, bool tao);
void _decode_uavcan_protocol_file_Read_req(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_file_Read_req_s* msg, bool tao);
