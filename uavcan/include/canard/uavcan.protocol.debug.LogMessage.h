#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.protocol.debug.LogLevel.h>

#define UAVCAN_PROTOCOL_DEBUG_LOGMESSAGE_MAX_PACK_SIZE 123
#define UAVCAN_PROTOCOL_DEBUG_LOGMESSAGE_DT_SIG 0xD654A48E0C049D75
#define UAVCAN_PROTOCOL_DEBUG_LOGMESSAGE_DT_ID 16383

struct uavcan_protocol_debug_LogMessage_s {
    struct uavcan_protocol_debug_LogLevel_s level;
    struct {uint8_t source_len; uint8_t source[31];};
    struct {uint8_t text_len; uint8_t text[90];};
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_debug_LogMessage_descriptor;

uint32_t encode_uavcan_protocol_debug_LogMessage(struct uavcan_protocol_debug_LogMessage_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_debug_LogMessage(const CanardRxTransfer* transfer, struct uavcan_protocol_debug_LogMessage_s* msg);
void _encode_uavcan_protocol_debug_LogMessage(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_debug_LogMessage_s* msg, bool tao);
void _decode_uavcan_protocol_debug_LogMessage(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_debug_LogMessage_s* msg, bool tao);
