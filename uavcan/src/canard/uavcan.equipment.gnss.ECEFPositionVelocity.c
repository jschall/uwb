#include <canard/uavcan.equipment.gnss.ECEFPositionVelocity.h>
#include <string.h>


uint32_t encode_uavcan_equipment_gnss_ECEFPositionVelocity(struct uavcan_equipment_gnss_ECEFPositionVelocity_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_GNSS_ECEFPOSITIONVELOCITY_MAX_PACK_SIZE);
    _encode_uavcan_equipment_gnss_ECEFPositionVelocity(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_gnss_ECEFPositionVelocity(const CanardRxTransfer* transfer, struct uavcan_equipment_gnss_ECEFPositionVelocity_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_gnss_ECEFPositionVelocity(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_gnss_ECEFPositionVelocity(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_gnss_ECEFPositionVelocity_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    for (size_t i=0; i < 3; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 32, &msg->velocity_xyz[i]);
            *bit_ofs += 32;
    }
    for (size_t i=0; i < 3; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 36, &msg->position_xyz_mm[i]);
            *bit_ofs += 36;
    }
    *bit_ofs += 6;
    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 6, &msg->covariance_len);
        *bit_ofs += 6;
    }
    for (size_t i=0; i < msg->covariance_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 16, &msg->covariance[i]);
            *bit_ofs += 16;
    }
}

void _decode_uavcan_equipment_gnss_ECEFPositionVelocity(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_gnss_ECEFPositionVelocity_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    for (size_t i=0; i < 3; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->velocity_xyz[i]);
            *bit_ofs += 32;
    }
    for (size_t i=0; i < 3; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 36, true, &msg->position_xyz_mm[i]);
            *bit_ofs += 36;
    }
    *bit_ofs += 6;
    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 6, false, &msg->covariance_len);
        *bit_ofs += 6;
    }
    for (size_t i=0; i < msg->covariance_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->covariance[i]);
            *bit_ofs += 16;
    }
}
