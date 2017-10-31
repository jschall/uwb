#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_ACTUATOR_COMMAND_MAX_PACK_SIZE 4
#define UAVCAN_EQUIPMENT_ACTUATOR_COMMAND_DT_SIG 0x8D9A6A920C1D616C

#define UAVCAN_EQUIPMENT_ACTUATOR_COMMAND_COMMAND_TYPE_UNITLESS 0
#define UAVCAN_EQUIPMENT_ACTUATOR_COMMAND_COMMAND_TYPE_POSITION 1
#define UAVCAN_EQUIPMENT_ACTUATOR_COMMAND_COMMAND_TYPE_FORCE 2
#define UAVCAN_EQUIPMENT_ACTUATOR_COMMAND_COMMAND_TYPE_SPEED 3

struct uavcan_equipment_actuator_Command_s {
    uint8_t actuator_id;
    uint8_t command_type;
    float command_value;
};


uint32_t encode_uavcan_equipment_actuator_Command(struct uavcan_equipment_actuator_Command_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_actuator_Command(const CanardRxTransfer* transfer, struct uavcan_equipment_actuator_Command_s* msg);
void _encode_uavcan_equipment_actuator_Command(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_actuator_Command_s* msg, bool tao);
void _decode_uavcan_equipment_actuator_Command(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_actuator_Command_s* msg, bool tao);
