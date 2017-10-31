#include <canard/uavcan.protocol.Panic.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_protocol_Panic(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_protocol_Panic(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_protocol_Panic_descriptor = {
    UAVCAN_PROTOCOL_PANIC_DT_SIG,
    UAVCAN_PROTOCOL_PANIC_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_protocol_Panic_s),
    UAVCAN_PROTOCOL_PANIC_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_protocol_Panic(struct uavcan_protocol_Panic_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_PROTOCOL_PANIC_MAX_PACK_SIZE);
    _encode_uavcan_protocol_Panic(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_protocol_Panic(const CanardRxTransfer* transfer, struct uavcan_protocol_Panic_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_protocol_Panic(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_protocol_Panic(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_Panic_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 3, &msg->reason_text_len);
        *bit_ofs += 3;
    }
    for (size_t i=0; i < msg->reason_text_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 8, &msg->reason_text[i]);
            *bit_ofs += 8;
    }
}

void _decode_uavcan_protocol_Panic(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_Panic_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 3, false, &msg->reason_text_len);
        *bit_ofs += 3;
    }
    for (size_t i=0; i < msg->reason_text_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->reason_text[i]);
            *bit_ofs += 8;
    }
}
