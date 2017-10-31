#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.protocol.file.EntryType.h>
#include <canard/uavcan.protocol.file.Error.h>

#define UAVCAN_PROTOCOL_FILE_GETINFO_RES_MAX_PACK_SIZE 8
#define UAVCAN_PROTOCOL_FILE_GETINFO_RES_DT_SIG 0x5004891EE8A27531
#define UAVCAN_PROTOCOL_FILE_GETINFO_RES_DT_ID 45

struct uavcan_protocol_file_GetInfo_res_s {
    uint64_t size;
    struct uavcan_protocol_file_Error_s error;
    struct uavcan_protocol_file_EntryType_s entry_type;
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_file_GetInfo_res_descriptor;

uint32_t encode_uavcan_protocol_file_GetInfo_res(struct uavcan_protocol_file_GetInfo_res_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_file_GetInfo_res(const CanardRxTransfer* transfer, struct uavcan_protocol_file_GetInfo_res_s* msg);
void _encode_uavcan_protocol_file_GetInfo_res(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_file_GetInfo_res_s* msg, bool tao);
void _decode_uavcan_protocol_file_GetInfo_res(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_file_GetInfo_res_s* msg, bool tao);
