#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.protocol.file.Path.h>

#define UAVCAN_PROTOCOL_FILE_WRITE_REQ_MAX_PACK_SIZE 399
#define UAVCAN_PROTOCOL_FILE_WRITE_REQ_DT_SIG 0x515AA1DC77E58429
#define UAVCAN_PROTOCOL_FILE_WRITE_REQ_DT_ID 49

struct uavcan_protocol_file_Write_req_s {
    uint64_t offset;
    struct uavcan_protocol_file_Path_s path;
    struct {uint8_t data_len; uint8_t data[192];};
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_file_Write_req_descriptor;

uint32_t encode_uavcan_protocol_file_Write_req(struct uavcan_protocol_file_Write_req_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_file_Write_req(const CanardRxTransfer* transfer, struct uavcan_protocol_file_Write_req_s* msg);
void _encode_uavcan_protocol_file_Write_req(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_file_Write_req_s* msg, bool tao);
void _decode_uavcan_protocol_file_Write_req(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_file_Write_req_s* msg, bool tao);
