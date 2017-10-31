#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_ICE_RECIPROCATING_CYLINDERSTATUS_MAX_PACK_SIZE 10
#define UAVCAN_EQUIPMENT_ICE_RECIPROCATING_CYLINDERSTATUS_DT_SIG 0xD68AC83A89D5B36B

struct uavcan_equipment_ice_reciprocating_CylinderStatus_s {
    float ignition_timing_deg;
    float injection_time_ms;
    float cylinder_head_temperature;
    float exhaust_gas_temperature;
    float lambda_coefficient;
};


uint32_t encode_uavcan_equipment_ice_reciprocating_CylinderStatus(struct uavcan_equipment_ice_reciprocating_CylinderStatus_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_ice_reciprocating_CylinderStatus(const CanardRxTransfer* transfer, struct uavcan_equipment_ice_reciprocating_CylinderStatus_s* msg);
void _encode_uavcan_equipment_ice_reciprocating_CylinderStatus(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_ice_reciprocating_CylinderStatus_s* msg, bool tao);
void _decode_uavcan_equipment_ice_reciprocating_CylinderStatus(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_ice_reciprocating_CylinderStatus_s* msg, bool tao);
