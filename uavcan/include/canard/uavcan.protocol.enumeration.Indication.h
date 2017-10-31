#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.protocol.param.NumericValue.h>

#define UAVCAN_PROTOCOL_ENUMERATION_INDICATION_MAX_PACK_SIZE 102
#define UAVCAN_PROTOCOL_ENUMERATION_INDICATION_DT_SIG 0x884CB63050A84F35
#define UAVCAN_PROTOCOL_ENUMERATION_INDICATION_DT_ID 380

struct uavcan_protocol_enumeration_Indication_s {
    struct uavcan_protocol_param_NumericValue_s value;
    struct {uint8_t parameter_name_len; uint8_t parameter_name[92];};
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_enumeration_Indication_descriptor;

uint32_t encode_uavcan_protocol_enumeration_Indication(struct uavcan_protocol_enumeration_Indication_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_enumeration_Indication(const CanardRxTransfer* transfer, struct uavcan_protocol_enumeration_Indication_s* msg);
void _encode_uavcan_protocol_enumeration_Indication(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_enumeration_Indication_s* msg, bool tao);
void _decode_uavcan_protocol_enumeration_Indication(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_enumeration_Indication_s* msg, bool tao);
