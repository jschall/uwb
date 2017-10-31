#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_CAMERA_GIMBAL_MODE_MAX_PACK_SIZE 1
#define UAVCAN_EQUIPMENT_CAMERA_GIMBAL_MODE_DT_SIG 0x9108C7785AEB69C4

#define UAVCAN_EQUIPMENT_CAMERA_GIMBAL_MODE_COMMAND_MODE_ANGULAR_VELOCITY 0
#define UAVCAN_EQUIPMENT_CAMERA_GIMBAL_MODE_COMMAND_MODE_ORIENTATION_FIXED_FRAME 1
#define UAVCAN_EQUIPMENT_CAMERA_GIMBAL_MODE_COMMAND_MODE_ORIENTATION_BODY_FRAME 2
#define UAVCAN_EQUIPMENT_CAMERA_GIMBAL_MODE_COMMAND_MODE_GEO_POI 3

struct uavcan_equipment_camera_gimbal_Mode_s {
    uint8_t command_mode;
};


uint32_t encode_uavcan_equipment_camera_gimbal_Mode(struct uavcan_equipment_camera_gimbal_Mode_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_camera_gimbal_Mode(const CanardRxTransfer* transfer, struct uavcan_equipment_camera_gimbal_Mode_s* msg);
void _encode_uavcan_equipment_camera_gimbal_Mode(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_camera_gimbal_Mode_s* msg, bool tao);
void _decode_uavcan_equipment_camera_gimbal_Mode(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_camera_gimbal_Mode_s* msg, bool tao);
