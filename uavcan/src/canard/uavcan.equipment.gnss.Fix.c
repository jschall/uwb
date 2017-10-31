#include <canard/uavcan.equipment.gnss.Fix.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_equipment_gnss_Fix(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_equipment_gnss_Fix(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_equipment_gnss_Fix_descriptor = {
    UAVCAN_EQUIPMENT_GNSS_FIX_DT_SIG,
    UAVCAN_EQUIPMENT_GNSS_FIX_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_equipment_gnss_Fix_s),
    UAVCAN_EQUIPMENT_GNSS_FIX_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_equipment_gnss_Fix(struct uavcan_equipment_gnss_Fix_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_GNSS_FIX_MAX_PACK_SIZE);
    _encode_uavcan_equipment_gnss_Fix(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_gnss_Fix(const CanardRxTransfer* transfer, struct uavcan_equipment_gnss_Fix_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_gnss_Fix(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_gnss_Fix(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_gnss_Fix_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    _encode_uavcan_Timestamp(buffer, bit_ofs, &msg->timestamp, false);
    _encode_uavcan_Timestamp(buffer, bit_ofs, &msg->gnss_timestamp, false);
    canardEncodeScalar(buffer, *bit_ofs, 3, &msg->gnss_time_standard);
    *bit_ofs += 3;
    *bit_ofs += 5;
    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->num_leap_seconds);
    *bit_ofs += 8;
    canardEncodeScalar(buffer, *bit_ofs, 37, &msg->longitude_deg_1e8);
    *bit_ofs += 37;
    canardEncodeScalar(buffer, *bit_ofs, 37, &msg->latitude_deg_1e8);
    *bit_ofs += 37;
    canardEncodeScalar(buffer, *bit_ofs, 27, &msg->height_ellipsoid_mm);
    *bit_ofs += 27;
    canardEncodeScalar(buffer, *bit_ofs, 27, &msg->height_msl_mm);
    *bit_ofs += 27;
    for (size_t i=0; i < 3; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 16, &msg->ned_velocity[i]);
            *bit_ofs += 16;
    }
    canardEncodeScalar(buffer, *bit_ofs, 6, &msg->sats_used);
    *bit_ofs += 6;
    canardEncodeScalar(buffer, *bit_ofs, 2, &msg->status);
    *bit_ofs += 2;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->pdop);
    *bit_ofs += 16;
    *bit_ofs += 4;
    canardEncodeScalar(buffer, *bit_ofs, 4, &msg->position_covariance_len);
    *bit_ofs += 4;
    for (size_t i=0; i < msg->position_covariance_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 16, &msg->position_covariance[i]);
            *bit_ofs += 16;
    }
    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 4, &msg->velocity_covariance_len);
        *bit_ofs += 4;
    }
    for (size_t i=0; i < msg->velocity_covariance_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 16, &msg->velocity_covariance[i]);
            *bit_ofs += 16;
    }
}

void _decode_uavcan_equipment_gnss_Fix(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_gnss_Fix_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    _decode_uavcan_Timestamp(transfer, bit_ofs, &msg->timestamp, false);
    _decode_uavcan_Timestamp(transfer, bit_ofs, &msg->gnss_timestamp, false);
    canardDecodeScalar(transfer, *bit_ofs, 3, false, &msg->gnss_time_standard);
    *bit_ofs += 3;
    *bit_ofs += 5;
    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->num_leap_seconds);
    *bit_ofs += 8;
    canardDecodeScalar(transfer, *bit_ofs, 37, true, &msg->longitude_deg_1e8);
    *bit_ofs += 37;
    canardDecodeScalar(transfer, *bit_ofs, 37, true, &msg->latitude_deg_1e8);
    *bit_ofs += 37;
    canardDecodeScalar(transfer, *bit_ofs, 27, true, &msg->height_ellipsoid_mm);
    *bit_ofs += 27;
    canardDecodeScalar(transfer, *bit_ofs, 27, true, &msg->height_msl_mm);
    *bit_ofs += 27;
    for (size_t i=0; i < 3; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->ned_velocity[i]);
            *bit_ofs += 16;
    }
    canardDecodeScalar(transfer, *bit_ofs, 6, false, &msg->sats_used);
    *bit_ofs += 6;
    canardDecodeScalar(transfer, *bit_ofs, 2, false, &msg->status);
    *bit_ofs += 2;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->pdop);
    *bit_ofs += 16;
    *bit_ofs += 4;
    canardDecodeScalar(transfer, *bit_ofs, 4, false, &msg->position_covariance_len);
    *bit_ofs += 4;
    for (size_t i=0; i < msg->position_covariance_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->position_covariance[i]);
            *bit_ofs += 16;
    }
    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 4, false, &msg->velocity_covariance_len);
        *bit_ofs += 4;
    }
    for (size_t i=0; i < msg->velocity_covariance_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->velocity_covariance[i]);
            *bit_ofs += 16;
    }
}
