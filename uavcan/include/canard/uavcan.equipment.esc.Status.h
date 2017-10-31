#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_ESC_STATUS_MAX_PACK_SIZE 14
#define UAVCAN_EQUIPMENT_ESC_STATUS_DT_SIG 0xA9AF28AEA2FBB254
#define UAVCAN_EQUIPMENT_ESC_STATUS_DT_ID 1034

struct uavcan_equipment_esc_Status_s {
    uint32_t error_count;
    float voltage;
    float current;
    float temperature;
    int32_t rpm;
    uint8_t power_rating_pct;
    uint8_t esc_index;
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_esc_Status_descriptor;

uint32_t encode_uavcan_equipment_esc_Status(struct uavcan_equipment_esc_Status_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_esc_Status(const CanardRxTransfer* transfer, struct uavcan_equipment_esc_Status_s* msg);
void _encode_uavcan_equipment_esc_Status(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_esc_Status_s* msg, bool tao);
void _decode_uavcan_equipment_esc_Status(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_esc_Status_s* msg, bool tao);
