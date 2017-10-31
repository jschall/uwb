#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.equipment.actuator.Command.h>

#define UAVCAN_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_MAX_PACK_SIZE 61
#define UAVCAN_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_DT_SIG 0xD8A7486238EC3AF3
#define UAVCAN_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_DT_ID 1010

struct uavcan_equipment_actuator_ArrayCommand_s {
    struct {uint8_t commands_len; struct uavcan_equipment_actuator_Command_s commands[15];};
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_actuator_ArrayCommand_descriptor;

uint32_t encode_uavcan_equipment_actuator_ArrayCommand(struct uavcan_equipment_actuator_ArrayCommand_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_actuator_ArrayCommand(const CanardRxTransfer* transfer, struct uavcan_equipment_actuator_ArrayCommand_s* msg);
void _encode_uavcan_equipment_actuator_ArrayCommand(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_actuator_ArrayCommand_s* msg, bool tao);
void _decode_uavcan_equipment_actuator_ArrayCommand(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_actuator_ArrayCommand_s* msg, bool tao);
