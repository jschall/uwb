#include <canard/uavcan.equipment.esc.RawCommand.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_equipment_esc_RawCommand(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_equipment_esc_RawCommand(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_equipment_esc_RawCommand_descriptor = {
    UAVCAN_EQUIPMENT_ESC_RAWCOMMAND_DT_SIG,
    UAVCAN_EQUIPMENT_ESC_RAWCOMMAND_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_equipment_esc_RawCommand_s),
    UAVCAN_EQUIPMENT_ESC_RAWCOMMAND_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_equipment_esc_RawCommand(struct uavcan_equipment_esc_RawCommand_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_ESC_RAWCOMMAND_MAX_PACK_SIZE);
    _encode_uavcan_equipment_esc_RawCommand(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_esc_RawCommand(const CanardRxTransfer* transfer, struct uavcan_equipment_esc_RawCommand_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_esc_RawCommand(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_esc_RawCommand(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_esc_RawCommand_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 5, &msg->cmd_len);
        *bit_ofs += 5;
    }
    for (size_t i=0; i < msg->cmd_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 14, &msg->cmd[i]);
            *bit_ofs += 14;
    }
}

void _decode_uavcan_equipment_esc_RawCommand(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_esc_RawCommand_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 5, false, &msg->cmd_len);
        *bit_ofs += 5;
    }
    for (size_t i=0; i < msg->cmd_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 14, true, &msg->cmd[i]);
            *bit_ofs += 14;
    }
}
