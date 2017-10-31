#include <canard/uavcan.protocol.file.Path.h>
#include <string.h>


uint32_t encode_uavcan_protocol_file_Path(struct uavcan_protocol_file_Path_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_PROTOCOL_FILE_PATH_MAX_PACK_SIZE);
    _encode_uavcan_protocol_file_Path(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_protocol_file_Path(const CanardRxTransfer* transfer, struct uavcan_protocol_file_Path_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_protocol_file_Path(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_protocol_file_Path(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_file_Path_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 8, &msg->path_len);
        *bit_ofs += 8;
    }
    for (size_t i=0; i < msg->path_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 8, &msg->path[i]);
            *bit_ofs += 8;
    }
}

void _decode_uavcan_protocol_file_Path(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_file_Path_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->path_len);
        *bit_ofs += 8;
    }
    for (size_t i=0; i < msg->path_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->path[i]);
            *bit_ofs += 8;
    }
}
