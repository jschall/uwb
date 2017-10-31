#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_CANIFACESTATS_MAX_PACK_SIZE 18
#define UAVCAN_PROTOCOL_CANIFACESTATS_DT_SIG 0x13B106F0C44CA350

struct uavcan_protocol_CANIfaceStats_s {
    uint64_t frames_tx;
    uint64_t frames_rx;
    uint64_t errors;
};


uint32_t encode_uavcan_protocol_CANIfaceStats(struct uavcan_protocol_CANIfaceStats_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_CANIfaceStats(const CanardRxTransfer* transfer, struct uavcan_protocol_CANIfaceStats_s* msg);
void _encode_uavcan_protocol_CANIfaceStats(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_CANIfaceStats_s* msg, bool tao);
void _decode_uavcan_protocol_CANIfaceStats(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_CANIfaceStats_s* msg, bool tao);
