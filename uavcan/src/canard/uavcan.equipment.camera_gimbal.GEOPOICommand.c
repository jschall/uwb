#include <canard/uavcan.equipment.camera_gimbal.GEOPOICommand.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_equipment_camera_gimbal_GEOPOICommand(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_equipment_camera_gimbal_GEOPOICommand(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_equipment_camera_gimbal_GEOPOICommand_descriptor = {
    UAVCAN_EQUIPMENT_CAMERA_GIMBAL_GEOPOICOMMAND_DT_SIG,
    UAVCAN_EQUIPMENT_CAMERA_GIMBAL_GEOPOICOMMAND_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_equipment_camera_gimbal_GEOPOICommand_s),
    UAVCAN_EQUIPMENT_CAMERA_GIMBAL_GEOPOICOMMAND_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_equipment_camera_gimbal_GEOPOICommand(struct uavcan_equipment_camera_gimbal_GEOPOICommand_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_CAMERA_GIMBAL_GEOPOICOMMAND_MAX_PACK_SIZE);
    _encode_uavcan_equipment_camera_gimbal_GEOPOICommand(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_camera_gimbal_GEOPOICommand(const CanardRxTransfer* transfer, struct uavcan_equipment_camera_gimbal_GEOPOICommand_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_camera_gimbal_GEOPOICommand(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_camera_gimbal_GEOPOICommand(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_camera_gimbal_GEOPOICommand_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->gimbal_id);
    *bit_ofs += 8;
    _encode_uavcan_equipment_camera_gimbal_Mode(buffer, bit_ofs, &msg->mode, false);
    canardEncodeScalar(buffer, *bit_ofs, 32, &msg->longitude_deg_1e7);
    *bit_ofs += 32;
    canardEncodeScalar(buffer, *bit_ofs, 32, &msg->latitude_deg_1e7);
    *bit_ofs += 32;
    canardEncodeScalar(buffer, *bit_ofs, 22, &msg->height_cm);
    *bit_ofs += 22;
    canardEncodeScalar(buffer, *bit_ofs, 2, &msg->height_reference);
    *bit_ofs += 2;
}

void _decode_uavcan_equipment_camera_gimbal_GEOPOICommand(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_camera_gimbal_GEOPOICommand_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->gimbal_id);
    *bit_ofs += 8;
    _decode_uavcan_equipment_camera_gimbal_Mode(transfer, bit_ofs, &msg->mode, false);
    canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->longitude_deg_1e7);
    *bit_ofs += 32;
    canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->latitude_deg_1e7);
    *bit_ofs += 32;
    canardDecodeScalar(transfer, *bit_ofs, 22, true, &msg->height_cm);
    *bit_ofs += 22;
    canardDecodeScalar(transfer, *bit_ofs, 2, false, &msg->height_reference);
    *bit_ofs += 2;
}
