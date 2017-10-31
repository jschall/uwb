#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.equipment.camera_gimbal.Mode.h>

#define UAVCAN_EQUIPMENT_CAMERA_GIMBAL_GEOPOICOMMAND_MAX_PACK_SIZE 13
#define UAVCAN_EQUIPMENT_CAMERA_GIMBAL_GEOPOICOMMAND_DT_SIG 0x9371428A92F01FD6
#define UAVCAN_EQUIPMENT_CAMERA_GIMBAL_GEOPOICOMMAND_DT_ID 1041

#define UAVCAN_EQUIPMENT_CAMERA_GIMBAL_GEOPOICOMMAND_HEIGHT_REFERENCE_ELLIPSOID 0
#define UAVCAN_EQUIPMENT_CAMERA_GIMBAL_GEOPOICOMMAND_HEIGHT_REFERENCE_MEAN_SEA_LEVEL 1

struct uavcan_equipment_camera_gimbal_GEOPOICommand_s {
    uint8_t gimbal_id;
    struct uavcan_equipment_camera_gimbal_Mode_s mode;
    int32_t longitude_deg_1e7;
    int32_t latitude_deg_1e7;
    int32_t height_cm;
    uint8_t height_reference;
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_camera_gimbal_GEOPOICommand_descriptor;

uint32_t encode_uavcan_equipment_camera_gimbal_GEOPOICommand(struct uavcan_equipment_camera_gimbal_GEOPOICommand_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_camera_gimbal_GEOPOICommand(const CanardRxTransfer* transfer, struct uavcan_equipment_camera_gimbal_GEOPOICommand_s* msg);
void _encode_uavcan_equipment_camera_gimbal_GEOPOICommand(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_camera_gimbal_GEOPOICommand_s* msg, bool tao);
void _decode_uavcan_equipment_camera_gimbal_GEOPOICommand(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_camera_gimbal_GEOPOICommand_s* msg, bool tao);
