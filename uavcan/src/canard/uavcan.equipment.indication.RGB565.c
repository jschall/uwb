#include <canard/uavcan.equipment.indication.RGB565.h>
#include <string.h>


uint32_t encode_uavcan_equipment_indication_RGB565(struct uavcan_equipment_indication_RGB565_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_EQUIPMENT_INDICATION_RGB565_MAX_PACK_SIZE);
    _encode_uavcan_equipment_indication_RGB565(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_equipment_indication_RGB565(const CanardRxTransfer* transfer, struct uavcan_equipment_indication_RGB565_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_equipment_indication_RGB565(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_equipment_indication_RGB565(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_indication_RGB565_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 5, &msg->red);
    *bit_ofs += 5;
    canardEncodeScalar(buffer, *bit_ofs, 6, &msg->green);
    *bit_ofs += 6;
    canardEncodeScalar(buffer, *bit_ofs, 5, &msg->blue);
    *bit_ofs += 5;
}

void _decode_uavcan_equipment_indication_RGB565(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_indication_RGB565_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 5, false, &msg->red);
    *bit_ofs += 5;
    canardDecodeScalar(transfer, *bit_ofs, 6, false, &msg->green);
    *bit_ofs += 6;
    canardDecodeScalar(transfer, *bit_ofs, 5, false, &msg->blue);
    *bit_ofs += 5;
}
