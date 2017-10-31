#include <canard/uavcan.protocol.param.Empty.h>
#include <string.h>


uint32_t encode_uavcan_protocol_param_Empty(struct uavcan_protocol_param_Empty_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_PROTOCOL_PARAM_EMPTY_MAX_PACK_SIZE);
    _encode_uavcan_protocol_param_Empty(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_protocol_param_Empty(const CanardRxTransfer* transfer, struct uavcan_protocol_param_Empty_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_protocol_param_Empty(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_protocol_param_Empty(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_param_Empty_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

}

void _decode_uavcan_protocol_param_Empty(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_param_Empty_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

}
