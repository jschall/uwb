#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_INDICATION_RGB565_MAX_PACK_SIZE 2
#define UAVCAN_EQUIPMENT_INDICATION_RGB565_DT_SIG 0x58A7CEF41951EC34

struct uavcan_equipment_indication_RGB565_s {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};


uint32_t encode_uavcan_equipment_indication_RGB565(struct uavcan_equipment_indication_RGB565_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_indication_RGB565(const CanardRxTransfer* transfer, struct uavcan_equipment_indication_RGB565_s* msg);
void _encode_uavcan_equipment_indication_RGB565(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_indication_RGB565_s* msg, bool tao);
void _decode_uavcan_equipment_indication_RGB565(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_indication_RGB565_s* msg, bool tao);
