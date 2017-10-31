#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_DEBUG_KEYVALUE_MAX_PACK_SIZE 63
#define UAVCAN_PROTOCOL_DEBUG_KEYVALUE_DT_SIG 0xE02F25D6E0C98AE0
#define UAVCAN_PROTOCOL_DEBUG_KEYVALUE_DT_ID 16370

struct uavcan_protocol_debug_KeyValue_s {
    float value;
    struct {uint8_t key_len; uint8_t key[58];};
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_debug_KeyValue_descriptor;

uint32_t encode_uavcan_protocol_debug_KeyValue(struct uavcan_protocol_debug_KeyValue_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_debug_KeyValue(const CanardRxTransfer* transfer, struct uavcan_protocol_debug_KeyValue_s* msg);
void _encode_uavcan_protocol_debug_KeyValue(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_debug_KeyValue_s* msg, bool tao);
void _decode_uavcan_protocol_debug_KeyValue(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_debug_KeyValue_s* msg, bool tao);
