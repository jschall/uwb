#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_POWER_CIRCUITSTATUS_MAX_PACK_SIZE 7
#define UAVCAN_EQUIPMENT_POWER_CIRCUITSTATUS_DT_SIG 0x8313D33D0DDDA115
#define UAVCAN_EQUIPMENT_POWER_CIRCUITSTATUS_DT_ID 1091

#define UAVCAN_EQUIPMENT_POWER_CIRCUITSTATUS_ERROR_FLAG_OVERVOLTAGE 1
#define UAVCAN_EQUIPMENT_POWER_CIRCUITSTATUS_ERROR_FLAG_UNDERVOLTAGE 2
#define UAVCAN_EQUIPMENT_POWER_CIRCUITSTATUS_ERROR_FLAG_OVERCURRENT 4
#define UAVCAN_EQUIPMENT_POWER_CIRCUITSTATUS_ERROR_FLAG_UNDERCURRENT 8

struct uavcan_equipment_power_CircuitStatus_s {
    uint16_t circuit_id;
    float voltage;
    float current;
    uint8_t error_flags;
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_power_CircuitStatus_descriptor;

uint32_t encode_uavcan_equipment_power_CircuitStatus(struct uavcan_equipment_power_CircuitStatus_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_power_CircuitStatus(const CanardRxTransfer* transfer, struct uavcan_equipment_power_CircuitStatus_s* msg);
void _encode_uavcan_equipment_power_CircuitStatus(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_power_CircuitStatus_s* msg, bool tao);
void _decode_uavcan_equipment_power_CircuitStatus(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_power_CircuitStatus_s* msg, bool tao);
