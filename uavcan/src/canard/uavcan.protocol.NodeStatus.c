#include <canard/uavcan.protocol.NodeStatus.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_protocol_NodeStatus(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_protocol_NodeStatus(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_protocol_NodeStatus_descriptor = {
    UAVCAN_PROTOCOL_NODESTATUS_DT_SIG,
    UAVCAN_PROTOCOL_NODESTATUS_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_protocol_NodeStatus_s),
    UAVCAN_PROTOCOL_NODESTATUS_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_protocol_NodeStatus(struct uavcan_protocol_NodeStatus_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_PROTOCOL_NODESTATUS_MAX_PACK_SIZE);
    _encode_uavcan_protocol_NodeStatus(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_protocol_NodeStatus(const CanardRxTransfer* transfer, struct uavcan_protocol_NodeStatus_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_protocol_NodeStatus(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_protocol_NodeStatus(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_NodeStatus_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 32, &msg->uptime_sec);
    *bit_ofs += 32;
    canardEncodeScalar(buffer, *bit_ofs, 2, &msg->health);
    *bit_ofs += 2;
    canardEncodeScalar(buffer, *bit_ofs, 3, &msg->mode);
    *bit_ofs += 3;
    canardEncodeScalar(buffer, *bit_ofs, 3, &msg->sub_mode);
    *bit_ofs += 3;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->vendor_specific_status_code);
    *bit_ofs += 16;
}

void _decode_uavcan_protocol_NodeStatus(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_NodeStatus_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 32, false, &msg->uptime_sec);
    *bit_ofs += 32;
    canardDecodeScalar(transfer, *bit_ofs, 2, false, &msg->health);
    *bit_ofs += 2;
    canardDecodeScalar(transfer, *bit_ofs, 3, false, &msg->mode);
    *bit_ofs += 3;
    canardDecodeScalar(transfer, *bit_ofs, 3, false, &msg->sub_mode);
    *bit_ofs += 3;
    canardDecodeScalar(transfer, *bit_ofs, 16, false, &msg->vendor_specific_status_code);
    *bit_ofs += 16;
}
