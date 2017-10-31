#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_PARAM_EXECUTEOPCODE_REQ_MAX_PACK_SIZE 7
#define UAVCAN_PROTOCOL_PARAM_EXECUTEOPCODE_REQ_DT_SIG 0x3B131AC5EB69D2CD
#define UAVCAN_PROTOCOL_PARAM_EXECUTEOPCODE_REQ_DT_ID 10

#define UAVCAN_PROTOCOL_PARAM_EXECUTEOPCODE_REQ_OPCODE_SAVE 0
#define UAVCAN_PROTOCOL_PARAM_EXECUTEOPCODE_REQ_OPCODE_ERASE 1

struct uavcan_protocol_param_ExecuteOpcode_req_s {
    uint8_t opcode;
    int64_t argument;
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_param_ExecuteOpcode_req_descriptor;

uint32_t encode_uavcan_protocol_param_ExecuteOpcode_req(struct uavcan_protocol_param_ExecuteOpcode_req_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_param_ExecuteOpcode_req(const CanardRxTransfer* transfer, struct uavcan_protocol_param_ExecuteOpcode_req_s* msg);
void _encode_uavcan_protocol_param_ExecuteOpcode_req(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_param_ExecuteOpcode_req_s* msg, bool tao);
void _decode_uavcan_protocol_param_ExecuteOpcode_req(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_param_ExecuteOpcode_req_s* msg, bool tao);
