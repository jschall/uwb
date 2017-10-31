#include <canard/uavcan.protocol.dynamic_node_id.server.Discovery.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_protocol_dynamic_node_id_server_Discovery(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_protocol_dynamic_node_id_server_Discovery(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_protocol_dynamic_node_id_server_Discovery_descriptor = {
    UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_DISCOVERY_DT_SIG,
    UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_DISCOVERY_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_protocol_dynamic_node_id_server_Discovery_s),
    UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_DISCOVERY_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_protocol_dynamic_node_id_server_Discovery(struct uavcan_protocol_dynamic_node_id_server_Discovery_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_DISCOVERY_MAX_PACK_SIZE);
    _encode_uavcan_protocol_dynamic_node_id_server_Discovery(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_protocol_dynamic_node_id_server_Discovery(const CanardRxTransfer* transfer, struct uavcan_protocol_dynamic_node_id_server_Discovery_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_protocol_dynamic_node_id_server_Discovery(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_protocol_dynamic_node_id_server_Discovery(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_dynamic_node_id_server_Discovery_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->configured_cluster_size);
    *bit_ofs += 8;
    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 3, &msg->known_nodes_len);
        *bit_ofs += 3;
    }
    for (size_t i=0; i < msg->known_nodes_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 8, &msg->known_nodes[i]);
            *bit_ofs += 8;
    }
}

void _decode_uavcan_protocol_dynamic_node_id_server_Discovery(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_dynamic_node_id_server_Discovery_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->configured_cluster_size);
    *bit_ofs += 8;
    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 3, false, &msg->known_nodes_len);
        *bit_ofs += 3;
    }
    for (size_t i=0; i < msg->known_nodes_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->known_nodes[i]);
            *bit_ofs += 8;
    }
}
