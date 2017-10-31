#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_DATATYPEKIND_MAX_PACK_SIZE 1
#define UAVCAN_PROTOCOL_DATATYPEKIND_DT_SIG 0x9420A73E008E5930

#define UAVCAN_PROTOCOL_DATATYPEKIND_SERVICE 0
#define UAVCAN_PROTOCOL_DATATYPEKIND_MESSAGE 1

struct uavcan_protocol_DataTypeKind_s {
    uint8_t value;
};


uint32_t encode_uavcan_protocol_DataTypeKind(struct uavcan_protocol_DataTypeKind_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_DataTypeKind(const CanardRxTransfer* transfer, struct uavcan_protocol_DataTypeKind_s* msg);
void _encode_uavcan_protocol_DataTypeKind(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_DataTypeKind_s* msg, bool tao);
void _decode_uavcan_protocol_DataTypeKind(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_DataTypeKind_s* msg, bool tao);
