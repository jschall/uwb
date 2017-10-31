#include <canard/uavcan.navigation.GlobalNavigationSolution.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_navigation_GlobalNavigationSolution(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_navigation_GlobalNavigationSolution(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_navigation_GlobalNavigationSolution_descriptor = {
    UAVCAN_NAVIGATION_GLOBALNAVIGATIONSOLUTION_DT_SIG,
    UAVCAN_NAVIGATION_GLOBALNAVIGATIONSOLUTION_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_navigation_GlobalNavigationSolution_s),
    UAVCAN_NAVIGATION_GLOBALNAVIGATIONSOLUTION_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_navigation_GlobalNavigationSolution(struct uavcan_navigation_GlobalNavigationSolution_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_NAVIGATION_GLOBALNAVIGATIONSOLUTION_MAX_PACK_SIZE);
    _encode_uavcan_navigation_GlobalNavigationSolution(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_navigation_GlobalNavigationSolution(const CanardRxTransfer* transfer, struct uavcan_navigation_GlobalNavigationSolution_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_navigation_GlobalNavigationSolution(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_navigation_GlobalNavigationSolution(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_navigation_GlobalNavigationSolution_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    _encode_uavcan_Timestamp(buffer, bit_ofs, &msg->timestamp, false);
    canardEncodeScalar(buffer, *bit_ofs, 64, &msg->longitude);
    *bit_ofs += 64;
    canardEncodeScalar(buffer, *bit_ofs, 64, &msg->latitude);
    *bit_ofs += 64;
    canardEncodeScalar(buffer, *bit_ofs, 32, &msg->height_ellipsoid);
    *bit_ofs += 32;
    canardEncodeScalar(buffer, *bit_ofs, 32, &msg->height_msl);
    *bit_ofs += 32;
    canardEncodeScalar(buffer, *bit_ofs, 32, &msg->height_agl);
    *bit_ofs += 32;
    canardEncodeScalar(buffer, *bit_ofs, 32, &msg->height_baro);
    *bit_ofs += 32;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->qnh_hpa);
    *bit_ofs += 16;
    for (size_t i=0; i < 4; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 32, &msg->orientation_xyzw[i]);
            *bit_ofs += 32;
    }
    canardEncodeScalar(buffer, *bit_ofs, 6, &msg->pose_covariance_len);
    *bit_ofs += 6;
    for (size_t i=0; i < msg->pose_covariance_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 16, &msg->pose_covariance[i]);
            *bit_ofs += 16;
    }
    for (size_t i=0; i < 3; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 32, &msg->linear_velocity_body[i]);
            *bit_ofs += 32;
    }
    for (size_t i=0; i < 3; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 32, &msg->angular_velocity_body[i]);
            *bit_ofs += 32;
    }
    for (size_t i=0; i < 3; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 16, &msg->linear_acceleration_body[i]);
            *bit_ofs += 16;
    }
    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 6, &msg->velocity_covariance_len);
        *bit_ofs += 6;
    }
    for (size_t i=0; i < msg->velocity_covariance_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 16, &msg->velocity_covariance[i]);
            *bit_ofs += 16;
    }
}

void _decode_uavcan_navigation_GlobalNavigationSolution(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_navigation_GlobalNavigationSolution_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    _decode_uavcan_Timestamp(transfer, bit_ofs, &msg->timestamp, false);
    canardDecodeScalar(transfer, *bit_ofs, 64, true, &msg->longitude);
    *bit_ofs += 64;
    canardDecodeScalar(transfer, *bit_ofs, 64, true, &msg->latitude);
    *bit_ofs += 64;
    canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->height_ellipsoid);
    *bit_ofs += 32;
    canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->height_msl);
    *bit_ofs += 32;
    canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->height_agl);
    *bit_ofs += 32;
    canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->height_baro);
    *bit_ofs += 32;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->qnh_hpa);
    *bit_ofs += 16;
    for (size_t i=0; i < 4; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->orientation_xyzw[i]);
            *bit_ofs += 32;
    }
    canardDecodeScalar(transfer, *bit_ofs, 6, false, &msg->pose_covariance_len);
    *bit_ofs += 6;
    for (size_t i=0; i < msg->pose_covariance_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->pose_covariance[i]);
            *bit_ofs += 16;
    }
    for (size_t i=0; i < 3; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->linear_velocity_body[i]);
            *bit_ofs += 32;
    }
    for (size_t i=0; i < 3; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->angular_velocity_body[i]);
            *bit_ofs += 32;
    }
    for (size_t i=0; i < 3; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->linear_acceleration_body[i]);
            *bit_ofs += 16;
    }
    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 6, false, &msg->velocity_covariance_len);
        *bit_ofs += 6;
    }
    for (size_t i=0; i < msg->velocity_covariance_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->velocity_covariance[i]);
            *bit_ofs += 16;
    }
}
