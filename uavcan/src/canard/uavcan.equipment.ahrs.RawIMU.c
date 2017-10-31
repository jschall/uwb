#include <canard/uavcan.equipment.ahrs.RawIMU.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_equipment_ahrs_RawIMU(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_equipment_ahrs_RawIMU(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_equipment_ahrs_RawIMU_descriptor = {
    UAVCAN_EQUIPMENT_AHRS_RAWIMU_DT_SIG,
    UAVCAN_EQUIPMENT_AHRS_RAWIMU_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_equipment_ahrs_RawIMU_s),
    UAVCAN_EQUIPMENT_AHRS_RAWIMU_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_equipment_ahrs_RawIMU(struct uavcan_equipment_ahrs_RawIMU_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_AHRS_RAWIMU_MAX_PACK_SIZE);
    _encode_uavcan_equipment_ahrs_RawIMU(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_ahrs_RawIMU(const CanardRxTransfer* transfer, struct uavcan_equipment_ahrs_RawIMU_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_ahrs_RawIMU(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_ahrs_RawIMU(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_ahrs_RawIMU_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    _encode_uavcan_Timestamp(buffer, bit_ofs, &msg->timestamp, false);
    canardEncodeScalar(buffer, *bit_ofs, 32, &msg->integration_interval);
    *bit_ofs += 32;
    for (size_t i=0; i < 3; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 16, &msg->rate_gyro_latest[i]);
            *bit_ofs += 16;
    }
    for (size_t i=0; i < 3; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 32, &msg->rate_gyro_integral[i]);
            *bit_ofs += 32;
    }
    for (size_t i=0; i < 3; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 16, &msg->accelerometer_latest[i]);
            *bit_ofs += 16;
    }
    for (size_t i=0; i < 3; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 32, &msg->accelerometer_integral[i]);
            *bit_ofs += 32;
    }
    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 6, &msg->covariance_len);
        *bit_ofs += 6;
    }
    for (size_t i=0; i < msg->covariance_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 16, &msg->covariance[i]);
            *bit_ofs += 16;
    }
}

void _decode_uavcan_equipment_ahrs_RawIMU(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_ahrs_RawIMU_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    _decode_uavcan_Timestamp(transfer, bit_ofs, &msg->timestamp, false);
    canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->integration_interval);
    *bit_ofs += 32;
    for (size_t i=0; i < 3; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->rate_gyro_latest[i]);
            *bit_ofs += 16;
    }
    for (size_t i=0; i < 3; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->rate_gyro_integral[i]);
            *bit_ofs += 32;
    }
    for (size_t i=0; i < 3; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->accelerometer_latest[i]);
            *bit_ofs += 16;
    }
    for (size_t i=0; i < 3; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->accelerometer_integral[i]);
            *bit_ofs += 32;
    }
    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 6, false, &msg->covariance_len);
        *bit_ofs += 6;
    }
    for (size_t i=0; i < msg->covariance_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->covariance[i]);
            *bit_ofs += 16;
    }
}
