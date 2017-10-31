#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.Timestamp.h>

#define UAVCAN_NAVIGATION_GLOBALNAVIGATIONSOLUTION_MAX_PACK_SIZE 233
#define UAVCAN_NAVIGATION_GLOBALNAVIGATIONSOLUTION_DT_SIG 0x463B10CCCBE51C3D
#define UAVCAN_NAVIGATION_GLOBALNAVIGATIONSOLUTION_DT_ID 2000

struct uavcan_navigation_GlobalNavigationSolution_s {
    struct uavcan_Timestamp_s timestamp;
    double longitude;
    double latitude;
    float height_ellipsoid;
    float height_msl;
    float height_agl;
    float height_baro;
    float qnh_hpa;
    float orientation_xyzw[4];
    struct {uint8_t pose_covariance_len; float pose_covariance[36];};
    float linear_velocity_body[3];
    float angular_velocity_body[3];
    float linear_acceleration_body[3];
    struct {uint8_t velocity_covariance_len; float velocity_covariance[36];};
};

extern const struct uavcan_message_descriptor_s uavcan_navigation_GlobalNavigationSolution_descriptor;

uint32_t encode_uavcan_navigation_GlobalNavigationSolution(struct uavcan_navigation_GlobalNavigationSolution_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_navigation_GlobalNavigationSolution(const CanardRxTransfer* transfer, struct uavcan_navigation_GlobalNavigationSolution_s* msg);
void _encode_uavcan_navigation_GlobalNavigationSolution(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_navigation_GlobalNavigationSolution_s* msg, bool tao);
void _decode_uavcan_navigation_GlobalNavigationSolution(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_navigation_GlobalNavigationSolution_s* msg, bool tao);
