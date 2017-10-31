#include <canard/uavcan.protocol.dynamic_node_id.server.Entry.h>
#include <string.h>


uint32_t encode_uavcan_protocol_dynamic_node_id_server_Entry(struct uavcan_protocol_dynamic_node_id_server_Entry_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_ENTRY_MAX_PACK_SIZE);
    _encode_uavcan_protocol_dynamic_node_id_server_Entry(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_protocol_dynamic_node_id_server_Entry(const CanardRxTransfer* transfer, struct uavcan_protocol_dynamic_node_id_server_Entry_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_protocol_dynamic_node_id_server_Entry(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_protocol_dynamic_node_id_server_Entry(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_dynamic_node_id_server_Entry_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 32, &msg->term);
    *bit_ofs += 32;
    for (size_t i=0; i < 16; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 8, &msg->unique_id[i]);
            *bit_ofs += 8;
    }
    *bit_ofs += 1;
    canardEncodeScalar(buffer, *bit_ofs, 7, &msg->node_id);
    *bit_ofs += 7;
}

void _decode_uavcan_protocol_dynamic_node_id_server_Entry(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_dynamic_node_id_server_Entry_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 32, false, &msg->term);
    *bit_ofs += 32;
    for (size_t i=0; i < 16; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->unique_id[i]);
            *bit_ofs += 8;
    }
    *bit_ofs += 1;
    canardDecodeScalar(transfer, *bit_ofs, 7, false, &msg->node_id);
    *bit_ofs += 7;
}
