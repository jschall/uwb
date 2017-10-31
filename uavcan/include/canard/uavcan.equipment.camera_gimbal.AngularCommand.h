#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.equipment.camera_gimbal.Mode.h>

#define UAVCAN_EQUIPMENT_CAMERA_GIMBAL_ANGULARCOMMAND_MAX_PACK_SIZE 10
#define UAVCAN_EQUIPMENT_CAMERA_GIMBAL_ANGULARCOMMAND_DT_SIG 0x4AF6E57B2B2BE29C
#define UAVCAN_EQUIPMENT_CAMERA_GIMBAL_ANGULARCOMMAND_DT_ID 1040

struct uavcan_equipment_camera_gimbal_AngularCommand_s {
    uint8_t gimbal_id;
    struct uavcan_equipment_camera_gimbal_Mode_s mode;
    float quaternion_xyzw[4];
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_camera_gimbal_AngularCommand_descriptor;

uint32_t encode_uavcan_equipment_camera_gimbal_AngularCommand(struct uavcan_equipment_camera_gimbal_AngularCommand_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_camera_gimbal_AngularCommand(const CanardRxTransfer* transfer, struct uavcan_equipment_camera_gimbal_AngularCommand_s* msg);
void _encode_uavcan_equipment_camera_gimbal_AngularCommand(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_camera_gimbal_AngularCommand_s* msg, bool tao);
void _decode_uavcan_equipment_camera_gimbal_AngularCommand(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_camera_gimbal_AngularCommand_s* msg, bool tao);
