#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.protocol.DataTypeKind.h>

#define UAVCAN_PROTOCOL_GETDATATYPEINFO_RES_MAX_PACK_SIZE 93
#define UAVCAN_PROTOCOL_GETDATATYPEINFO_RES_DT_SIG 0x1B283338A7BED2D8
#define UAVCAN_PROTOCOL_GETDATATYPEINFO_RES_DT_ID 2

#define UAVCAN_PROTOCOL_GETDATATYPEINFO_RES_FLAG_KNOWN 1
#define UAVCAN_PROTOCOL_GETDATATYPEINFO_RES_FLAG_SUBSCRIBED 2
#define UAVCAN_PROTOCOL_GETDATATYPEINFO_RES_FLAG_PUBLISHING 4
#define UAVCAN_PROTOCOL_GETDATATYPEINFO_RES_FLAG_SERVING 8

struct uavcan_protocol_GetDataTypeInfo_res_s {
    uint64_t signature;
    uint16_t id;
    struct uavcan_protocol_DataTypeKind_s kind;
    uint8_t flags;
    struct {uint8_t name_len; uint8_t name[80];};
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_GetDataTypeInfo_res_descriptor;

uint32_t encode_uavcan_protocol_GetDataTypeInfo_res(struct uavcan_protocol_GetDataTypeInfo_res_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_GetDataTypeInfo_res(const CanardRxTransfer* transfer, struct uavcan_protocol_GetDataTypeInfo_res_s* msg);
void _encode_uavcan_protocol_GetDataTypeInfo_res(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_GetDataTypeInfo_res_s* msg, bool tao);
void _decode_uavcan_protocol_GetDataTypeInfo_res(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_GetDataTypeInfo_res_s* msg, bool tao);
