#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_GETNODEINFO_REQ_MAX_PACK_SIZE 0
#define UAVCAN_PROTOCOL_GETNODEINFO_REQ_DT_SIG 0xEE468A8121C46A9E
#define UAVCAN_PROTOCOL_GETNODEINFO_REQ_DT_ID 1

struct uavcan_protocol_GetNodeInfo_req_s {
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_GetNodeInfo_req_descriptor;

uint32_t encode_uavcan_protocol_GetNodeInfo_req(struct uavcan_protocol_GetNodeInfo_req_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_GetNodeInfo_req(const CanardRxTransfer* transfer, struct uavcan_protocol_GetNodeInfo_req_s* msg);
void _encode_uavcan_protocol_GetNodeInfo_req(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_GetNodeInfo_req_s* msg, bool tao);
void _decode_uavcan_protocol_GetNodeInfo_req(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_GetNodeInfo_req_s* msg, bool tao);
