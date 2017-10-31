#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_PARAM_EXECUTEOPCODE_RES_MAX_PACK_SIZE 7
#define UAVCAN_PROTOCOL_PARAM_EXECUTEOPCODE_RES_DT_SIG 0x3B131AC5EB69D2CD
#define UAVCAN_PROTOCOL_PARAM_EXECUTEOPCODE_RES_DT_ID 10

struct uavcan_protocol_param_ExecuteOpcode_res_s {
    int64_t argument;
    bool ok;
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_param_ExecuteOpcode_res_descriptor;

uint32_t encode_uavcan_protocol_param_ExecuteOpcode_res(struct uavcan_protocol_param_ExecuteOpcode_res_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_param_ExecuteOpcode_res(const CanardRxTransfer* transfer, struct uavcan_protocol_param_ExecuteOpcode_res_s* msg);
void _encode_uavcan_protocol_param_ExecuteOpcode_res(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_param_ExecuteOpcode_res_s* msg, bool tao);
void _decode_uavcan_protocol_param_ExecuteOpcode_res(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_param_ExecuteOpcode_res_s* msg, bool tao);
