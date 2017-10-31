#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_DISCOVERY_MAX_PACK_SIZE 7
#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_DISCOVERY_DT_SIG 0x821AE2F525F69F21
#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_DISCOVERY_DT_ID 390

#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_DISCOVERY_BROADCASTING_PERIOD_MS 1000

struct uavcan_protocol_dynamic_node_id_server_Discovery_s {
    uint8_t configured_cluster_size;
    struct {uint8_t known_nodes_len; uint8_t known_nodes[5];};
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_dynamic_node_id_server_Discovery_descriptor;

uint32_t encode_uavcan_protocol_dynamic_node_id_server_Discovery(struct uavcan_protocol_dynamic_node_id_server_Discovery_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_dynamic_node_id_server_Discovery(const CanardRxTransfer* transfer, struct uavcan_protocol_dynamic_node_id_server_Discovery_s* msg);
void _encode_uavcan_protocol_dynamic_node_id_server_Discovery(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_dynamic_node_id_server_Discovery_s* msg, bool tao);
void _decode_uavcan_protocol_dynamic_node_id_server_Discovery(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_dynamic_node_id_server_Discovery_s* msg, bool tao);
