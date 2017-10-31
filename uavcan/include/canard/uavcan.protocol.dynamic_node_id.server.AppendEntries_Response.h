#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_APPENDENTRIES_RES_MAX_PACK_SIZE 5
#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_APPENDENTRIES_RES_DT_SIG 0x8032C7097B48A3CC
#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_APPENDENTRIES_RES_DT_ID 30

struct uavcan_protocol_dynamic_node_id_server_AppendEntries_res_s {
    uint32_t term;
    bool success;
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_dynamic_node_id_server_AppendEntries_res_descriptor;

uint32_t encode_uavcan_protocol_dynamic_node_id_server_AppendEntries_res(struct uavcan_protocol_dynamic_node_id_server_AppendEntries_res_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_dynamic_node_id_server_AppendEntries_res(const CanardRxTransfer* transfer, struct uavcan_protocol_dynamic_node_id_server_AppendEntries_res_s* msg);
void _encode_uavcan_protocol_dynamic_node_id_server_AppendEntries_res(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_dynamic_node_id_server_AppendEntries_res_s* msg, bool tao);
void _decode_uavcan_protocol_dynamic_node_id_server_AppendEntries_res(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_dynamic_node_id_server_AppendEntries_res_s* msg, bool tao);
