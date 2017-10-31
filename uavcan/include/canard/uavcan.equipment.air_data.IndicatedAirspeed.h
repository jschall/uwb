#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_AIR_DATA_INDICATEDAIRSPEED_MAX_PACK_SIZE 4
#define UAVCAN_EQUIPMENT_AIR_DATA_INDICATEDAIRSPEED_DT_SIG 0xA1892D72AB8945F
#define UAVCAN_EQUIPMENT_AIR_DATA_INDICATEDAIRSPEED_DT_ID 1021

struct uavcan_equipment_air_data_IndicatedAirspeed_s {
    float indicated_airspeed;
    float indicated_airspeed_variance;
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_air_data_IndicatedAirspeed_descriptor;

uint32_t encode_uavcan_equipment_air_data_IndicatedAirspeed(struct uavcan_equipment_air_data_IndicatedAirspeed_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_air_data_IndicatedAirspeed(const CanardRxTransfer* transfer, struct uavcan_equipment_air_data_IndicatedAirspeed_s* msg);
void _encode_uavcan_equipment_air_data_IndicatedAirspeed(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_air_data_IndicatedAirspeed_s* msg, bool tao);
void _decode_uavcan_equipment_air_data_IndicatedAirspeed(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_air_data_IndicatedAirspeed_s* msg, bool tao);
