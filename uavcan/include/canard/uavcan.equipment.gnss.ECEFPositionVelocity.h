#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_GNSS_ECEFPOSITIONVELOCITY_MAX_PACK_SIZE 99
#define UAVCAN_EQUIPMENT_GNSS_ECEFPOSITIONVELOCITY_DT_SIG 0x24A5DA4ABEE3A248

struct uavcan_equipment_gnss_ECEFPositionVelocity_s {
    float velocity_xyz[3];
    int64_t position_xyz_mm[3];
    struct {uint8_t covariance_len; float covariance[36];};
};


uint32_t encode_uavcan_equipment_gnss_ECEFPositionVelocity(struct uavcan_equipment_gnss_ECEFPositionVelocity_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_gnss_ECEFPositionVelocity(const CanardRxTransfer* transfer, struct uavcan_equipment_gnss_ECEFPositionVelocity_s* msg);
void _encode_uavcan_equipment_gnss_ECEFPositionVelocity(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_gnss_ECEFPositionVelocity_s* msg, bool tao);
void _decode_uavcan_equipment_gnss_ECEFPositionVelocity(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_gnss_ECEFPositionVelocity_s* msg, bool tao);
