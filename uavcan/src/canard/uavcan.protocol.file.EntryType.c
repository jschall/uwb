#include <canard/uavcan.protocol.file.EntryType.h>
#include <string.h>


uint32_t encode_uavcan_protocol_file_EntryType(struct uavcan_protocol_file_EntryType_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_PROTOCOL_FILE_ENTRYTYPE_MAX_PACK_SIZE);
    _encode_uavcan_protocol_file_EntryType(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_protocol_file_EntryType(const CanardRxTransfer* transfer, struct uavcan_protocol_file_EntryType_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_protocol_file_EntryType(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_protocol_file_EntryType(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_file_EntryType_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->flags);
    *bit_ofs += 8;
}

void _decode_uavcan_protocol_file_EntryType(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_file_EntryType_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->flags);
    *bit_ofs += 8;
}
