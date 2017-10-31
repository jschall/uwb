#include <canard/uavcan.protocol.GlobalTimeSync.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_protocol_GlobalTimeSync(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_protocol_GlobalTimeSync(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_protocol_GlobalTimeSync_descriptor = {
    UAVCAN_PROTOCOL_GLOBALTIMESYNC_DT_SIG,
    UAVCAN_PROTOCOL_GLOBALTIMESYNC_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_protocol_GlobalTimeSync_s),
    UAVCAN_PROTOCOL_GLOBALTIMESYNC_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_protocol_GlobalTimeSync(struct uavcan_protocol_GlobalTimeSync_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_PROTOCOL_GLOBALTIMESYNC_MAX_PACK_SIZE);
    _encode_uavcan_protocol_GlobalTimeSync(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_protocol_GlobalTimeSync(const CanardRxTransfer* transfer, struct uavcan_protocol_GlobalTimeSync_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_protocol_GlobalTimeSync(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_protocol_GlobalTimeSync(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_GlobalTimeSync_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 56, &msg->previous_transmission_timestamp_usec);
    *bit_ofs += 56;
}

void _decode_uavcan_protocol_GlobalTimeSync(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_GlobalTimeSync_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 56, false, &msg->previous_transmission_timestamp_usec);
    *bit_ofs += 56;
}
