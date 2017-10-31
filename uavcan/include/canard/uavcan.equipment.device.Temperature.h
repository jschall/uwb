#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_DEVICE_TEMPERATURE_MAX_PACK_SIZE 5
#define UAVCAN_EQUIPMENT_DEVICE_TEMPERATURE_DT_SIG 0x70261C28A94144C6
#define UAVCAN_EQUIPMENT_DEVICE_TEMPERATURE_DT_ID 1110

#define UAVCAN_EQUIPMENT_DEVICE_TEMPERATURE_ERROR_FLAG_OVERHEATING 1
#define UAVCAN_EQUIPMENT_DEVICE_TEMPERATURE_ERROR_FLAG_OVERCOOLING 2

struct uavcan_equipment_device_Temperature_s {
    uint16_t device_id;
    float temperature;
    uint8_t error_flags;
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_device_Temperature_descriptor;

uint32_t encode_uavcan_equipment_device_Temperature(struct uavcan_equipment_device_Temperature_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_device_Temperature(const CanardRxTransfer* transfer, struct uavcan_equipment_device_Temperature_s* msg);
void _encode_uavcan_equipment_device_Temperature(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_device_Temperature_s* msg, bool tao);
void _decode_uavcan_equipment_device_Temperature(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_device_Temperature_s* msg, bool tao);
