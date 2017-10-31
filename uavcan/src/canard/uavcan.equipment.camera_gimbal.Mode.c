#include <canard/uavcan.equipment.camera_gimbal.Mode.h>
#include <string.h>


uint32_t encode_uavcan_equipment_camera_gimbal_Mode(struct uavcan_equipment_camera_gimbal_Mode_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_CAMERA_GIMBAL_MODE_MAX_PACK_SIZE);
    _encode_uavcan_equipment_camera_gimbal_Mode(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_camera_gimbal_Mode(const CanardRxTransfer* transfer, struct uavcan_equipment_camera_gimbal_Mode_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_camera_gimbal_Mode(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_camera_gimbal_Mode(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_camera_gimbal_Mode_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->command_mode);
    *bit_ofs += 8;
}

void _decode_uavcan_equipment_camera_gimbal_Mode(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_camera_gimbal_Mode_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->command_mode);
    *bit_ofs += 8;
}
