#include <canard/uavcan.equipment.ahrs.MagneticFieldStrength2.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_equipment_ahrs_MagneticFieldStrength2(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_equipment_ahrs_MagneticFieldStrength2(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_equipment_ahrs_MagneticFieldStrength2_descriptor = {
    UAVCAN_EQUIPMENT_AHRS_MAGNETICFIELDSTRENGTH2_DT_SIG,
    UAVCAN_EQUIPMENT_AHRS_MAGNETICFIELDSTRENGTH2_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_equipment_ahrs_MagneticFieldStrength2_s),
    UAVCAN_EQUIPMENT_AHRS_MAGNETICFIELDSTRENGTH2_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_equipment_ahrs_MagneticFieldStrength2(struct uavcan_equipment_ahrs_MagneticFieldStrength2_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_AHRS_MAGNETICFIELDSTRENGTH2_MAX_PACK_SIZE);
    _encode_uavcan_equipment_ahrs_MagneticFieldStrength2(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_ahrs_MagneticFieldStrength2(const CanardRxTransfer* transfer, struct uavcan_equipment_ahrs_MagneticFieldStrength2_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_ahrs_MagneticFieldStrength2(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_ahrs_MagneticFieldStrength2(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_ahrs_MagneticFieldStrength2_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->sensor_id);
    *bit_ofs += 8;
    for (size_t i=0; i < 3; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 16, &msg->magnetic_field_ga[i]);
            *bit_ofs += 16;
    }
    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 4, &msg->magnetic_field_covariance_len);
        *bit_ofs += 4;
    }
    for (size_t i=0; i < msg->magnetic_field_covariance_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 16, &msg->magnetic_field_covariance[i]);
            *bit_ofs += 16;
    }
}

void _decode_uavcan_equipment_ahrs_MagneticFieldStrength2(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_ahrs_MagneticFieldStrength2_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->sensor_id);
    *bit_ofs += 8;
    for (size_t i=0; i < 3; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->magnetic_field_ga[i]);
            *bit_ofs += 16;
    }
    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 4, false, &msg->magnetic_field_covariance_len);
        *bit_ofs += 4;
    }
    for (size_t i=0; i < msg->magnetic_field_covariance_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->magnetic_field_covariance[i]);
            *bit_ofs += 16;
    }
}
