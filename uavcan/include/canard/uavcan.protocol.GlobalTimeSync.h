#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_GLOBALTIMESYNC_MAX_PACK_SIZE 7
#define UAVCAN_PROTOCOL_GLOBALTIMESYNC_DT_SIG 0x20271116A793C2DB
#define UAVCAN_PROTOCOL_GLOBALTIMESYNC_DT_ID 4

#define UAVCAN_PROTOCOL_GLOBALTIMESYNC_MAX_BROADCASTING_PERIOD_MS 1100
#define UAVCAN_PROTOCOL_GLOBALTIMESYNC_MIN_BROADCASTING_PERIOD_MS 40
#define UAVCAN_PROTOCOL_GLOBALTIMESYNC_RECOMMENDED_BROADCASTER_TIMEOUT_MS 2200

struct uavcan_protocol_GlobalTimeSync_s {
    uint64_t previous_transmission_timestamp_usec;
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_GlobalTimeSync_descriptor;

uint32_t encode_uavcan_protocol_GlobalTimeSync(struct uavcan_protocol_GlobalTimeSync_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_GlobalTimeSync(const CanardRxTransfer* transfer, struct uavcan_protocol_GlobalTimeSync_s* msg);
void _encode_uavcan_protocol_GlobalTimeSync(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_GlobalTimeSync_s* msg, bool tao);
void _decode_uavcan_protocol_GlobalTimeSync(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_GlobalTimeSync_s* msg, bool tao);
