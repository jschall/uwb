#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.Timestamp.h>

#define UAVCAN_EQUIPMENT_AHRS_RAWIMU_MAX_PACK_SIZE 120
#define UAVCAN_EQUIPMENT_AHRS_RAWIMU_DT_SIG 0x8280632C40E574B5
#define UAVCAN_EQUIPMENT_AHRS_RAWIMU_DT_ID 1003

struct uavcan_equipment_ahrs_RawIMU_s {
    struct uavcan_Timestamp_s timestamp;
    float integration_interval;
    float rate_gyro_latest[3];
    float rate_gyro_integral[3];
    float accelerometer_latest[3];
    float accelerometer_integral[3];
    struct {uint8_t covariance_len; float covariance[36];};
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_ahrs_RawIMU_descriptor;

uint32_t encode_uavcan_equipment_ahrs_RawIMU(struct uavcan_equipment_ahrs_RawIMU_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_ahrs_RawIMU(const CanardRxTransfer* transfer, struct uavcan_equipment_ahrs_RawIMU_s* msg);
void _encode_uavcan_equipment_ahrs_RawIMU(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_ahrs_RawIMU_s* msg, bool tao);
void _decode_uavcan_equipment_ahrs_RawIMU(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_ahrs_RawIMU_s* msg, bool tao);
