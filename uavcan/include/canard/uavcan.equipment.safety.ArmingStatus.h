#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_SAFETY_ARMINGSTATUS_MAX_PACK_SIZE 1
#define UAVCAN_EQUIPMENT_SAFETY_ARMINGSTATUS_DT_SIG 0x8700F375556A8003
#define UAVCAN_EQUIPMENT_SAFETY_ARMINGSTATUS_DT_ID 1100

#define UAVCAN_EQUIPMENT_SAFETY_ARMINGSTATUS_STATUS_DISARMED 0
#define UAVCAN_EQUIPMENT_SAFETY_ARMINGSTATUS_STATUS_FULLY_ARMED 255

struct uavcan_equipment_safety_ArmingStatus_s {
    uint8_t status;
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_safety_ArmingStatus_descriptor;

uint32_t encode_uavcan_equipment_safety_ArmingStatus(struct uavcan_equipment_safety_ArmingStatus_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_safety_ArmingStatus(const CanardRxTransfer* transfer, struct uavcan_equipment_safety_ArmingStatus_s* msg);
void _encode_uavcan_equipment_safety_ArmingStatus(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_safety_ArmingStatus_s* msg, bool tao);
void _decode_uavcan_equipment_safety_ArmingStatus(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_safety_ArmingStatus_s* msg, bool tao);
