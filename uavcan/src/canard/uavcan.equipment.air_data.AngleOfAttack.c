#include <canard/uavcan.equipment.air_data.AngleOfAttack.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_equipment_air_data_AngleOfAttack(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_equipment_air_data_AngleOfAttack(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_equipment_air_data_AngleOfAttack_descriptor = {
    UAVCAN_EQUIPMENT_AIR_DATA_ANGLEOFATTACK_DT_SIG,
    UAVCAN_EQUIPMENT_AIR_DATA_ANGLEOFATTACK_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_equipment_air_data_AngleOfAttack_s),
    UAVCAN_EQUIPMENT_AIR_DATA_ANGLEOFATTACK_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_equipment_air_data_AngleOfAttack(struct uavcan_equipment_air_data_AngleOfAttack_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_AIR_DATA_ANGLEOFATTACK_MAX_PACK_SIZE);
    _encode_uavcan_equipment_air_data_AngleOfAttack(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_air_data_AngleOfAttack(const CanardRxTransfer* transfer, struct uavcan_equipment_air_data_AngleOfAttack_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_air_data_AngleOfAttack(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_air_data_AngleOfAttack(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_air_data_AngleOfAttack_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->sensor_id);
    *bit_ofs += 8;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->aoa);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->aoa_variance);
    *bit_ofs += 16;
}

void _decode_uavcan_equipment_air_data_AngleOfAttack(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_air_data_AngleOfAttack_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->sensor_id);
    *bit_ofs += 8;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->aoa);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->aoa_variance);
    *bit_ofs += 16;
}
