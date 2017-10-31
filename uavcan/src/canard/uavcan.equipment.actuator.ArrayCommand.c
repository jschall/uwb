#include <canard/uavcan.equipment.actuator.ArrayCommand.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_equipment_actuator_ArrayCommand(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_equipment_actuator_ArrayCommand(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_equipment_actuator_ArrayCommand_descriptor = {
    UAVCAN_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_DT_SIG,
    UAVCAN_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_equipment_actuator_ArrayCommand_s),
    UAVCAN_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_equipment_actuator_ArrayCommand(struct uavcan_equipment_actuator_ArrayCommand_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_MAX_PACK_SIZE);
    _encode_uavcan_equipment_actuator_ArrayCommand(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_actuator_ArrayCommand(const CanardRxTransfer* transfer, struct uavcan_equipment_actuator_ArrayCommand_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_actuator_ArrayCommand(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_actuator_ArrayCommand(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_actuator_ArrayCommand_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 4, &msg->commands_len);
        *bit_ofs += 4;
    }
    for (size_t i=0; i < msg->commands_len; i++) {
            _encode_uavcan_equipment_actuator_Command(buffer, bit_ofs, &msg->commands[i], false);
    }
}

void _decode_uavcan_equipment_actuator_ArrayCommand(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_actuator_ArrayCommand_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 4, false, &msg->commands_len);
        *bit_ofs += 4;
    }
    for (size_t i=0; i < msg->commands_len; i++) {
            _decode_uavcan_equipment_actuator_Command(transfer, bit_ofs, &msg->commands[i], false);
    }
}
