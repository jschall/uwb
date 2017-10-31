#include <canard/uavcan.equipment.actuator.Command.h>
#include <string.h>


uint32_t encode_uavcan_equipment_actuator_Command(struct uavcan_equipment_actuator_Command_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_ACTUATOR_COMMAND_MAX_PACK_SIZE);
    _encode_uavcan_equipment_actuator_Command(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_actuator_Command(const CanardRxTransfer* transfer, struct uavcan_equipment_actuator_Command_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_actuator_Command(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_actuator_Command(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_actuator_Command_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->actuator_id);
    *bit_ofs += 8;
    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->command_type);
    *bit_ofs += 8;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->command_value);
    *bit_ofs += 16;
}

void _decode_uavcan_equipment_actuator_Command(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_actuator_Command_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->actuator_id);
    *bit_ofs += 8;
    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->command_type);
    *bit_ofs += 8;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->command_value);
    *bit_ofs += 16;
}
