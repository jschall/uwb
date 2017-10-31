#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_GNSS_AUXILIARY_MAX_PACK_SIZE 16
#define UAVCAN_EQUIPMENT_GNSS_AUXILIARY_DT_SIG 0x9BE8BDC4C3DBBFD2
#define UAVCAN_EQUIPMENT_GNSS_AUXILIARY_DT_ID 1061

struct uavcan_equipment_gnss_Auxiliary_s {
    float gdop;
    float pdop;
    float hdop;
    float vdop;
    float tdop;
    float ndop;
    float edop;
    uint8_t sats_visible;
    uint8_t sats_used;
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_gnss_Auxiliary_descriptor;

uint32_t encode_uavcan_equipment_gnss_Auxiliary(struct uavcan_equipment_gnss_Auxiliary_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_gnss_Auxiliary(const CanardRxTransfer* transfer, struct uavcan_equipment_gnss_Auxiliary_s* msg);
void _encode_uavcan_equipment_gnss_Auxiliary(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_gnss_Auxiliary_s* msg, bool tao);
void _decode_uavcan_equipment_gnss_Auxiliary(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_gnss_Auxiliary_s* msg, bool tao);
