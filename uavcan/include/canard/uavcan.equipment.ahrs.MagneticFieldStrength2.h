#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_AHRS_MAGNETICFIELDSTRENGTH2_MAX_PACK_SIZE 26
#define UAVCAN_EQUIPMENT_AHRS_MAGNETICFIELDSTRENGTH2_DT_SIG 0xB6AC0C442430297E
#define UAVCAN_EQUIPMENT_AHRS_MAGNETICFIELDSTRENGTH2_DT_ID 1002

struct uavcan_equipment_ahrs_MagneticFieldStrength2_s {
    uint8_t sensor_id;
    float magnetic_field_ga[3];
    struct {uint8_t magnetic_field_covariance_len; float magnetic_field_covariance[9];};
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_ahrs_MagneticFieldStrength2_descriptor;

uint32_t encode_uavcan_equipment_ahrs_MagneticFieldStrength2(struct uavcan_equipment_ahrs_MagneticFieldStrength2_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_ahrs_MagneticFieldStrength2(const CanardRxTransfer* transfer, struct uavcan_equipment_ahrs_MagneticFieldStrength2_s* msg);
void _encode_uavcan_equipment_ahrs_MagneticFieldStrength2(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_ahrs_MagneticFieldStrength2_s* msg, bool tao);
void _decode_uavcan_equipment_ahrs_MagneticFieldStrength2(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_ahrs_MagneticFieldStrength2_s* msg, bool tao);
