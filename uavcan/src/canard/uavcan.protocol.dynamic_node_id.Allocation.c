#include <canard/uavcan.protocol.dynamic_node_id.Allocation.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_protocol_dynamic_node_id_Allocation(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_protocol_dynamic_node_id_Allocation(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_protocol_dynamic_node_id_Allocation_descriptor = {
    UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_DT_SIG,
    UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_protocol_dynamic_node_id_Allocation_s),
    UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_protocol_dynamic_node_id_Allocation(struct uavcan_protocol_dynamic_node_id_Allocation_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_MAX_PACK_SIZE);
    _encode_uavcan_protocol_dynamic_node_id_Allocation(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_protocol_dynamic_node_id_Allocation(const CanardRxTransfer* transfer, struct uavcan_protocol_dynamic_node_id_Allocation_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_protocol_dynamic_node_id_Allocation(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_protocol_dynamic_node_id_Allocation(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_dynamic_node_id_Allocation_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 7, &msg->node_id);
    *bit_ofs += 7;
    canardEncodeScalar(buffer, *bit_ofs, 1, &msg->first_part_of_unique_id);
    *bit_ofs += 1;
    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 5, &msg->unique_id_len);
        *bit_ofs += 5;
    }
    for (size_t i=0; i < msg->unique_id_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 8, &msg->unique_id[i]);
            *bit_ofs += 8;
    }
}

void _decode_uavcan_protocol_dynamic_node_id_Allocation(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_dynamic_node_id_Allocation_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 7, false, &msg->node_id);
    *bit_ofs += 7;
    canardDecodeScalar(transfer, *bit_ofs, 1, false, &msg->first_part_of_unique_id);
    *bit_ofs += 1;
    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 5, false, &msg->unique_id_len);
        *bit_ofs += 5;
    }
    for (size_t i=0; i < msg->unique_id_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->unique_id[i]);
            *bit_ofs += 8;
    }
}
