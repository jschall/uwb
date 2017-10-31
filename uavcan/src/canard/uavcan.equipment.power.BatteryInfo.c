#include <canard/uavcan.equipment.power.BatteryInfo.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_equipment_power_BatteryInfo(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_equipment_power_BatteryInfo(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_equipment_power_BatteryInfo_descriptor = {
    UAVCAN_EQUIPMENT_POWER_BATTERYINFO_DT_SIG,
    UAVCAN_EQUIPMENT_POWER_BATTERYINFO_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_equipment_power_BatteryInfo_s),
    UAVCAN_EQUIPMENT_POWER_BATTERYINFO_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_equipment_power_BatteryInfo(struct uavcan_equipment_power_BatteryInfo_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_POWER_BATTERYINFO_MAX_PACK_SIZE);
    _encode_uavcan_equipment_power_BatteryInfo(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_power_BatteryInfo(const CanardRxTransfer* transfer, struct uavcan_equipment_power_BatteryInfo_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_power_BatteryInfo(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_power_BatteryInfo(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_power_BatteryInfo_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->temperature);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->voltage);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->current);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->average_power_10sec);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->remaining_capacity_wh);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->full_charge_capacity_wh);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->hours_to_full_charge);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 11, &msg->status_flags);
    *bit_ofs += 11;
    canardEncodeScalar(buffer, *bit_ofs, 7, &msg->state_of_health_pct);
    *bit_ofs += 7;
    canardEncodeScalar(buffer, *bit_ofs, 7, &msg->state_of_charge_pct);
    *bit_ofs += 7;
    canardEncodeScalar(buffer, *bit_ofs, 7, &msg->state_of_charge_pct_stdev);
    *bit_ofs += 7;
    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->battery_id);
    *bit_ofs += 8;
    canardEncodeScalar(buffer, *bit_ofs, 32, &msg->model_instance_id);
    *bit_ofs += 32;
    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 5, &msg->model_name_len);
        *bit_ofs += 5;
    }
    for (size_t i=0; i < msg->model_name_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 8, &msg->model_name[i]);
            *bit_ofs += 8;
    }
}

void _decode_uavcan_equipment_power_BatteryInfo(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_power_BatteryInfo_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->temperature);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->voltage);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->current);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->average_power_10sec);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->remaining_capacity_wh);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->full_charge_capacity_wh);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->hours_to_full_charge);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 11, false, &msg->status_flags);
    *bit_ofs += 11;
    canardDecodeScalar(transfer, *bit_ofs, 7, false, &msg->state_of_health_pct);
    *bit_ofs += 7;
    canardDecodeScalar(transfer, *bit_ofs, 7, false, &msg->state_of_charge_pct);
    *bit_ofs += 7;
    canardDecodeScalar(transfer, *bit_ofs, 7, false, &msg->state_of_charge_pct_stdev);
    *bit_ofs += 7;
    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->battery_id);
    *bit_ofs += 8;
    canardDecodeScalar(transfer, *bit_ofs, 32, false, &msg->model_instance_id);
    *bit_ofs += 32;
    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 5, false, &msg->model_name_len);
        *bit_ofs += 5;
    }
    for (size_t i=0; i < msg->model_name_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->model_name[i]);
            *bit_ofs += 8;
    }
}
