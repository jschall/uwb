#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.Timestamp.h>

#define UAVCAN_EQUIPMENT_GNSS_FIX_MAX_PACK_SIZE 79
#define UAVCAN_EQUIPMENT_GNSS_FIX_DT_SIG 0x54C1572B9E07F297
#define UAVCAN_EQUIPMENT_GNSS_FIX_DT_ID 1060

#define UAVCAN_EQUIPMENT_GNSS_FIX_GNSS_TIME_STANDARD_NONE 0
#define UAVCAN_EQUIPMENT_GNSS_FIX_GNSS_TIME_STANDARD_TAI 1
#define UAVCAN_EQUIPMENT_GNSS_FIX_GNSS_TIME_STANDARD_UTC 2
#define UAVCAN_EQUIPMENT_GNSS_FIX_GNSS_TIME_STANDARD_GPS 3
#define UAVCAN_EQUIPMENT_GNSS_FIX_NUM_LEAP_SECONDS_UNKNOWN 0
#define UAVCAN_EQUIPMENT_GNSS_FIX_STATUS_NO_FIX 0
#define UAVCAN_EQUIPMENT_GNSS_FIX_STATUS_TIME_ONLY 1
#define UAVCAN_EQUIPMENT_GNSS_FIX_STATUS_2D_FIX 2
#define UAVCAN_EQUIPMENT_GNSS_FIX_STATUS_3D_FIX 3

struct uavcan_equipment_gnss_Fix_s {
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
    float pdop;
    struct {uint8_t position_covariance_len; float position_covariance[9];};
    struct {uint8_t velocity_covariance_len; float velocity_covariance[9];};
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_gnss_Fix_descriptor;

uint32_t encode_uavcan_equipment_gnss_Fix(struct uavcan_equipment_gnss_Fix_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_gnss_Fix(const CanardRxTransfer* transfer, struct uavcan_equipment_gnss_Fix_s* msg);
void _encode_uavcan_equipment_gnss_Fix(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_gnss_Fix_s* msg, bool tao);
void _decode_uavcan_equipment_gnss_Fix(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_gnss_Fix_s* msg, bool tao);
