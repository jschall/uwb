#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.protocol.dynamic_node_id.server.Entry.h>

#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_APPENDENTRIES_REQ_MAX_PACK_SIZE 32
#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_APPENDENTRIES_REQ_DT_SIG 0x8032C7097B48A3CC
#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_APPENDENTRIES_REQ_DT_ID 30

#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_APPENDENTRIES_REQ_DEFAULT_MIN_ELECTION_TIMEOUT_MS 2000
#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_APPENDENTRIES_REQ_DEFAULT_MAX_ELECTION_TIMEOUT_MS 4000

struct uavcan_protocol_dynamic_node_id_server_AppendEntries_req_s {
    uint32_t term;
    uint32_t prev_log_term;
    uint8_t prev_log_index;
    uint8_t leader_commit;
    struct {uint8_t entries_len; struct uavcan_protocol_dynamic_node_id_server_Entry_s entries[1];};
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_dynamic_node_id_server_AppendEntries_req_descriptor;

uint32_t encode_uavcan_protocol_dynamic_node_id_server_AppendEntries_req(struct uavcan_protocol_dynamic_node_id_server_AppendEntries_req_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_dynamic_node_id_server_AppendEntries_req(const CanardRxTransfer* transfer, struct uavcan_protocol_dynamic_node_id_server_AppendEntries_req_s* msg);
void _encode_uavcan_protocol_dynamic_node_id_server_AppendEntries_req(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_dynamic_node_id_server_AppendEntries_req_s* msg, bool tao);
void _decode_uavcan_protocol_dynamic_node_id_server_AppendEntries_req(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_dynamic_node_id_server_AppendEntries_req_s* msg, bool tao);
