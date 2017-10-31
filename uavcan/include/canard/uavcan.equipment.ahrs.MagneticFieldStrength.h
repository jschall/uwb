#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_AHRS_MAGNETICFIELDSTRENGTH_MAX_PACK_SIZE 25
#define UAVCAN_EQUIPMENT_AHRS_MAGNETICFIELDSTRENGTH_DT_SIG 0xE2A7D4A9460BC2F2
#define UAVCAN_EQUIPMENT_AHRS_MAGNETICFIELDSTRENGTH_DT_ID 1001

struct uavcan_equipment_ahrs_MagneticFieldStrength_s {
    float magnetic_field_ga[3];
    struct {uint8_t magnetic_field_covariance_len; float magnetic_field_covariance[9];};
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_ahrs_MagneticFieldStrength_descriptor;

uint32_t encode_uavcan_equipment_ahrs_MagneticFieldStrength(struct uavcan_equipment_ahrs_MagneticFieldStrength_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_ahrs_MagneticFieldStrength(const CanardRxTransfer* transfer, struct uavcan_equipment_ahrs_MagneticFieldStrength_s* msg);
void _encode_uavcan_equipment_ahrs_MagneticFieldStrength(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_ahrs_MagneticFieldStrength_s* msg, bool tao);
void _decode_uavcan_equipment_ahrs_MagneticFieldStrength(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_ahrs_MagneticFieldStrength_s* msg, bool tao);
