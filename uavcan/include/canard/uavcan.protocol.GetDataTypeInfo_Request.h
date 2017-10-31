#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.protocol.DataTypeKind.h>

#define UAVCAN_PROTOCOL_GETDATATYPEINFO_REQ_MAX_PACK_SIZE 84
#define UAVCAN_PROTOCOL_GETDATATYPEINFO_REQ_DT_SIG 0x1B283338A7BED2D8
#define UAVCAN_PROTOCOL_GETDATATYPEINFO_REQ_DT_ID 2

struct uavcan_protocol_GetDataTypeInfo_req_s {
    uint16_t id;
    struct uavcan_protocol_DataTypeKind_s kind;
    struct {uint8_t name_len; uint8_t name[80];};
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_GetDataTypeInfo_req_descriptor;

uint32_t encode_uavcan_protocol_GetDataTypeInfo_req(struct uavcan_protocol_GetDataTypeInfo_req_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_GetDataTypeInfo_req(const CanardRxTransfer* transfer, struct uavcan_protocol_GetDataTypeInfo_req_s* msg);
void _encode_uavcan_protocol_GetDataTypeInfo_req(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_GetDataTypeInfo_req_s* msg, bool tao);
void _decode_uavcan_protocol_GetDataTypeInfo_req(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_GetDataTypeInfo_req_s* msg, bool tao);
