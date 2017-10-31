#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_ESC_RAWCOMMAND_MAX_PACK_SIZE 36
#define UAVCAN_EQUIPMENT_ESC_RAWCOMMAND_DT_SIG 0x217F5C87D7EC951D
#define UAVCAN_EQUIPMENT_ESC_RAWCOMMAND_DT_ID 1030

struct uavcan_equipment_esc_RawCommand_s {
    struct {uint8_t cmd_len; int16_t cmd[20];};
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_esc_RawCommand_descriptor;

uint32_t encode_uavcan_equipment_esc_RawCommand(struct uavcan_equipment_esc_RawCommand_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_esc_RawCommand(const CanardRxTransfer* transfer, struct uavcan_equipment_esc_RawCommand_s* msg);
void _encode_uavcan_equipment_esc_RawCommand(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_esc_RawCommand_s* msg, bool tao);
void _decode_uavcan_equipment_esc_RawCommand(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_esc_RawCommand_s* msg, bool tao);
