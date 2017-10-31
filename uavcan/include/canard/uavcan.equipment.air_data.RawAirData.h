#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_AIR_DATA_RAWAIRDATA_MAX_PACK_SIZE 50
#define UAVCAN_EQUIPMENT_AIR_DATA_RAWAIRDATA_DT_SIG 0xC77DF38BA122F5DA
#define UAVCAN_EQUIPMENT_AIR_DATA_RAWAIRDATA_DT_ID 1027

#define UAVCAN_EQUIPMENT_AIR_DATA_RAWAIRDATA_FLAG_HEATER_AVAILABLE 1
#define UAVCAN_EQUIPMENT_AIR_DATA_RAWAIRDATA_FLAG_HEATER_WORKING 2
#define UAVCAN_EQUIPMENT_AIR_DATA_RAWAIRDATA_FLAG_HEATER_OVERCURRENT 4
#define UAVCAN_EQUIPMENT_AIR_DATA_RAWAIRDATA_FLAG_HEATER_OPENCIRCUIT 8

struct uavcan_equipment_air_data_RawAirData_s {
    uint8_t flags;
    float static_pressure;
    float differential_pressure;
    float static_pressure_sensor_temperature;
    float differential_pressure_sensor_temperature;
    float static_air_temperature;
    float pitot_temperature;
    struct {uint8_t covariance_len; float covariance[16];};
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_air_data_RawAirData_descriptor;

uint32_t encode_uavcan_equipment_air_data_RawAirData(struct uavcan_equipment_air_data_RawAirData_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_air_data_RawAirData(const CanardRxTransfer* transfer, struct uavcan_equipment_air_data_RawAirData_s* msg);
void _encode_uavcan_equipment_air_data_RawAirData(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_air_data_RawAirData_s* msg, bool tao);
void _decode_uavcan_equipment_air_data_RawAirData(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_air_data_RawAirData_s* msg, bool tao);
