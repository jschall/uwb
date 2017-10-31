#include <canard/uavcan.equipment.range_sensor.Measurement.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_equipment_range_sensor_Measurement(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_equipment_range_sensor_Measurement(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_equipment_range_sensor_Measurement_descriptor = {
    UAVCAN_EQUIPMENT_RANGE_SENSOR_MEASUREMENT_DT_SIG,
    UAVCAN_EQUIPMENT_RANGE_SENSOR_MEASUREMENT_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_equipment_range_sensor_Measurement_s),
    UAVCAN_EQUIPMENT_RANGE_SENSOR_MEASUREMENT_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_equipment_range_sensor_Measurement(struct uavcan_equipment_range_sensor_Measurement_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_RANGE_SENSOR_MEASUREMENT_MAX_PACK_SIZE);
    _encode_uavcan_equipment_range_sensor_Measurement(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_range_sensor_Measurement(const CanardRxTransfer* transfer, struct uavcan_equipment_range_sensor_Measurement_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_range_sensor_Measurement(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_range_sensor_Measurement(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_range_sensor_Measurement_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    _encode_uavcan_Timestamp(buffer, bit_ofs, &msg->timestamp, false);
    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->sensor_id);
    *bit_ofs += 8;
    _encode_uavcan_CoarseOrientation(buffer, bit_ofs, &msg->beam_orientation_in_body_frame, false);
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->field_of_view);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 5, &msg->sensor_type);
    *bit_ofs += 5;
    canardEncodeScalar(buffer, *bit_ofs, 3, &msg->reading_type);
    *bit_ofs += 3;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->range);
    *bit_ofs += 16;
}

void _decode_uavcan_equipment_range_sensor_Measurement(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_range_sensor_Measurement_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    _decode_uavcan_Timestamp(transfer, bit_ofs, &msg->timestamp, false);
    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->sensor_id);
    *bit_ofs += 8;
    _decode_uavcan_CoarseOrientation(transfer, bit_ofs, &msg->beam_orientation_in_body_frame, false);
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->field_of_view);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 5, false, &msg->sensor_type);
    *bit_ofs += 5;
    canardDecodeScalar(transfer, *bit_ofs, 3, false, &msg->reading_type);
    *bit_ofs += 3;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->range);
    *bit_ofs += 16;
}
