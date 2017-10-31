#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_INDICATION_BEEPCOMMAND_MAX_PACK_SIZE 4
#define UAVCAN_EQUIPMENT_INDICATION_BEEPCOMMAND_DT_SIG 0xBE9EA9FEC2B15D52
#define UAVCAN_EQUIPMENT_INDICATION_BEEPCOMMAND_DT_ID 1080

struct uavcan_equipment_indication_BeepCommand_s {
    float frequency;
    float duration;
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_indication_BeepCommand_descriptor;

uint32_t encode_uavcan_equipment_indication_BeepCommand(struct uavcan_equipment_indication_BeepCommand_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_indication_BeepCommand(const CanardRxTransfer* transfer, struct uavcan_equipment_indication_BeepCommand_s* msg);
void _encode_uavcan_equipment_indication_BeepCommand(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_indication_BeepCommand_s* msg, bool tao);
void _decode_uavcan_equipment_indication_BeepCommand(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_indication_BeepCommand_s* msg, bool tao);
