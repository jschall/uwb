#include <canard/uavcan.protocol.param.Value.h>
#include <string.h>


uint32_t encode_uavcan_protocol_param_Value(struct uavcan_protocol_param_Value_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_PROTOCOL_PARAM_VALUE_MAX_PACK_SIZE);
    _encode_uavcan_protocol_param_Value(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_protocol_param_Value(const CanardRxTransfer* transfer, struct uavcan_protocol_param_Value_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_protocol_param_Value(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_protocol_param_Value(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_param_Value_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    uint8_t uavcan_protocol_param_Value_type = msg->uavcan_protocol_param_Value_type;
    canardEncodeScalar(buffer, *bit_ofs, 3, &uavcan_protocol_param_Value_type);
    *bit_ofs += 3;

    switch(msg->uavcan_protocol_param_Value_type) {
        case UAVCAN_PROTOCOL_PARAM_VALUE_TYPE_EMPTY: {
            _encode_uavcan_protocol_param_Empty(buffer, bit_ofs, &msg->empty, false);
            break;
        }
        case UAVCAN_PROTOCOL_PARAM_VALUE_TYPE_INTEGER_VALUE: {
            canardEncodeScalar(buffer, *bit_ofs, 64, &msg->integer_value);
            *bit_ofs += 64;
            break;
        }
        case UAVCAN_PROTOCOL_PARAM_VALUE_TYPE_REAL_VALUE: {
            canardEncodeScalar(buffer, *bit_ofs, 32, &msg->real_value);
            *bit_ofs += 32;
            break;
        }
        case UAVCAN_PROTOCOL_PARAM_VALUE_TYPE_BOOLEAN_VALUE: {
            canardEncodeScalar(buffer, *bit_ofs, 8, &msg->boolean_value);
            *bit_ofs += 8;
            break;
        }
        case UAVCAN_PROTOCOL_PARAM_VALUE_TYPE_STRING_VALUE: {
            if (!tao) {
                canardEncodeScalar(buffer, *bit_ofs, 8, &msg->string_value_len);
                *bit_ofs += 8;
            }
            for (size_t i=0; i < msg->string_value_len; i++) {
                    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->string_value[i]);
                    *bit_ofs += 8;
            }
            break;
        }
    }
}

void _decode_uavcan_protocol_param_Value(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_param_Value_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    uint8_t uavcan_protocol_param_Value_type;
    canardDecodeScalar(transfer, *bit_ofs, 3, false, &uavcan_protocol_param_Value_type);
    msg->uavcan_protocol_param_Value_type = uavcan_protocol_param_Value_type;
    *bit_ofs += 3;

    switch(msg->uavcan_protocol_param_Value_type) {
        case UAVCAN_PROTOCOL_PARAM_VALUE_TYPE_EMPTY: {
            _decode_uavcan_protocol_param_Empty(transfer, bit_ofs, &msg->empty, false);
            break;
        }
        case UAVCAN_PROTOCOL_PARAM_VALUE_TYPE_INTEGER_VALUE: {
            canardDecodeScalar(transfer, *bit_ofs, 64, true, &msg->integer_value);
            *bit_ofs += 64;
            break;
        }
        case UAVCAN_PROTOCOL_PARAM_VALUE_TYPE_REAL_VALUE: {
            canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->real_value);
            *bit_ofs += 32;
            break;
        }
        case UAVCAN_PROTOCOL_PARAM_VALUE_TYPE_BOOLEAN_VALUE: {
            canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->boolean_value);
            *bit_ofs += 8;
            break;
        }
        case UAVCAN_PROTOCOL_PARAM_VALUE_TYPE_STRING_VALUE: {
            if (!tao) {
                canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->string_value_len);
                *bit_ofs += 8;
            }
            for (size_t i=0; i < msg->string_value_len; i++) {
                    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->string_value[i]);
                    *bit_ofs += 8;
            }
            break;
        }
    }
}
