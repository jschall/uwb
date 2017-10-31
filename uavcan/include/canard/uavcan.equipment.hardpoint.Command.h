#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_HARDPOINT_COMMAND_MAX_PACK_SIZE 3
#define UAVCAN_EQUIPMENT_HARDPOINT_COMMAND_DT_SIG 0xA1A036268B0C3455
#define UAVCAN_EQUIPMENT_HARDPOINT_COMMAND_DT_ID 1070

struct uavcan_equipment_hardpoint_Command_s {
    uint8_t hardpoint_id;
    uint16_t command;
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_hardpoint_Command_descriptor;

uint32_t encode_uavcan_equipment_hardpoint_Command(struct uavcan_equipment_hardpoint_Command_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_hardpoint_Command(const CanardRxTransfer* transfer, struct uavcan_equipment_hardpoint_Command_s* msg);
void _encode_uavcan_equipment_hardpoint_Command(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_hardpoint_Command_s* msg, bool tao);
void _decode_uavcan_equipment_hardpoint_Command(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_hardpoint_Command_s* msg, bool tao);
