#include <canard/uavcan.protocol.dynamic_node_id.server.AppendEntries_Request.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_protocol_dynamic_node_id_server_AppendEntries_req(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_protocol_dynamic_node_id_server_AppendEntries_req(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_protocol_dynamic_node_id_server_AppendEntries_req_descriptor = {
    UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_APPENDENTRIES_REQ_DT_SIG,
    UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_APPENDENTRIES_REQ_DT_ID,
    CanardTransferTypeRequest,
    sizeof(struct uavcan_protocol_dynamic_node_id_server_AppendEntries_req_s),
    UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_APPENDENTRIES_REQ_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_protocol_dynamic_node_id_server_AppendEntries_req(struct uavcan_protocol_dynamic_node_id_server_AppendEntries_req_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_APPENDENTRIES_REQ_MAX_PACK_SIZE);
    _encode_uavcan_protocol_dynamic_node_id_server_AppendEntries_req(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_protocol_dynamic_node_id_server_AppendEntries_req(const CanardRxTransfer* transfer, struct uavcan_protocol_dynamic_node_id_server_AppendEntries_req_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_protocol_dynamic_node_id_server_AppendEntries_req(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_protocol_dynamic_node_id_server_AppendEntries_req(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_dynamic_node_id_server_AppendEntries_req_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 32, &msg->term);
    *bit_ofs += 32;
    canardEncodeScalar(buffer, *bit_ofs, 32, &msg->prev_log_term);
    *bit_ofs += 32;
    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->prev_log_index);
    *bit_ofs += 8;
    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->leader_commit);
    *bit_ofs += 8;
    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 1, &msg->entries_len);
        *bit_ofs += 1;
    }
    for (size_t i=0; i < msg->entries_len; i++) {
            _encode_uavcan_protocol_dynamic_node_id_server_Entry(buffer, bit_ofs, &msg->entries[i], false);
    }
}

void _decode_uavcan_protocol_dynamic_node_id_server_AppendEntries_req(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_dynamic_node_id_server_AppendEntries_req_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 32, false, &msg->term);
    *bit_ofs += 32;
    canardDecodeScalar(transfer, *bit_ofs, 32, false, &msg->prev_log_term);
    *bit_ofs += 32;
    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->prev_log_index);
    *bit_ofs += 8;
    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->leader_commit);
    *bit_ofs += 8;
    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 1, false, &msg->entries_len);
        *bit_ofs += 1;
    }
    for (size_t i=0; i < msg->entries_len; i++) {
            _decode_uavcan_protocol_dynamic_node_id_server_Entry(transfer, bit_ofs, &msg->entries[i], false);
    }
}
