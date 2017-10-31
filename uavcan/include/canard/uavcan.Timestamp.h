#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_TIMESTAMP_MAX_PACK_SIZE 7
#define UAVCAN_TIMESTAMP_DT_SIG 0x5BD0B5C81087E0D

#define UAVCAN_TIMESTAMP_UNKNOWN 0

struct uavcan_Timestamp_s {
    uint64_t usec;
};


uint32_t encode_uavcan_Timestamp(struct uavcan_Timestamp_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_Timestamp(const CanardRxTransfer* transfer, struct uavcan_Timestamp_s* msg);
void _encode_uavcan_Timestamp(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_Timestamp_s* msg, bool tao);
void _decode_uavcan_Timestamp(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_Timestamp_s* msg, bool tao);
