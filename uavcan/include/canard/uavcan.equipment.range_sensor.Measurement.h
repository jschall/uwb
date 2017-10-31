#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.CoarseOrientation.h>
#include <canard/uavcan.Timestamp.h>

#define UAVCAN_EQUIPMENT_RANGE_SENSOR_MEASUREMENT_MAX_PACK_SIZE 15
#define UAVCAN_EQUIPMENT_RANGE_SENSOR_MEASUREMENT_DT_SIG 0x68FFFE70FC771952
#define UAVCAN_EQUIPMENT_RANGE_SENSOR_MEASUREMENT_DT_ID 1050

#define UAVCAN_EQUIPMENT_RANGE_SENSOR_MEASUREMENT_SENSOR_TYPE_UNDEFINED 0
#define UAVCAN_EQUIPMENT_RANGE_SENSOR_MEASUREMENT_SENSOR_TYPE_SONAR 1
#define UAVCAN_EQUIPMENT_RANGE_SENSOR_MEASUREMENT_SENSOR_TYPE_LIDAR 2
#define UAVCAN_EQUIPMENT_RANGE_SENSOR_MEASUREMENT_SENSOR_TYPE_RADAR 3
#define UAVCAN_EQUIPMENT_RANGE_SENSOR_MEASUREMENT_READING_TYPE_UNDEFINED 0
#define UAVCAN_EQUIPMENT_RANGE_SENSOR_MEASUREMENT_READING_TYPE_VALID_RANGE 1
#define UAVCAN_EQUIPMENT_RANGE_SENSOR_MEASUREMENT_READING_TYPE_TOO_CLOSE 2
#define UAVCAN_EQUIPMENT_RANGE_SENSOR_MEASUREMENT_READING_TYPE_TOO_FAR 3

struct uavcan_equipment_range_sensor_Measurement_s {
    struct uavcan_Timestamp_s timestamp;
    uint8_t sensor_id;
    struct uavcan_CoarseOrientation_s beam_orientation_in_body_frame;
    float field_of_view;
    uint8_t sensor_type;
    uint8_t reading_type;
    float range;
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_range_sensor_Measurement_descriptor;

uint32_t encode_uavcan_equipment_range_sensor_Measurement(struct uavcan_equipment_range_sensor_Measurement_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_range_sensor_Measurement(const CanardRxTransfer* transfer, struct uavcan_equipment_range_sensor_Measurement_s* msg);
void _encode_uavcan_equipment_range_sensor_Measurement(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_range_sensor_Measurement_s* msg, bool tao);
void _decode_uavcan_equipment_range_sensor_Measurement(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_range_sensor_Measurement_s* msg, bool tao);
