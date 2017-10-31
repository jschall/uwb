#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.equipment.indication.SingleLightCommand.h>

#define UAVCAN_EQUIPMENT_INDICATION_LIGHTSCOMMAND_MAX_PACK_SIZE 61
#define UAVCAN_EQUIPMENT_INDICATION_LIGHTSCOMMAND_DT_SIG 0x2031D93C8BDD1EC4
#define UAVCAN_EQUIPMENT_INDICATION_LIGHTSCOMMAND_DT_ID 1081

struct uavcan_equipment_indication_LightsCommand_s {
    struct {uint8_t commands_len; struct uavcan_equipment_indication_SingleLightCommand_s commands[20];};
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_indication_LightsCommand_descriptor;

uint32_t encode_uavcan_equipment_indication_LightsCommand(struct uavcan_equipment_indication_LightsCommand_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_indication_LightsCommand(const CanardRxTransfer* transfer, struct uavcan_equipment_indication_LightsCommand_s* msg);
void _encode_uavcan_equipment_indication_LightsCommand(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_indication_LightsCommand_s* msg, bool tao);
void _decode_uavcan_equipment_indication_LightsCommand(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_indication_LightsCommand_s* msg, bool tao);
