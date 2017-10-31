#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_ACTUATOR_STATUS_MAX_PACK_SIZE 8
#define UAVCAN_EQUIPMENT_ACTUATOR_STATUS_DT_SIG 0x5E9BBA44FAF1EA04
#define UAVCAN_EQUIPMENT_ACTUATOR_STATUS_DT_ID 1011

#define UAVCAN_EQUIPMENT_ACTUATOR_STATUS_POWER_RATING_PCT_UNKNOWN 127

struct uavcan_equipment_actuator_Status_s {
    uint8_t actuator_id;
    float position;
    float force;
    float speed;
    uint8_t power_rating_pct;
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_actuator_Status_descriptor;

uint32_t encode_uavcan_equipment_actuator_Status(struct uavcan_equipment_actuator_Status_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_actuator_Status(const CanardRxTransfer* transfer, struct uavcan_equipment_actuator_Status_s* msg);
void _encode_uavcan_equipment_actuator_Status(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_actuator_Status_s* msg, bool tao);
void _decode_uavcan_equipment_actuator_Status(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_actuator_Status_s* msg, bool tao);
