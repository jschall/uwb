#include <canard/uavcan.equipment.ice.FuelTankStatus.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_equipment_ice_FuelTankStatus(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_equipment_ice_FuelTankStatus(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_equipment_ice_FuelTankStatus_descriptor = {
    UAVCAN_EQUIPMENT_ICE_FUELTANKSTATUS_DT_SIG,
    UAVCAN_EQUIPMENT_ICE_FUELTANKSTATUS_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_equipment_ice_FuelTankStatus_s),
    UAVCAN_EQUIPMENT_ICE_FUELTANKSTATUS_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_equipment_ice_FuelTankStatus(struct uavcan_equipment_ice_FuelTankStatus_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_ICE_FUELTANKSTATUS_MAX_PACK_SIZE);
    _encode_uavcan_equipment_ice_FuelTankStatus(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_ice_FuelTankStatus(const CanardRxTransfer* transfer, struct uavcan_equipment_ice_FuelTankStatus_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_ice_FuelTankStatus(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_ice_FuelTankStatus(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_ice_FuelTankStatus_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    *bit_ofs += 9;
    canardEncodeScalar(buffer, *bit_ofs, 7, &msg->available_fuel_volume_percent);
    *bit_ofs += 7;
    canardEncodeScalar(buffer, *bit_ofs, 32, &msg->available_fuel_volume_cm3);
    *bit_ofs += 32;
    canardEncodeScalar(buffer, *bit_ofs, 32, &msg->fuel_consumption_rate_cm3pm);
    *bit_ofs += 32;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->fuel_temperature);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->fuel_tank_id);
    *bit_ofs += 8;
}

void _decode_uavcan_equipment_ice_FuelTankStatus(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_ice_FuelTankStatus_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    *bit_ofs += 9;
    canardDecodeScalar(transfer, *bit_ofs, 7, false, &msg->available_fuel_volume_percent);
    *bit_ofs += 7;
    canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->available_fuel_volume_cm3);
    *bit_ofs += 32;
    canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->fuel_consumption_rate_cm3pm);
    *bit_ofs += 32;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->fuel_temperature);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->fuel_tank_id);
    *bit_ofs += 8;
}
