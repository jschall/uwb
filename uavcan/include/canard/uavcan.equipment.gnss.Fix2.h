#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.equipment.gnss.ECEFPositionVelocity.h>
#include <canard/uavcan.Timestamp.h>

#define UAVCAN_EQUIPMENT_GNSS_FIX2_MAX_PACK_SIZE 222
#define UAVCAN_EQUIPMENT_GNSS_FIX2_DT_SIG 0xCA41E7000F37435F
#define UAVCAN_EQUIPMENT_GNSS_FIX2_DT_ID 1063

#define UAVCAN_EQUIPMENT_GNSS_FIX2_GNSS_TIME_STANDARD_NONE 0
#define UAVCAN_EQUIPMENT_GNSS_FIX2_GNSS_TIME_STANDARD_TAI 1
#define UAVCAN_EQUIPMENT_GNSS_FIX2_GNSS_TIME_STANDARD_UTC 2
#define UAVCAN_EQUIPMENT_GNSS_FIX2_GNSS_TIME_STANDARD_GPS 3
#define UAVCAN_EQUIPMENT_GNSS_FIX2_NUM_LEAP_SECONDS_UNKNOWN 0
#define UAVCAN_EQUIPMENT_GNSS_FIX2_STATUS_NO_FIX 0
#define UAVCAN_EQUIPMENT_GNSS_FIX2_STATUS_TIME_ONLY 1
#define UAVCAN_EQUIPMENT_GNSS_FIX2_STATUS_2D_FIX 2
#define UAVCAN_EQUIPMENT_GNSS_FIX2_STATUS_3D_FIX 3
#define UAVCAN_EQUIPMENT_GNSS_FIX2_MODE_SINGLE 0
#define UAVCAN_EQUIPMENT_GNSS_FIX2_MODE_DGPS 1
#define UAVCAN_EQUIPMENT_GNSS_FIX2_MODE_RTK 2
#define UAVCAN_EQUIPMENT_GNSS_FIX2_MODE_PPP 3
#define UAVCAN_EQUIPMENT_GNSS_FIX2_SUB_MODE_DGPS_OTHER 0
#define UAVCAN_EQUIPMENT_GNSS_FIX2_SUB_MODE_DGPS_SBAS 1
#define UAVCAN_EQUIPMENT_GNSS_FIX2_SUB_MODE_RTK_FLOAT 0
#define UAVCAN_EQUIPMENT_GNSS_FIX2_SUB_MODE_RTK_FIXED 1

struct uavcan_equipment_gnss_Fix2_s {
    struct uavcan_Timestamp_s timestamp;
    struct uavcan_Timestamp_s gnss_timestamp;
    uint8_t gnss_time_standard;
    uint8_t num_leap_seconds;
    int64_t longitude_deg_1e8;
    int64_t latitude_deg_1e8;
    int32_t height_ellipsoid_mm;
    int32_t height_msl_mm;
    float ned_velocity[3];
    uint8_t sats_used;
    uint8_t status;
    uint8_t mode;
    uint8_t sub_mode;
    struct {uint8_t covariance_len; float covariance[36];};
    float pdop;
    struct {uint8_t ecef_position_velocity_len; struct uavcan_equipment_gnss_ECEFPositionVelocity_s ecef_position_velocity[1];};
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_gnss_Fix2_descriptor;

uint32_t encode_uavcan_equipment_gnss_Fix2(struct uavcan_equipment_gnss_Fix2_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_gnss_Fix2(const CanardRxTransfer* transfer, struct uavcan_equipment_gnss_Fix2_s* msg);
void _encode_uavcan_equipment_gnss_Fix2(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_gnss_Fix2_s* msg, bool tao);
void _decode_uavcan_equipment_gnss_Fix2(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_gnss_Fix2_s* msg, bool tao);
