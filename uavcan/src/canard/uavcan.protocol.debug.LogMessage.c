#include <canard/uavcan.protocol.debug.LogMessage.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_protocol_debug_LogMessage(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_protocol_debug_LogMessage(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_protocol_debug_LogMessage_descriptor = {
    UAVCAN_PROTOCOL_DEBUG_LOGMESSAGE_DT_SIG,
    UAVCAN_PROTOCOL_DEBUG_LOGMESSAGE_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_protocol_debug_LogMessage_s),
    UAVCAN_PROTOCOL_DEBUG_LOGMESSAGE_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_protocol_debug_LogMessage(struct uavcan_protocol_debug_LogMessage_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_PROTOCOL_DEBUG_LOGMESSAGE_MAX_PACK_SIZE);
    _encode_uavcan_protocol_debug_LogMessage(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_protocol_debug_LogMessage(const CanardRxTransfer* transfer, struct uavcan_protocol_debug_LogMessage_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_protocol_debug_LogMessage(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_protocol_debug_LogMessage(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_debug_LogMessage_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    _encode_uavcan_protocol_debug_LogLevel(buffer, bit_ofs, &msg->level, false);
    canardEncodeScalar(buffer, *bit_ofs, 5, &msg->source_len);
    *bit_ofs += 5;
    for (size_t i=0; i < msg->source_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 8, &msg->source[i]);
            *bit_ofs += 8;
    }
    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 7, &msg->text_len);
        *bit_ofs += 7;
    }
    for (size_t i=0; i < msg->text_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 8, &msg->text[i]);
            *bit_ofs += 8;
    }
}

void _decode_uavcan_protocol_debug_LogMessage(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_debug_LogMessage_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    _decode_uavcan_protocol_debug_LogLevel(transfer, bit_ofs, &msg->level, false);
    canardDecodeScalar(transfer, *bit_ofs, 5, false, &msg->source_len);
    *bit_ofs += 5;
    for (size_t i=0; i < msg->source_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->source[i]);
            *bit_ofs += 8;
    }
    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 7, false, &msg->text_len);
        *bit_ofs += 7;
    }
    for (size_t i=0; i < msg->text_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->text[i]);
            *bit_ofs += 8;
    }
}
