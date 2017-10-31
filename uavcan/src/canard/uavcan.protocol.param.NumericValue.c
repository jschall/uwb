#include <canard/uavcan.protocol.param.NumericValue.h>
#include <string.h>


uint32_t encode_uavcan_protocol_param_NumericValue(struct uavcan_protocol_param_NumericValue_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_PROTOCOL_PARAM_NUMERICVALUE_MAX_PACK_SIZE);
    _encode_uavcan_protocol_param_NumericValue(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_protocol_param_NumericValue(const CanardRxTransfer* transfer, struct uavcan_protocol_param_NumericValue_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_protocol_param_NumericValue(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_protocol_param_NumericValue(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_param_NumericValue_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    uint8_t uavcan_protocol_param_NumericValue_type = msg->uavcan_protocol_param_NumericValue_type;
    canardEncodeScalar(buffer, *bit_ofs, 2, &uavcan_protocol_param_NumericValue_type);
    *bit_ofs += 2;

    switch(msg->uavcan_protocol_param_NumericValue_type) {
        case UAVCAN_PROTOCOL_PARAM_NUMERICVALUE_TYPE_EMPTY: {
            _encode_uavcan_protocol_param_Empty(buffer, bit_ofs, &msg->empty, false);
            break;
        }
        case UAVCAN_PROTOCOL_PARAM_NUMERICVALUE_TYPE_INTEGER_VALUE: {
            canardEncodeScalar(buffer, *bit_ofs, 64, &msg->integer_value);
            *bit_ofs += 64;
            break;
        }
        case UAVCAN_PROTOCOL_PARAM_NUMERICVALUE_TYPE_REAL_VALUE: {
            canardEncodeScalar(buffer, *bit_ofs, 32, &msg->real_value);
            *bit_ofs += 32;
            break;
        }
    }
}

void _decode_uavcan_protocol_param_NumericValue(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_param_NumericValue_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    uint8_t uavcan_protocol_param_NumericValue_type;
    canardDecodeScalar(transfer, *bit_ofs, 2, false, &uavcan_protocol_param_NumericValue_type);
    msg->uavcan_protocol_param_NumericValue_type = uavcan_protocol_param_NumericValue_type;
    *bit_ofs += 2;

    switch(msg->uavcan_protocol_param_NumericValue_type) {
        case UAVCAN_PROTOCOL_PARAM_NUMERICVALUE_TYPE_EMPTY: {
            _decode_uavcan_protocol_param_Empty(transfer, bit_ofs, &msg->empty, false);
            break;
        }
        case UAVCAN_PROTOCOL_PARAM_NUMERICVALUE_TYPE_INTEGER_VALUE: {
            canardDecodeScalar(transfer, *bit_ofs, 64, true, &msg->integer_value);
            *bit_ofs += 64;
            break;
        }
        case UAVCAN_PROTOCOL_PARAM_NUMERICVALUE_TYPE_REAL_VALUE: {
            canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->real_value);
            *bit_ofs += 32;
            break;
        }
    }
}
