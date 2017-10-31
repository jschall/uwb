#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_HARDPOINT_STATUS_MAX_PACK_SIZE 7
#define UAVCAN_EQUIPMENT_HARDPOINT_STATUS_DT_SIG 0x624A519D42553D82
#define UAVCAN_EQUIPMENT_HARDPOINT_STATUS_DT_ID 1071

struct uavcan_equipment_hardpoint_Status_s {
    uint8_t hardpoint_id;
    float payload_weight;
    float payload_weight_variance;
    uint16_t status;
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_hardpoint_Status_descriptor;

uint32_t encode_uavcan_equipment_hardpoint_Status(struct uavcan_equipment_hardpoint_Status_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_hardpoint_Status(const CanardRxTransfer* transfer, struct uavcan_equipment_hardpoint_Status_s* msg);
void _encode_uavcan_equipment_hardpoint_Status(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_hardpoint_Status_s* msg, bool tao);
void _decode_uavcan_equipment_hardpoint_Status(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_hardpoint_Status_s* msg, bool tao);
