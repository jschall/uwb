#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.protocol.CANIfaceStats.h>

#define UAVCAN_PROTOCOL_GETTRANSPORTSTATS_RES_MAX_PACK_SIZE 73
#define UAVCAN_PROTOCOL_GETTRANSPORTSTATS_RES_DT_SIG 0xBE6F76A7EC312B04
#define UAVCAN_PROTOCOL_GETTRANSPORTSTATS_RES_DT_ID 4

struct uavcan_protocol_GetTransportStats_res_s {
    uint64_t transfers_tx;
    uint64_t transfers_rx;
    uint64_t transfer_errors;
    struct {uint8_t can_iface_stats_len; struct uavcan_protocol_CANIfaceStats_s can_iface_stats[3];};
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_GetTransportStats_res_descriptor;

uint32_t encode_uavcan_protocol_GetTransportStats_res(struct uavcan_protocol_GetTransportStats_res_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_GetTransportStats_res(const CanardRxTransfer* transfer, struct uavcan_protocol_GetTransportStats_res_s* msg);
void _encode_uavcan_protocol_GetTransportStats_res(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_GetTransportStats_res_s* msg, bool tao);
void _decode_uavcan_protocol_GetTransportStats_res(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_GetTransportStats_res_s* msg, bool tao);
