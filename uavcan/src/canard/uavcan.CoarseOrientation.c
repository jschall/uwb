#include <canard/uavcan.CoarseOrientation.h>
#include <string.h>


uint32_t encode_uavcan_CoarseOrientation(struct uavcan_CoarseOrientation_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_COARSEORIENTATION_MAX_PACK_SIZE);
    _encode_uavcan_CoarseOrientation(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_CoarseOrientation(const CanardRxTransfer* transfer, struct uavcan_CoarseOrientation_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_CoarseOrientation(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_CoarseOrientation(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_CoarseOrientation_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    for (size_t i=0; i < 3; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 5, &msg->fixed_axis_roll_pitch_yaw[i]);
            *bit_ofs += 5;
    }
    canardEncodeScalar(buffer, *bit_ofs, 1, &msg->orientation_defined);
    *bit_ofs += 1;
}

void _decode_uavcan_CoarseOrientation(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_CoarseOrientation_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    for (size_t i=0; i < 3; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 5, true, &msg->fixed_axis_roll_pitch_yaw[i]);
            *bit_ofs += 5;
    }
    canardDecodeScalar(transfer, *bit_ofs, 1, false, &msg->orientation_defined);
    *bit_ofs += 1;
}
