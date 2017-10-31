#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_COARSEORIENTATION_MAX_PACK_SIZE 2
#define UAVCAN_COARSEORIENTATION_DT_SIG 0x271BA10B0DAC9E52

#define UAVCAN_COARSEORIENTATION_ANGLE_MULTIPLIER 4.77464829276

struct uavcan_CoarseOrientation_s {
    int8_t fixed_axis_roll_pitch_yaw[3];
    bool orientation_defined;
};


uint32_t encode_uavcan_CoarseOrientation(struct uavcan_CoarseOrientation_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_CoarseOrientation(const CanardRxTransfer* transfer, struct uavcan_CoarseOrientation_s* msg);
void _encode_uavcan_CoarseOrientation(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_CoarseOrientation_s* msg, bool tao);
void _decode_uavcan_CoarseOrientation(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_CoarseOrientation_s* msg, bool tao);
