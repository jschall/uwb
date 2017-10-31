#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_AIR_DATA_STATICTEMPERATURE_MAX_PACK_SIZE 4
#define UAVCAN_EQUIPMENT_AIR_DATA_STATICTEMPERATURE_DT_SIG 0x49272A6477D96271
#define UAVCAN_EQUIPMENT_AIR_DATA_STATICTEMPERATURE_DT_ID 1029

struct uavcan_equipment_air_data_StaticTemperature_s {
    float static_temperature;
    float static_temperature_variance;
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_air_data_StaticTemperature_descriptor;

uint32_t encode_uavcan_equipment_air_data_StaticTemperature(struct uavcan_equipment_air_data_StaticTemperature_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_air_data_StaticTemperature(const CanardRxTransfer* transfer, struct uavcan_equipment_air_data_StaticTemperature_s* msg);
void _encode_uavcan_equipment_air_data_StaticTemperature(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_air_data_StaticTemperature_s* msg, bool tao);
void _decode_uavcan_equipment_air_data_StaticTemperature(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_air_data_StaticTemperature_s* msg, bool tao);
