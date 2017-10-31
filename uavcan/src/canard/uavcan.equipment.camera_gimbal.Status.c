#include <canard/uavcan.equipment.camera_gimbal.Status.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_equipment_camera_gimbal_Status(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_equipment_camera_gimbal_Status(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_equipment_camera_gimbal_Status_descriptor = {
    UAVCAN_EQUIPMENT_CAMERA_GIMBAL_STATUS_DT_SIG,
    UAVCAN_EQUIPMENT_CAMERA_GIMBAL_STATUS_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_equipment_camera_gimbal_Status_s),
    UAVCAN_EQUIPMENT_CAMERA_GIMBAL_STATUS_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_equipment_camera_gimbal_Status(struct uavcan_equipment_camera_gimbal_Status_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_CAMERA_GIMBAL_STATUS_MAX_PACK_SIZE);
    _encode_uavcan_equipment_camera_gimbal_Status(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_camera_gimbal_Status(const CanardRxTransfer* transfer, struct uavcan_equipment_camera_gimbal_Status_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_camera_gimbal_Status(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_camera_gimbal_Status(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_camera_gimbal_Status_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->gimbal_id);
    *bit_ofs += 8;
    _encode_uavcan_equipment_camera_gimbal_Mode(buffer, bit_ofs, &msg->mode, false);
    for (size_t i=0; i < 4; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 16, &msg->camera_orientation_in_body_frame_xyzw[i]);
            *bit_ofs += 16;
    }
    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 4, &msg->camera_orientation_in_body_frame_covariance_len);
        *bit_ofs += 4;
    }
    for (size_t i=0; i < msg->camera_orientation_in_body_frame_covariance_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 16, &msg->camera_orientation_in_body_frame_covariance[i]);
            *bit_ofs += 16;
    }
}

void _decode_uavcan_equipment_camera_gimbal_Status(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_camera_gimbal_Status_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->gimbal_id);
    *bit_ofs += 8;
    _decode_uavcan_equipment_camera_gimbal_Mode(transfer, bit_ofs, &msg->mode, false);
    for (size_t i=0; i < 4; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->camera_orientation_in_body_frame_xyzw[i]);
            *bit_ofs += 16;
    }
    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 4, false, &msg->camera_orientation_in_body_frame_covariance_len);
        *bit_ofs += 4;
    }
    for (size_t i=0; i < msg->camera_orientation_in_body_frame_covariance_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->camera_orientation_in_body_frame_covariance[i]);
            *bit_ofs += 16;
    }
}
