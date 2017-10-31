#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_AIR_DATA_TRUEAIRSPEED_MAX_PACK_SIZE 4
#define UAVCAN_EQUIPMENT_AIR_DATA_TRUEAIRSPEED_DT_SIG 0x306F69E0A591AFAA
#define UAVCAN_EQUIPMENT_AIR_DATA_TRUEAIRSPEED_DT_ID 1020

struct uavcan_equipment_air_data_TrueAirspeed_s {
    float true_airspeed;
    float true_airspeed_variance;
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_air_data_TrueAirspeed_descriptor;

uint32_t encode_uavcan_equipment_air_data_TrueAirspeed(struct uavcan_equipment_air_data_TrueAirspeed_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_air_data_TrueAirspeed(const CanardRxTransfer* transfer, struct uavcan_equipment_air_data_TrueAirspeed_s* msg);
void _encode_uavcan_equipment_air_data_TrueAirspeed(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_air_data_TrueAirspeed_s* msg, bool tao);
void _decode_uavcan_equipment_air_data_TrueAirspeed(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_air_data_TrueAirspeed_s* msg, bool tao);
