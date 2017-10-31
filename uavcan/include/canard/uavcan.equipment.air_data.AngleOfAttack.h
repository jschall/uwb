#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_AIR_DATA_ANGLEOFATTACK_MAX_PACK_SIZE 5
#define UAVCAN_EQUIPMENT_AIR_DATA_ANGLEOFATTACK_DT_SIG 0xD5513C3F7AFAC74E
#define UAVCAN_EQUIPMENT_AIR_DATA_ANGLEOFATTACK_DT_ID 1025

#define UAVCAN_EQUIPMENT_AIR_DATA_ANGLEOFATTACK_SENSOR_ID_LEFT 254
#define UAVCAN_EQUIPMENT_AIR_DATA_ANGLEOFATTACK_SENSOR_ID_RIGHT 255

struct uavcan_equipment_air_data_AngleOfAttack_s {
    uint8_t sensor_id;
    float aoa;
    float aoa_variance;
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_air_data_AngleOfAttack_descriptor;

uint32_t encode_uavcan_equipment_air_data_AngleOfAttack(struct uavcan_equipment_air_data_AngleOfAttack_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_air_data_AngleOfAttack(const CanardRxTransfer* transfer, struct uavcan_equipment_air_data_AngleOfAttack_s* msg);
void _encode_uavcan_equipment_air_data_AngleOfAttack(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_air_data_AngleOfAttack_s* msg, bool tao);
void _decode_uavcan_equipment_air_data_AngleOfAttack(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_air_data_AngleOfAttack_s* msg, bool tao);
