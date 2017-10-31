#include <canard/uavcan.equipment.hardpoint.Command.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_equipment_hardpoint_Command(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_equipment_hardpoint_Command(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_equipment_hardpoint_Command_descriptor = {
    UAVCAN_EQUIPMENT_HARDPOINT_COMMAND_DT_SIG,
    UAVCAN_EQUIPMENT_HARDPOINT_COMMAND_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_equipment_hardpoint_Command_s),
    UAVCAN_EQUIPMENT_HARDPOINT_COMMAND_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_equipment_hardpoint_Command(struct uavcan_equipment_hardpoint_Command_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_HARDPOINT_COMMAND_MAX_PACK_SIZE);
    _encode_uavcan_equipment_hardpoint_Command(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_hardpoint_Command(const CanardRxTransfer* transfer, struct uavcan_equipment_hardpoint_Command_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_hardpoint_Command(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_hardpoint_Command(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_hardpoint_Command_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->hardpoint_id);
    *bit_ofs += 8;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->command);
    *bit_ofs += 16;
}

void _decode_uavcan_equipment_hardpoint_Command(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_hardpoint_Command_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->hardpoint_id);
    *bit_ofs += 8;
    canardDecodeScalar(transfer, *bit_ofs, 16, false, &msg->command);
    *bit_ofs += 16;
}
