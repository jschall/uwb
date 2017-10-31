#include <canard/uavcan.equipment.air_data.RawAirData.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_equipment_air_data_RawAirData(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_equipment_air_data_RawAirData(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_equipment_air_data_RawAirData_descriptor = {
    UAVCAN_EQUIPMENT_AIR_DATA_RAWAIRDATA_DT_SIG,
    UAVCAN_EQUIPMENT_AIR_DATA_RAWAIRDATA_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_equipment_air_data_RawAirData_s),
    UAVCAN_EQUIPMENT_AIR_DATA_RAWAIRDATA_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_equipment_air_data_RawAirData(struct uavcan_equipment_air_data_RawAirData_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_AIR_DATA_RAWAIRDATA_MAX_PACK_SIZE);
    _encode_uavcan_equipment_air_data_RawAirData(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_air_data_RawAirData(const CanardRxTransfer* transfer, struct uavcan_equipment_air_data_RawAirData_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_air_data_RawAirData(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_air_data_RawAirData(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_air_data_RawAirData_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->flags);
    *bit_ofs += 8;
    canardEncodeScalar(buffer, *bit_ofs, 32, &msg->static_pressure);
    *bit_ofs += 32;
    canardEncodeScalar(buffer, *bit_ofs, 32, &msg->differential_pressure);
    *bit_ofs += 32;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->static_pressure_sensor_temperature);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->differential_pressure_sensor_temperature);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->static_air_temperature);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->pitot_temperature);
    *bit_ofs += 16;
    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 5, &msg->covariance_len);
        *bit_ofs += 5;
    }
    for (size_t i=0; i < msg->covariance_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 16, &msg->covariance[i]);
            *bit_ofs += 16;
    }
}

void _decode_uavcan_equipment_air_data_RawAirData(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_air_data_RawAirData_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->flags);
    *bit_ofs += 8;
    canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->static_pressure);
    *bit_ofs += 32;
    canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->differential_pressure);
    *bit_ofs += 32;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->static_pressure_sensor_temperature);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->differential_pressure_sensor_temperature);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->static_air_temperature);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->pitot_temperature);
    *bit_ofs += 16;
    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 5, false, &msg->covariance_len);
        *bit_ofs += 5;
    }
    for (size_t i=0; i < msg->covariance_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->covariance[i]);
            *bit_ofs += 16;
    }
}
