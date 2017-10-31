#include <canard/uavcan.protocol.enumeration.Indication.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_protocol_enumeration_Indication(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_protocol_enumeration_Indication(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_protocol_enumeration_Indication_descriptor = {
    UAVCAN_PROTOCOL_ENUMERATION_INDICATION_DT_SIG,
    UAVCAN_PROTOCOL_ENUMERATION_INDICATION_DT_ID,
    CanardTransferTypeBroadcast,
    sizeof(struct uavcan_protocol_enumeration_Indication_s),
    UAVCAN_PROTOCOL_ENUMERATION_INDICATION_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_protocol_enumeration_Indication(struct uavcan_protocol_enumeration_Indication_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_PROTOCOL_ENUMERATION_INDICATION_MAX_PACK_SIZE);
    _encode_uavcan_protocol_enumeration_Indication(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_protocol_enumeration_Indication(const CanardRxTransfer* transfer, struct uavcan_protocol_enumeration_Indication_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_protocol_enumeration_Indication(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_protocol_enumeration_Indication(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_enumeration_Indication_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    *bit_ofs += 6;
    _encode_uavcan_protocol_param_NumericValue(buffer, bit_ofs, &msg->value, false);
    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 7, &msg->parameter_name_len);
        *bit_ofs += 7;
    }
    for (size_t i=0; i < msg->parameter_name_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 8, &msg->parameter_name[i]);
            *bit_ofs += 8;
    }
}

void _decode_uavcan_protocol_enumeration_Indication(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_enumeration_Indication_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    *bit_ofs += 6;
    _decode_uavcan_protocol_param_NumericValue(transfer, bit_ofs, &msg->value, false);
    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 7, false, &msg->parameter_name_len);
        *bit_ofs += 7;
    }
    for (size_t i=0; i < msg->parameter_name_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->parameter_name[i]);
            *bit_ofs += 8;
    }
}
