#include <canard/uavcan.equipment.power.PrimaryPowerSupplyStatus.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_equipment_power_PrimaryPowerSupplyStatus(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_equipment_power_PrimaryPowerSupplyStatus(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_equipment_power_PrimaryPowerSupplyStatus_descriptor = {
    UAVCAN_EQUIPMENT_POWER_PRIMARYPOWERSUPPLYSTATUS_DT_SIG,
    UAVCAN_EQUIPMENT_POWER_PRIMARYPOWERSUPPLYSTATUS_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_equipment_power_PrimaryPowerSupplyStatus_s),
    UAVCAN_EQUIPMENT_POWER_PRIMARYPOWERSUPPLYSTATUS_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_equipment_power_PrimaryPowerSupplyStatus(struct uavcan_equipment_power_PrimaryPowerSupplyStatus_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_POWER_PRIMARYPOWERSUPPLYSTATUS_MAX_PACK_SIZE);
    _encode_uavcan_equipment_power_PrimaryPowerSupplyStatus(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_power_PrimaryPowerSupplyStatus(const CanardRxTransfer* transfer, struct uavcan_equipment_power_PrimaryPowerSupplyStatus_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_power_PrimaryPowerSupplyStatus(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_power_PrimaryPowerSupplyStatus(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_power_PrimaryPowerSupplyStatus_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->hours_to_empty_at_10sec_avg_power);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->hours_to_empty_at_10sec_avg_power_variance);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 1, &msg->external_power_available);
    *bit_ofs += 1;
    canardEncodeScalar(buffer, *bit_ofs, 7, &msg->remaining_energy_pct);
    *bit_ofs += 7;
    canardEncodeScalar(buffer, *bit_ofs, 7, &msg->remaining_energy_pct_stdev);
    *bit_ofs += 7;
}

void _decode_uavcan_equipment_power_PrimaryPowerSupplyStatus(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_power_PrimaryPowerSupplyStatus_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->hours_to_empty_at_10sec_avg_power);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->hours_to_empty_at_10sec_avg_power_variance);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 1, false, &msg->external_power_available);
    *bit_ofs += 1;
    canardDecodeScalar(transfer, *bit_ofs, 7, false, &msg->remaining_energy_pct);
    *bit_ofs += 7;
    canardDecodeScalar(transfer, *bit_ofs, 7, false, &msg->remaining_energy_pct_stdev);
    *bit_ofs += 7;
}
