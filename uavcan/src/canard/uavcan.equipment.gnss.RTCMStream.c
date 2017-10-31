#include <canard/uavcan.equipment.gnss.RTCMStream.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_equipment_gnss_RTCMStream(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_equipment_gnss_RTCMStream(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_equipment_gnss_RTCMStream_descriptor = {
    UAVCAN_EQUIPMENT_GNSS_RTCMSTREAM_DT_SIG,
    UAVCAN_EQUIPMENT_GNSS_RTCMSTREAM_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_equipment_gnss_RTCMStream_s),
    UAVCAN_EQUIPMENT_GNSS_RTCMSTREAM_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_equipment_gnss_RTCMStream(struct uavcan_equipment_gnss_RTCMStream_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_GNSS_RTCMSTREAM_MAX_PACK_SIZE);
    _encode_uavcan_equipment_gnss_RTCMStream(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_gnss_RTCMStream(const CanardRxTransfer* transfer, struct uavcan_equipment_gnss_RTCMStream_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_gnss_RTCMStream(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_gnss_RTCMStream(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_gnss_RTCMStream_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->protocol_id);
    *bit_ofs += 8;
    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 8, &msg->data_len);
        *bit_ofs += 8;
    }
    for (size_t i=0; i < msg->data_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 8, &msg->data[i]);
            *bit_ofs += 8;
    }
}

void _decode_uavcan_equipment_gnss_RTCMStream(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_gnss_RTCMStream_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->protocol_id);
    *bit_ofs += 8;
    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->data_len);
        *bit_ofs += 8;
    }
    for (size_t i=0; i < msg->data_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->data[i]);
            *bit_ofs += 8;
    }
}
