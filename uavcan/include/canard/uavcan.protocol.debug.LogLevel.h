#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_MAX_PACK_SIZE 1
#define UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DT_SIG 0x711BF141AF572346

#define UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG 0
#define UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_INFO 1
#define UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_WARNING 2
#define UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_ERROR 3

struct uavcan_protocol_debug_LogLevel_s {
    uint8_t value;
};


uint32_t encode_uavcan_protocol_debug_LogLevel(struct uavcan_protocol_debug_LogLevel_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_debug_LogLevel(const CanardRxTransfer* transfer, struct uavcan_protocol_debug_LogLevel_s* msg);
void _encode_uavcan_protocol_debug_LogLevel(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_debug_LogLevel_s* msg, bool tao);
void _decode_uavcan_protocol_debug_LogLevel(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_debug_LogLevel_s* msg, bool tao);
