#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.equipment.indication.RGB565.h>

#define UAVCAN_EQUIPMENT_INDICATION_SINGLELIGHTCOMMAND_MAX_PACK_SIZE 3
#define UAVCAN_EQUIPMENT_INDICATION_SINGLELIGHTCOMMAND_DT_SIG 0xE894B8B589807007

#define UAVCAN_EQUIPMENT_INDICATION_SINGLELIGHTCOMMAND_LIGHT_ID_ANTI_COLLISION 246
#define UAVCAN_EQUIPMENT_INDICATION_SINGLELIGHTCOMMAND_LIGHT_ID_RIGHT_OF_WAY 247
#define UAVCAN_EQUIPMENT_INDICATION_SINGLELIGHTCOMMAND_LIGHT_ID_STROBE 248
#define UAVCAN_EQUIPMENT_INDICATION_SINGLELIGHTCOMMAND_LIGHT_ID_WING 249
#define UAVCAN_EQUIPMENT_INDICATION_SINGLELIGHTCOMMAND_LIGHT_ID_LOGO 250
#define UAVCAN_EQUIPMENT_INDICATION_SINGLELIGHTCOMMAND_LIGHT_ID_TAXI 251
#define UAVCAN_EQUIPMENT_INDICATION_SINGLELIGHTCOMMAND_LIGHT_ID_TURN_OFF 252
#define UAVCAN_EQUIPMENT_INDICATION_SINGLELIGHTCOMMAND_LIGHT_ID_TAKE_OFF 253
#define UAVCAN_EQUIPMENT_INDICATION_SINGLELIGHTCOMMAND_LIGHT_ID_LANDING 254
#define UAVCAN_EQUIPMENT_INDICATION_SINGLELIGHTCOMMAND_LIGHT_ID_FORMATION 255

struct uavcan_equipment_indication_SingleLightCommand_s {
    uint8_t light_id;
    struct uavcan_equipment_indication_RGB565_s color;
};


uint32_t encode_uavcan_equipment_indication_SingleLightCommand(struct uavcan_equipment_indication_SingleLightCommand_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_indication_SingleLightCommand(const CanardRxTransfer* transfer, struct uavcan_equipment_indication_SingleLightCommand_s* msg);
void _encode_uavcan_equipment_indication_SingleLightCommand(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_indication_SingleLightCommand_s* msg, bool tao);
void _decode_uavcan_equipment_indication_SingleLightCommand(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_indication_SingleLightCommand_s* msg, bool tao);
