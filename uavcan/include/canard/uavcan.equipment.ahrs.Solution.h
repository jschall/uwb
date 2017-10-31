#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.Timestamp.h>

#define UAVCAN_EQUIPMENT_AHRS_SOLUTION_MAX_PACK_SIZE 84
#define UAVCAN_EQUIPMENT_AHRS_SOLUTION_DT_SIG 0x72A63A3C6F41FA9B
#define UAVCAN_EQUIPMENT_AHRS_SOLUTION_DT_ID 1000

struct uavcan_equipment_ahrs_Solution_s {
    struct uavcan_Timestamp_s timestamp;
    float orientation_xyzw[4];
    struct {uint8_t orientation_covariance_len; float orientation_covariance[9];};
    float angular_velocity[3];
    struct {uint8_t angular_velocity_covariance_len; float angular_velocity_covariance[9];};
    float linear_acceleration[3];
    struct {uint8_t linear_acceleration_covariance_len; float linear_acceleration_covariance[9];};
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_ahrs_Solution_descriptor;

uint32_t encode_uavcan_equipment_ahrs_Solution(struct uavcan_equipment_ahrs_Solution_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_ahrs_Solution(const CanardRxTransfer* transfer, struct uavcan_equipment_ahrs_Solution_s* msg);
void _encode_uavcan_equipment_ahrs_Solution(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_ahrs_Solution_s* msg, bool tao);
void _decode_uavcan_equipment_ahrs_Solution(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_ahrs_Solution_s* msg, bool tao);
