#include <canard/uavcan.equipment.actuator.Status.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_equipment_actuator_Status(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_equipment_actuator_Status(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_equipment_actuator_Status_descriptor = {
    UAVCAN_EQUIPMENT_ACTUATOR_STATUS_DT_SIG,
    UAVCAN_EQUIPMENT_ACTUATOR_STATUS_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_equipment_actuator_Status_s),
    UAVCAN_EQUIPMENT_ACTUATOR_STATUS_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_equipment_actuator_Status(struct uavcan_equipment_actuator_Status_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_ACTUATOR_STATUS_MAX_PACK_SIZE);
    _encode_uavcan_equipment_actuator_Status(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_actuator_Status(const CanardRxTransfer* transfer, struct uavcan_equipment_actuator_Status_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_actuator_Status(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_actuator_Status(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_actuator_Status_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->actuator_id);
    *bit_ofs += 8;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->position);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->force);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->speed);
    *bit_ofs += 16;
    *bit_ofs += 1;
    canardEncodeScalar(buffer, *bit_ofs, 7, &msg->power_rating_pct);
    *bit_ofs += 7;
}

void _decode_uavcan_equipment_actuator_Status(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_actuator_Status_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->actuator_id);
    *bit_ofs += 8;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->position);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->force);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->speed);
    *bit_ofs += 16;
    *bit_ofs += 1;
    canardDecodeScalar(transfer, *bit_ofs, 7, false, &msg->power_rating_pct);
    *bit_ofs += 7;
}
