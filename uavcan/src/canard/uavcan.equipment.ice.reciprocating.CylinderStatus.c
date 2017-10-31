#include <canard/uavcan.equipment.ice.reciprocating.CylinderStatus.h>
#include <string.h>


uint32_t encode_uavcan_equipment_ice_reciprocating_CylinderStatus(struct uavcan_equipment_ice_reciprocating_CylinderStatus_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_ICE_RECIPROCATING_CYLINDERSTATUS_MAX_PACK_SIZE);
    _encode_uavcan_equipment_ice_reciprocating_CylinderStatus(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_ice_reciprocating_CylinderStatus(const CanardRxTransfer* transfer, struct uavcan_equipment_ice_reciprocating_CylinderStatus_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_ice_reciprocating_CylinderStatus(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_ice_reciprocating_CylinderStatus(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_ice_reciprocating_CylinderStatus_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->ignition_timing_deg);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->injection_time_ms);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->cylinder_head_temperature);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->exhaust_gas_temperature);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->lambda_coefficient);
    *bit_ofs += 16;
}

void _decode_uavcan_equipment_ice_reciprocating_CylinderStatus(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_ice_reciprocating_CylinderStatus_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->ignition_timing_deg);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->injection_time_ms);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->cylinder_head_temperature);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->exhaust_gas_temperature);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->lambda_coefficient);
    *bit_ofs += 16;
}
