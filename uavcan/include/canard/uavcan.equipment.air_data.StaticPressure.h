#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_AIR_DATA_STATICPRESSURE_MAX_PACK_SIZE 6
#define UAVCAN_EQUIPMENT_AIR_DATA_STATICPRESSURE_DT_SIG 0xCDC7C43412BDC89A
#define UAVCAN_EQUIPMENT_AIR_DATA_STATICPRESSURE_DT_ID 1028

struct uavcan_equipment_air_data_StaticPressure_s {
    float static_pressure;
    float static_pressure_variance;
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_air_data_StaticPressure_descriptor;

uint32_t encode_uavcan_equipment_air_data_StaticPressure(struct uavcan_equipment_air_data_StaticPressure_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_air_data_StaticPressure(const CanardRxTransfer* transfer, struct uavcan_equipment_air_data_StaticPressure_s* msg);
void _encode_uavcan_equipment_air_data_StaticPressure(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_air_data_StaticPressure_s* msg, bool tao);
void _decode_uavcan_equipment_air_data_StaticPressure(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_air_data_StaticPressure_s* msg, bool tao);
