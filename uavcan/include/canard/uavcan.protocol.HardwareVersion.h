#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_HARDWAREVERSION_MAX_PACK_SIZE 274
#define UAVCAN_PROTOCOL_HARDWAREVERSION_DT_SIG 0xAD5C4C933F4A0C4

struct uavcan_protocol_HardwareVersion_s {
    uint8_t major;
    uint8_t minor;
    uint8_t unique_id[16];
    struct {uint8_t certificate_of_authenticity_len; uint8_t certificate_of_authenticity[255];};
};


uint32_t encode_uavcan_protocol_HardwareVersion(struct uavcan_protocol_HardwareVersion_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_HardwareVersion(const CanardRxTransfer* transfer, struct uavcan_protocol_HardwareVersion_s* msg);
void _encode_uavcan_protocol_HardwareVersion(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_HardwareVersion_s* msg, bool tao);
void _decode_uavcan_protocol_HardwareVersion(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_HardwareVersion_s* msg, bool tao);
