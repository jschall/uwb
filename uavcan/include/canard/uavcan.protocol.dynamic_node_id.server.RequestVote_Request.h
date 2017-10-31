#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_REQUESTVOTE_REQ_MAX_PACK_SIZE 9
#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_REQUESTVOTE_REQ_DT_SIG 0xCDDE07BB89A56356
#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_REQUESTVOTE_REQ_DT_ID 31

struct uavcan_protocol_dynamic_node_id_server_RequestVote_req_s {
    uint32_t term;
    uint32_t last_log_term;
    uint8_t last_log_index;
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_dynamic_node_id_server_RequestVote_req_descriptor;

uint32_t encode_uavcan_protocol_dynamic_node_id_server_RequestVote_req(struct uavcan_protocol_dynamic_node_id_server_RequestVote_req_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_dynamic_node_id_server_RequestVote_req(const CanardRxTransfer* transfer, struct uavcan_protocol_dynamic_node_id_server_RequestVote_req_s* msg);
void _encode_uavcan_protocol_dynamic_node_id_server_RequestVote_req(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_dynamic_node_id_server_RequestVote_req_s* msg, bool tao);
void _decode_uavcan_protocol_dynamic_node_id_server_RequestVote_req(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_dynamic_node_id_server_RequestVote_req_s* msg, bool tao);
