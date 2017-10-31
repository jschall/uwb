#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_ICE_FUELTANKSTATUS_MAX_PACK_SIZE 13
#define UAVCAN_EQUIPMENT_ICE_FUELTANKSTATUS_DT_SIG 0x286B4A387BA84BC4
#define UAVCAN_EQUIPMENT_ICE_FUELTANKSTATUS_DT_ID 1129

struct uavcan_equipment_ice_FuelTankStatus_s {
    uint8_t available_fuel_volume_percent;
    float available_fuel_volume_cm3;
    float fuel_consumption_rate_cm3pm;
    float fuel_temperature;
    uint8_t fuel_tank_id;
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_ice_FuelTankStatus_descriptor;

uint32_t encode_uavcan_equipment_ice_FuelTankStatus(struct uavcan_equipment_ice_FuelTankStatus_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_ice_FuelTankStatus(const CanardRxTransfer* transfer, struct uavcan_equipment_ice_FuelTankStatus_s* msg);
void _encode_uavcan_equipment_ice_FuelTankStatus(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_ice_FuelTankStatus_s* msg, bool tao);
void _decode_uavcan_equipment_ice_FuelTankStatus(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_ice_FuelTankStatus_s* msg, bool tao);
