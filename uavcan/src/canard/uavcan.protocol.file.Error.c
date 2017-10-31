#include <canard/uavcan.protocol.file.Error.h>
#include <string.h>


uint32_t encode_uavcan_protocol_file_Error(struct uavcan_protocol_file_Error_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_PROTOCOL_FILE_ERROR_MAX_PACK_SIZE);
    _encode_uavcan_protocol_file_Error(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_protocol_file_Error(const CanardRxTransfer* transfer, struct uavcan_protocol_file_Error_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_protocol_file_Error(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_protocol_file_Error(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_file_Error_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->value);
    *bit_ofs += 16;
}

void _decode_uavcan_protocol_file_Error(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_file_Error_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->value);
    *bit_ofs += 16;
}
