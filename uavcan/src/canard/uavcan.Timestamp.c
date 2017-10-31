#include <canard/uavcan.Timestamp.h>
#include <string.h>


uint32_t encode_uavcan_Timestamp(struct uavcan_Timestamp_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_TIMESTAMP_MAX_PACK_SIZE);
    _encode_uavcan_Timestamp(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_Timestamp(const CanardRxTransfer* transfer, struct uavcan_Timestamp_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_Timestamp(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_Timestamp(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_Timestamp_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 56, &msg->usec);
    *bit_ofs += 56;
}

void _decode_uavcan_Timestamp(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_Timestamp_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 56, false, &msg->usec);
    *bit_ofs += 56;
}
