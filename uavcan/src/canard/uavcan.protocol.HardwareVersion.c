#include <canard/uavcan.protocol.HardwareVersion.h>
#include <string.h>


uint32_t encode_uavcan_protocol_HardwareVersion(struct uavcan_protocol_HardwareVersion_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_PROTOCOL_HARDWAREVERSION_MAX_PACK_SIZE);
    _encode_uavcan_protocol_HardwareVersion(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_protocol_HardwareVersion(const CanardRxTransfer* transfer, struct uavcan_protocol_HardwareVersion_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_protocol_HardwareVersion(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_protocol_HardwareVersion(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_HardwareVersion_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->major);
    *bit_ofs += 8;
    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->minor);
    *bit_ofs += 8;
    for (size_t i=0; i < 16; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 8, &msg->unique_id[i]);
            *bit_ofs += 8;
    }
    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 8, &msg->certificate_of_authenticity_len);
        *bit_ofs += 8;
    }
    for (size_t i=0; i < msg->certificate_of_authenticity_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 8, &msg->certificate_of_authenticity[i]);
            *bit_ofs += 8;
    }
}

void _decode_uavcan_protocol_HardwareVersion(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_HardwareVersion_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->major);
    *bit_ofs += 8;
    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->minor);
    *bit_ofs += 8;
    for (size_t i=0; i < 16; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->unique_id[i]);
            *bit_ofs += 8;
    }
    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->certificate_of_authenticity_len);
        *bit_ofs += 8;
    }
    for (size_t i=0; i < msg->certificate_of_authenticity_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->certificate_of_authenticity[i]);
            *bit_ofs += 8;
    }
}
