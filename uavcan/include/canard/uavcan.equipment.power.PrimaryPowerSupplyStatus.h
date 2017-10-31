#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_POWER_PRIMARYPOWERSUPPLYSTATUS_MAX_PACK_SIZE 6
#define UAVCAN_EQUIPMENT_POWER_PRIMARYPOWERSUPPLYSTATUS_DT_SIG 0xBBA05074AD757480
#define UAVCAN_EQUIPMENT_POWER_PRIMARYPOWERSUPPLYSTATUS_DT_ID 1090

struct uavcan_equipment_power_PrimaryPowerSupplyStatus_s {
    float hours_to_empty_at_10sec_avg_power;
    float hours_to_empty_at_10sec_avg_power_variance;
    bool external_power_available;
    uint8_t remaining_energy_pct;
    uint8_t remaining_energy_pct_stdev;
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_power_PrimaryPowerSupplyStatus_descriptor;

uint32_t encode_uavcan_equipment_power_PrimaryPowerSupplyStatus(struct uavcan_equipment_power_PrimaryPowerSupplyStatus_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_power_PrimaryPowerSupplyStatus(const CanardRxTransfer* transfer, struct uavcan_equipment_power_PrimaryPowerSupplyStatus_s* msg);
void _encode_uavcan_equipment_power_PrimaryPowerSupplyStatus(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_power_PrimaryPowerSupplyStatus_s* msg, bool tao);
void _decode_uavcan_equipment_power_PrimaryPowerSupplyStatus(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_power_PrimaryPowerSupplyStatus_s* msg, bool tao);
