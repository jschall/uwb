#include <canard/uavcan.equipment.ahrs.Solution.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_equipment_ahrs_Solution(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_equipment_ahrs_Solution(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_equipment_ahrs_Solution_descriptor = {
    UAVCAN_EQUIPMENT_AHRS_SOLUTION_DT_SIG,
    UAVCAN_EQUIPMENT_AHRS_SOLUTION_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_equipment_ahrs_Solution_s),
    UAVCAN_EQUIPMENT_AHRS_SOLUTION_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_equipment_ahrs_Solution(struct uavcan_equipment_ahrs_Solution_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_AHRS_SOLUTION_MAX_PACK_SIZE);
    _encode_uavcan_equipment_ahrs_Solution(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_ahrs_Solution(const CanardRxTransfer* transfer, struct uavcan_equipment_ahrs_Solution_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_ahrs_Solution(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_ahrs_Solution(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_ahrs_Solution_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    _encode_uavcan_Timestamp(buffer, bit_ofs, &msg->timestamp, false);
    for (size_t i=0; i < 4; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 16, &msg->orientation_xyzw[i]);
            *bit_ofs += 16;
    }
    *bit_ofs += 4;
    canardEncodeScalar(buffer, *bit_ofs, 4, &msg->orientation_covariance_len);
    *bit_ofs += 4;
    for (size_t i=0; i < msg->orientation_covariance_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 16, &msg->orientation_covariance[i]);
            *bit_ofs += 16;
    }
    for (size_t i=0; i < 3; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 16, &msg->angular_velocity[i]);
            *bit_ofs += 16;
    }
    *bit_ofs += 4;
    canardEncodeScalar(buffer, *bit_ofs, 4, &msg->angular_velocity_covariance_len);
    *bit_ofs += 4;
    for (size_t i=0; i < msg->angular_velocity_covariance_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 16, &msg->angular_velocity_covariance[i]);
            *bit_ofs += 16;
    }
    for (size_t i=0; i < 3; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 16, &msg->linear_acceleration[i]);
            *bit_ofs += 16;
    }
    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 4, &msg->linear_acceleration_covariance_len);
        *bit_ofs += 4;
    }
    for (size_t i=0; i < msg->linear_acceleration_covariance_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 16, &msg->linear_acceleration_covariance[i]);
            *bit_ofs += 16;
    }
}

void _decode_uavcan_equipment_ahrs_Solution(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_ahrs_Solution_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    _decode_uavcan_Timestamp(transfer, bit_ofs, &msg->timestamp, false);
    for (size_t i=0; i < 4; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->orientation_xyzw[i]);
            *bit_ofs += 16;
    }
    *bit_ofs += 4;
    canardDecodeScalar(transfer, *bit_ofs, 4, false, &msg->orientation_covariance_len);
    *bit_ofs += 4;
    for (size_t i=0; i < msg->orientation_covariance_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->orientation_covariance[i]);
            *bit_ofs += 16;
    }
    for (size_t i=0; i < 3; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->angular_velocity[i]);
            *bit_ofs += 16;
    }
    *bit_ofs += 4;
    canardDecodeScalar(transfer, *bit_ofs, 4, false, &msg->angular_velocity_covariance_len);
    *bit_ofs += 4;
    for (size_t i=0; i < msg->angular_velocity_covariance_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->angular_velocity_covariance[i]);
            *bit_ofs += 16;
    }
    for (size_t i=0; i < 3; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->linear_acceleration[i]);
            *bit_ofs += 16;
    }
    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 4, false, &msg->linear_acceleration_covariance_len);
        *bit_ofs += 4;
    }
    for (size_t i=0; i < msg->linear_acceleration_covariance_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->linear_acceleration_covariance[i]);
            *bit_ofs += 16;
    }
}
