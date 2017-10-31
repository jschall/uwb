#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.equipment.camera_gimbal.Mode.h>

#define UAVCAN_EQUIPMENT_CAMERA_GIMBAL_STATUS_MAX_PACK_SIZE 29
#define UAVCAN_EQUIPMENT_CAMERA_GIMBAL_STATUS_DT_SIG 0xB9F127865BE0D61E
#define UAVCAN_EQUIPMENT_CAMERA_GIMBAL_STATUS_DT_ID 1044

struct uavcan_equipment_camera_gimbal_Status_s {
    uint8_t gimbal_id;
    struct uavcan_equipment_camera_gimbal_Mode_s mode;
    float camera_orientation_in_body_frame_xyzw[4];
    struct {uint8_t camera_orientation_in_body_frame_covariance_len; float camera_orientation_in_body_frame_covariance[9];};
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_camera_gimbal_Status_descriptor;

uint32_t encode_uavcan_equipment_camera_gimbal_Status(struct uavcan_equipment_camera_gimbal_Status_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_camera_gimbal_Status(const CanardRxTransfer* transfer, struct uavcan_equipment_camera_gimbal_Status_s* msg);
void _encode_uavcan_equipment_camera_gimbal_Status(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_camera_gimbal_Status_s* msg, bool tao);
void _decode_uavcan_equipment_camera_gimbal_Status(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_camera_gimbal_Status_s* msg, bool tao);
