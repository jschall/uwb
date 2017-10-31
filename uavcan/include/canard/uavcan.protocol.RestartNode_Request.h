#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_RESTARTNODE_REQ_MAX_PACK_SIZE 5
#define UAVCAN_PROTOCOL_RESTARTNODE_REQ_DT_SIG 0x569E05394A3017F0
#define UAVCAN_PROTOCOL_RESTARTNODE_REQ_DT_ID 5

#define UAVCAN_PROTOCOL_RESTARTNODE_REQ_MAGIC_NUMBER 742196058910

struct uavcan_protocol_RestartNode_req_s {
    uint64_t magic_number;
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_RestartNode_req_descriptor;

uint32_t encode_uavcan_protocol_RestartNode_req(struct uavcan_protocol_RestartNode_req_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_RestartNode_req(const CanardRxTransfer* transfer, struct uavcan_protocol_RestartNode_req_s* msg);
void _encode_uavcan_protocol_RestartNode_req(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_RestartNode_req_s* msg, bool tao);
void _decode_uavcan_protocol_RestartNode_req(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_RestartNode_req_s* msg, bool tao);
