#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_ESC_RPMCOMMAND_MAX_PACK_SIZE 46
#define UAVCAN_EQUIPMENT_ESC_RPMCOMMAND_DT_SIG 0xCE0F9F621CF7E70B
#define UAVCAN_EQUIPMENT_ESC_RPMCOMMAND_DT_ID 1031

struct uavcan_equipment_esc_RPMCommand_s {
    struct {uint8_t rpm_len; int32_t rpm[20];};
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_esc_RPMCommand_descriptor;

uint32_t encode_uavcan_equipment_esc_RPMCommand(struct uavcan_equipment_esc_RPMCommand_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_esc_RPMCommand(const CanardRxTransfer* transfer, struct uavcan_equipment_esc_RPMCommand_s* msg);
void _encode_uavcan_equipment_esc_RPMCommand(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_esc_RPMCommand_s* msg, bool tao);
void _decode_uavcan_equipment_esc_RPMCommand(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_esc_RPMCommand_s* msg, bool tao);
