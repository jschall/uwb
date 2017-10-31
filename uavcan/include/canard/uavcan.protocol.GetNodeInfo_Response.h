#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.protocol.HardwareVersion.h>
#include <canard/uavcan.protocol.NodeStatus.h>
#include <canard/uavcan.protocol.SoftwareVersion.h>

#define UAVCAN_PROTOCOL_GETNODEINFO_RES_MAX_PACK_SIZE 377
#define UAVCAN_PROTOCOL_GETNODEINFO_RES_DT_SIG 0xEE468A8121C46A9E
#define UAVCAN_PROTOCOL_GETNODEINFO_RES_DT_ID 1

struct uavcan_protocol_GetNodeInfo_res_s {
    struct uavcan_protocol_NodeStatus_s status;
    struct uavcan_protocol_SoftwareVersion_s software_version;
    struct uavcan_protocol_HardwareVersion_s hardware_version;
    struct {uint8_t name_len; uint8_t name[80];};
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_GetNodeInfo_res_descriptor;

uint32_t encode_uavcan_protocol_GetNodeInfo_res(struct uavcan_protocol_GetNodeInfo_res_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_GetNodeInfo_res(const CanardRxTransfer* transfer, struct uavcan_protocol_GetNodeInfo_res_s* msg);
void _encode_uavcan_protocol_GetNodeInfo_res(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_GetNodeInfo_res_s* msg, bool tao);
void _decode_uavcan_protocol_GetNodeInfo_res(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_GetNodeInfo_res_s* msg, bool tao);
