#include <canard/uavcan.protocol.CANIfaceStats.h>
#include <string.h>


uint32_t encode_uavcan_protocol_CANIfaceStats(struct uavcan_protocol_CANIfaceStats_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_PROTOCOL_CANIFACESTATS_MAX_PACK_SIZE);
    _encode_uavcan_protocol_CANIfaceStats(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_protocol_CANIfaceStats(const CanardRxTransfer* transfer, struct uavcan_protocol_CANIfaceStats_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_protocol_CANIfaceStats(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_protocol_CANIfaceStats(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_CANIfaceStats_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 48, &msg->frames_tx);
    *bit_ofs += 48;
    canardEncodeScalar(buffer, *bit_ofs, 48, &msg->frames_rx);
    *bit_ofs += 48;
    canardEncodeScalar(buffer, *bit_ofs, 48, &msg->errors);
    *bit_ofs += 48;
}

void _decode_uavcan_protocol_CANIfaceStats(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_CANIfaceStats_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 48, false, &msg->frames_tx);
    *bit_ofs += 48;
    canardDecodeScalar(transfer, *bit_ofs, 48, false, &msg->frames_rx);
    *bit_ofs += 48;
    canardDecodeScalar(transfer, *bit_ofs, 48, false, &msg->errors);
    *bit_ofs += 48;
}
