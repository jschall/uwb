#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_REQUESTVOTE_RES_MAX_PACK_SIZE 5
#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_REQUESTVOTE_RES_DT_SIG 0xCDDE07BB89A56356
#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_REQUESTVOTE_RES_DT_ID 31

struct uavcan_protocol_dynamic_node_id_server_RequestVote_res_s {
    uint32_t term;
    bool vote_granted;
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_dynamic_node_id_server_RequestVote_res_descriptor;

uint32_t encode_uavcan_protocol_dynamic_node_id_server_RequestVote_res(struct uavcan_protocol_dynamic_node_id_server_RequestVote_res_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_dynamic_node_id_server_RequestVote_res(const CanardRxTransfer* transfer, struct uavcan_protocol_dynamic_node_id_server_RequestVote_res_s* msg);
void _encode_uavcan_protocol_dynamic_node_id_server_RequestVote_res(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_dynamic_node_id_server_RequestVote_res_s* msg, bool tao);
void _decode_uavcan_protocol_dynamic_node_id_server_RequestVote_res(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_dynamic_node_id_server_RequestVote_res_s* msg, bool tao);
