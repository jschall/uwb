#include <canard/uavcan.protocol.debug.KeyValue.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_protocol_debug_KeyValue(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_protocol_debug_KeyValue(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_protocol_debug_KeyValue_descriptor = {
    UAVCAN_PROTOCOL_DEBUG_KEYVALUE_DT_SIG,
    UAVCAN_PROTOCOL_DEBUG_KEYVALUE_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_protocol_debug_KeyValue_s),
    UAVCAN_PROTOCOL_DEBUG_KEYVALUE_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_protocol_debug_KeyValue(struct uavcan_protocol_debug_KeyValue_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_PROTOCOL_DEBUG_KEYVALUE_MAX_PACK_SIZE);
    _encode_uavcan_protocol_debug_KeyValue(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_protocol_debug_KeyValue(const CanardRxTransfer* transfer, struct uavcan_protocol_debug_KeyValue_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_protocol_debug_KeyValue(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_protocol_debug_KeyValue(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_debug_KeyValue_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 32, &msg->value);
    *bit_ofs += 32;
    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 6, &msg->key_len);
        *bit_ofs += 6;
    }
    for (size_t i=0; i < msg->key_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 8, &msg->key[i]);
            *bit_ofs += 8;
    }
}

void _decode_uavcan_protocol_debug_KeyValue(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_debug_KeyValue_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->value);
    *bit_ofs += 32;
    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 6, false, &msg->key_len);
        *bit_ofs += 6;
    }
    for (size_t i=0; i < msg->key_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->key[i]);
            *bit_ofs += 8;
    }
}
