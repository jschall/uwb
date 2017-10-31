#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_ENTRY_MAX_PACK_SIZE 21
#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_ENTRY_DT_SIG 0x7FAA779D64FA75C2

struct uavcan_protocol_dynamic_node_id_server_Entry_s {
    uint32_t term;
    uint8_t unique_id[16];
    uint8_t node_id;
};


uint32_t encode_uavcan_protocol_dynamic_node_id_server_Entry(struct uavcan_protocol_dynamic_node_id_server_Entry_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_dynamic_node_id_server_Entry(const CanardRxTransfer* transfer, struct uavcan_protocol_dynamic_node_id_server_Entry_s* msg);
void _encode_uavcan_protocol_dynamic_node_id_server_Entry(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_dynamic_node_id_server_Entry_s* msg, bool tao);
void _decode_uavcan_protocol_dynamic_node_id_server_Entry(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_dynamic_node_id_server_Entry_s* msg, bool tao);
