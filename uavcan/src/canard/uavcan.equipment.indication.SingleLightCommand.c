#include <canard/uavcan.equipment.indication.SingleLightCommand.h>
#include <string.h>


uint32_t encode_uavcan_equipment_indication_SingleLightCommand(struct uavcan_equipment_indication_SingleLightCommand_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_INDICATION_SINGLELIGHTCOMMAND_MAX_PACK_SIZE);
    _encode_uavcan_equipment_indication_SingleLightCommand(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_indication_SingleLightCommand(const CanardRxTransfer* transfer, struct uavcan_equipment_indication_SingleLightCommand_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_indication_SingleLightCommand(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_indication_SingleLightCommand(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_indication_SingleLightCommand_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->light_id);
    *bit_ofs += 8;
    _encode_uavcan_equipment_indication_RGB565(buffer, bit_ofs, &msg->color, tao);
}

void _decode_uavcan_equipment_indication_SingleLightCommand(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_indication_SingleLightCommand_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->light_id);
    *bit_ofs += 8;
    _decode_uavcan_equipment_indication_RGB565(transfer, bit_ofs, &msg->color, tao);
}
