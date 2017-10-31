#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_PANIC_MAX_PACK_SIZE 8
#define UAVCAN_PROTOCOL_PANIC_DT_SIG 0x8B79B4101811C1D7
#define UAVCAN_PROTOCOL_PANIC_DT_ID 5

#define UAVCAN_PROTOCOL_PANIC_MIN_MESSAGES 3
#define UAVCAN_PROTOCOL_PANIC_MAX_INTERVAL_MS 500

struct uavcan_protocol_Panic_s {
    struct {uint8_t reason_text_len; uint8_t reason_text[7];};
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_Panic_descriptor;

uint32_t encode_uavcan_protocol_Panic(struct uavcan_protocol_Panic_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_Panic(const CanardRxTransfer* transfer, struct uavcan_protocol_Panic_s* msg);
void _encode_uavcan_protocol_Panic(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_Panic_s* msg, bool tao);
void _decode_uavcan_protocol_Panic(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_Panic_s* msg, bool tao);
