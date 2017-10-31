#include <canard/uavcan.equipment.camera_gimbal.AngularCommand.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_equipment_camera_gimbal_AngularCommand(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_equipment_camera_gimbal_AngularCommand(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_equipment_camera_gimbal_AngularCommand_descriptor = {
    UAVCAN_EQUIPMENT_CAMERA_GIMBAL_ANGULARCOMMAND_DT_SIG,
    UAVCAN_EQUIPMENT_CAMERA_GIMBAL_ANGULARCOMMAND_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_equipment_camera_gimbal_AngularCommand_s),
    UAVCAN_EQUIPMENT_CAMERA_GIMBAL_ANGULARCOMMAND_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_equipment_camera_gimbal_AngularCommand(struct uavcan_equipment_camera_gimbal_AngularCommand_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_CAMERA_GIMBAL_ANGULARCOMMAND_MAX_PACK_SIZE);
    _encode_uavcan_equipment_camera_gimbal_AngularCommand(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_camera_gimbal_AngularCommand(const CanardRxTransfer* transfer, struct uavcan_equipment_camera_gimbal_AngularCommand_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_camera_gimbal_AngularCommand(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_camera_gimbal_AngularCommand(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_camera_gimbal_AngularCommand_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->gimbal_id);
    *bit_ofs += 8;
    _encode_uavcan_equipment_camera_gimbal_Mode(buffer, bit_ofs, &msg->mode, false);
    for (size_t i=0; i < 4; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 16, &msg->quaternion_xyzw[i]);
            *bit_ofs += 16;
    }
}

void _decode_uavcan_equipment_camera_gimbal_AngularCommand(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_camera_gimbal_AngularCommand_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->gimbal_id);
    *bit_ofs += 8;
    _decode_uavcan_equipment_camera_gimbal_Mode(transfer, bit_ofs, &msg->mode, false);
    for (size_t i=0; i < 4; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->quaternion_xyzw[i]);
            *bit_ofs += 16;
    }
}
