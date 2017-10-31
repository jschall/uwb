#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.protocol.file.Error.h>

#define UAVCAN_PROTOCOL_FILE_WRITE_RES_MAX_PACK_SIZE 2
#define UAVCAN_PROTOCOL_FILE_WRITE_RES_DT_SIG 0x515AA1DC77E58429
#define UAVCAN_PROTOCOL_FILE_WRITE_RES_DT_ID 49

struct uavcan_protocol_file_Write_res_s {
    struct uavcan_protocol_file_Error_s error;
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_file_Write_res_descriptor;

uint32_t encode_uavcan_protocol_file_Write_res(struct uavcan_protocol_file_Write_res_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_file_Write_res(const CanardRxTransfer* transfer, struct uavcan_protocol_file_Write_res_s* msg);
void _encode_uavcan_protocol_file_Write_res(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_file_Write_res_s* msg, bool tao);
void _decode_uavcan_protocol_file_Write_res(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_file_Write_res_s* msg, bool tao);
