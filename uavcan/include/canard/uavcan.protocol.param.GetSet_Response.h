#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.protocol.param.NumericValue.h>
#include <canard/uavcan.protocol.param.Value.h>

#define UAVCAN_PROTOCOL_PARAM_GETSET_RES_MAX_PACK_SIZE 371
#define UAVCAN_PROTOCOL_PARAM_GETSET_RES_DT_SIG 0xA7B622F939D1A4D5
#define UAVCAN_PROTOCOL_PARAM_GETSET_RES_DT_ID 11

struct uavcan_protocol_param_GetSet_res_s {
    struct uavcan_protocol_param_Value_s value;
    struct uavcan_protocol_param_Value_s default_value;
    struct uavcan_protocol_param_NumericValue_s max_value;
    struct uavcan_protocol_param_NumericValue_s min_value;
    struct {uint8_t name_len; uint8_t name[92];};
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_param_GetSet_res_descriptor;

uint32_t encode_uavcan_protocol_param_GetSet_res(struct uavcan_protocol_param_GetSet_res_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_param_GetSet_res(const CanardRxTransfer* transfer, struct uavcan_protocol_param_GetSet_res_s* msg);
void _encode_uavcan_protocol_param_GetSet_res(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_param_GetSet_res_s* msg, bool tao);
void _decode_uavcan_protocol_param_GetSet_res(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_param_GetSet_res_s* msg, bool tao);
