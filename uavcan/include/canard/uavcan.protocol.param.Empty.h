#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_PARAM_EMPTY_MAX_PACK_SIZE 0
#define UAVCAN_PROTOCOL_PARAM_EMPTY_DT_SIG 0x6C4D0E8EF37361DF

struct uavcan_protocol_param_Empty_s {
};


uint32_t encode_uavcan_protocol_param_Empty(struct uavcan_protocol_param_Empty_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_param_Empty(const CanardRxTransfer* transfer, struct uavcan_protocol_param_Empty_s* msg);
void _encode_uavcan_protocol_param_Empty(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_param_Empty_s* msg, bool tao);
void _decode_uavcan_protocol_param_Empty(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_param_Empty_s* msg, bool tao);
