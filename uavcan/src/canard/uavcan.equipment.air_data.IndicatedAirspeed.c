#include <canard/uavcan.equipment.air_data.IndicatedAirspeed.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_equipment_air_data_IndicatedAirspeed(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_equipment_air_data_IndicatedAirspeed(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_equipment_air_data_IndicatedAirspeed_descriptor = {
    UAVCAN_EQUIPMENT_AIR_DATA_INDICATEDAIRSPEED_DT_SIG,
    UAVCAN_EQUIPMENT_AIR_DATA_INDICATEDAIRSPEED_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_equipment_air_data_IndicatedAirspeed_s),
    UAVCAN_EQUIPMENT_AIR_DATA_INDICATEDAIRSPEED_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_equipment_air_data_IndicatedAirspeed(struct uavcan_equipment_air_data_IndicatedAirspeed_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_AIR_DATA_INDICATEDAIRSPEED_MAX_PACK_SIZE);
    _encode_uavcan_equipment_air_data_IndicatedAirspeed(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_air_data_IndicatedAirspeed(const CanardRxTransfer* transfer, struct uavcan_equipment_air_data_IndicatedAirspeed_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_air_data_IndicatedAirspeed(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_air_data_IndicatedAirspeed(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_air_data_IndicatedAirspeed_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->indicated_airspeed);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->indicated_airspeed_variance);
    *bit_ofs += 16;
}

void _decode_uavcan_equipment_air_data_IndicatedAirspeed(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_air_data_IndicatedAirspeed_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->indicated_airspeed);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->indicated_airspeed_variance);
    *bit_ofs += 16;
}
