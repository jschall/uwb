#include <canard/uavcan.protocol.SoftwareVersion.h>
#include <string.h>


uint32_t encode_uavcan_protocol_SoftwareVersion(struct uavcan_protocol_SoftwareVersion_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_PROTOCOL_SOFTWAREVERSION_MAX_PACK_SIZE);
    _encode_uavcan_protocol_SoftwareVersion(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_protocol_SoftwareVersion(const CanardRxTransfer* transfer, struct uavcan_protocol_SoftwareVersion_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_protocol_SoftwareVersion(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_protocol_SoftwareVersion(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_SoftwareVersion_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->major);
    *bit_ofs += 8;
    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->minor);
    *bit_ofs += 8;
    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->optional_field_flags);
    *bit_ofs += 8;
    canardEncodeScalar(buffer, *bit_ofs, 32, &msg->vcs_commit);
    *bit_ofs += 32;
    canardEncodeScalar(buffer, *bit_ofs, 64, &msg->image_crc);
    *bit_ofs += 64;
}

void _decode_uavcan_protocol_SoftwareVersion(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_SoftwareVersion_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->major);
    *bit_ofs += 8;
    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->minor);
    *bit_ofs += 8;
    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->optional_field_flags);
    *bit_ofs += 8;
    canardDecodeScalar(transfer, *bit_ofs, 32, false, &msg->vcs_commit);
    *bit_ofs += 32;
    canardDecodeScalar(transfer, *bit_ofs, 64, false, &msg->image_crc);
    *bit_ofs += 64;
}
