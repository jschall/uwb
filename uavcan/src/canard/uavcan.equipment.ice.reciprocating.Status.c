#include <canard/uavcan.equipment.ice.reciprocating.Status.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_equipment_ice_reciprocating_Status(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_equipment_ice_reciprocating_Status(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_equipment_ice_reciprocating_Status_descriptor = {
    UAVCAN_EQUIPMENT_ICE_RECIPROCATING_STATUS_DT_SIG,
    UAVCAN_EQUIPMENT_ICE_RECIPROCATING_STATUS_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_equipment_ice_reciprocating_Status_s),
    UAVCAN_EQUIPMENT_ICE_RECIPROCATING_STATUS_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_equipment_ice_reciprocating_Status(struct uavcan_equipment_ice_reciprocating_Status_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_ICE_RECIPROCATING_STATUS_MAX_PACK_SIZE);
    _encode_uavcan_equipment_ice_reciprocating_Status(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_ice_reciprocating_Status(const CanardRxTransfer* transfer, struct uavcan_equipment_ice_reciprocating_Status_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_ice_reciprocating_Status(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_ice_reciprocating_Status(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_ice_reciprocating_Status_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 2, &msg->state);
    *bit_ofs += 2;
    canardEncodeScalar(buffer, *bit_ofs, 30, &msg->flags);
    *bit_ofs += 30;
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 7, &msg->engine_load_percent);
    *bit_ofs += 7;
    canardEncodeScalar(buffer, *bit_ofs, 17, &msg->engine_speed_rpm);
    *bit_ofs += 17;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->spark_dwell_time_ms);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->atmospheric_pressure_kpa);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->intake_manifold_pressure_kpa);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->intake_manifold_temperature);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->coolant_temperature);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->oil_pressure);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->oil_temperature);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->fuel_pressure);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 32, &msg->fuel_consumption_rate_cm3pm);
    *bit_ofs += 32;
    canardEncodeScalar(buffer, *bit_ofs, 32, &msg->estimated_consumed_fuel_volume_cm3);
    *bit_ofs += 32;
    canardEncodeScalar(buffer, *bit_ofs, 7, &msg->throttle_position_percent);
    *bit_ofs += 7;
    canardEncodeScalar(buffer, *bit_ofs, 6, &msg->ecu_index);
    *bit_ofs += 6;
    canardEncodeScalar(buffer, *bit_ofs, 3, &msg->spark_plug_usage);
    *bit_ofs += 3;
    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 5, &msg->cylinder_status_len);
        *bit_ofs += 5;
    }
    for (size_t i=0; i < msg->cylinder_status_len; i++) {
            _encode_uavcan_equipment_ice_reciprocating_CylinderStatus(buffer, bit_ofs, &msg->cylinder_status[i], false);
    }
}

void _decode_uavcan_equipment_ice_reciprocating_Status(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_ice_reciprocating_Status_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 2, false, &msg->state);
    *bit_ofs += 2;
    canardDecodeScalar(transfer, *bit_ofs, 30, false, &msg->flags);
    *bit_ofs += 30;
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 7, false, &msg->engine_load_percent);
    *bit_ofs += 7;
    canardDecodeScalar(transfer, *bit_ofs, 17, false, &msg->engine_speed_rpm);
    *bit_ofs += 17;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->spark_dwell_time_ms);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->atmospheric_pressure_kpa);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->intake_manifold_pressure_kpa);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->intake_manifold_temperature);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->coolant_temperature);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->oil_pressure);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->oil_temperature);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->fuel_pressure);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->fuel_consumption_rate_cm3pm);
    *bit_ofs += 32;
    canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->estimated_consumed_fuel_volume_cm3);
    *bit_ofs += 32;
    canardDecodeScalar(transfer, *bit_ofs, 7, false, &msg->throttle_position_percent);
    *bit_ofs += 7;
    canardDecodeScalar(transfer, *bit_ofs, 6, false, &msg->ecu_index);
    *bit_ofs += 6;
    canardDecodeScalar(transfer, *bit_ofs, 3, false, &msg->spark_plug_usage);
    *bit_ofs += 3;
    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 5, false, &msg->cylinder_status_len);
        *bit_ofs += 5;
    }
    for (size_t i=0; i < msg->cylinder_status_len; i++) {
            _decode_uavcan_equipment_ice_reciprocating_CylinderStatus(transfer, bit_ofs, &msg->cylinder_status[i], false);
    }
}
