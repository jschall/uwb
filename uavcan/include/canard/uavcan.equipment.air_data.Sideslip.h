#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_AIR_DATA_SIDESLIP_MAX_PACK_SIZE 4
#define UAVCAN_EQUIPMENT_AIR_DATA_SIDESLIP_DT_SIG 0x7B48E55FCFF42A57
#define UAVCAN_EQUIPMENT_AIR_DATA_SIDESLIP_DT_ID 1026

struct uavcan_equipment_air_data_Sideslip_s {
    float sideslip_angle;
    float sideslip_angle_variance;
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_air_data_Sideslip_descriptor;

uint32_t encode_uavcan_equipment_air_data_Sideslip(struct uavcan_equipment_air_data_Sideslip_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_air_data_Sideslip(const CanardRxTransfer* transfer, struct uavcan_equipment_air_data_Sideslip_s* msg);
void _encode_uavcan_equipment_air_data_Sideslip(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_air_data_Sideslip_s* msg, bool tao);
void _decode_uavcan_equipment_air_data_Sideslip(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_air_data_Sideslip_s* msg, bool tao);
