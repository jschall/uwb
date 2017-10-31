#include <canard/uavcan.equipment.gnss.Auxiliary.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_equipment_gnss_Auxiliary(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_equipment_gnss_Auxiliary(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_equipment_gnss_Auxiliary_descriptor = {
    UAVCAN_EQUIPMENT_GNSS_AUXILIARY_DT_SIG,
    UAVCAN_EQUIPMENT_GNSS_AUXILIARY_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_equipment_gnss_Auxiliary_s),
    UAVCAN_EQUIPMENT_GNSS_AUXILIARY_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_equipment_gnss_Auxiliary(struct uavcan_equipment_gnss_Auxiliary_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_GNSS_AUXILIARY_MAX_PACK_SIZE);
    _encode_uavcan_equipment_gnss_Auxiliary(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_gnss_Auxiliary(const CanardRxTransfer* transfer, struct uavcan_equipment_gnss_Auxiliary_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_gnss_Auxiliary(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_gnss_Auxiliary(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_gnss_Auxiliary_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->gdop);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->pdop);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->hdop);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->vdop);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->tdop);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->ndop);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->edop);
    *bit_ofs += 16;
    canardEncodeScalar(buffer, *bit_ofs, 7, &msg->sats_visible);
    *bit_ofs += 7;
    canardEncodeScalar(buffer, *bit_ofs, 6, &msg->sats_used);
    *bit_ofs += 6;
}

void _decode_uavcan_equipment_gnss_Auxiliary(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_gnss_Auxiliary_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->gdop);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->pdop);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->hdop);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->vdop);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->tdop);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->ndop);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 16, true, &msg->edop);
    *bit_ofs += 16;
    canardDecodeScalar(transfer, *bit_ofs, 7, false, &msg->sats_visible);
    *bit_ofs += 7;
    canardDecodeScalar(transfer, *bit_ofs, 6, false, &msg->sats_used);
    *bit_ofs += 6;
}
