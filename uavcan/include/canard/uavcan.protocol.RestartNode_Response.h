#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_RESTARTNODE_RES_MAX_PACK_SIZE 1
#define UAVCAN_PROTOCOL_RESTARTNODE_RES_DT_SIG 0x569E05394A3017F0
#define UAVCAN_PROTOCOL_RESTARTNODE_RES_DT_ID 5

struct uavcan_protocol_RestartNode_res_s {
    bool ok;
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_RestartNode_res_descriptor;

uint32_t encode_uavcan_protocol_RestartNode_res(struct uavcan_protocol_RestartNode_res_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_RestartNode_res(const CanardRxTransfer* transfer, struct uavcan_protocol_RestartNode_res_s* msg);
void _encode_uavcan_protocol_RestartNode_res(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_RestartNode_res_s* msg, bool tao);
void _decode_uavcan_protocol_RestartNode_res(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_RestartNode_res_s* msg, bool tao);
