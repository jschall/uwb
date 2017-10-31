#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.protocol.param.Empty.h>

#define UAVCAN_PROTOCOL_PARAM_NUMERICVALUE_MAX_PACK_SIZE 9
#define UAVCAN_PROTOCOL_PARAM_NUMERICVALUE_DT_SIG 0xDA6D6FEA22E3587

enum uavcan_protocol_param_NumericValue_type_t {
    UAVCAN_PROTOCOL_PARAM_NUMERICVALUE_TYPE_EMPTY,
    UAVCAN_PROTOCOL_PARAM_NUMERICVALUE_TYPE_INTEGER_VALUE,
    UAVCAN_PROTOCOL_PARAM_NUMERICVALUE_TYPE_REAL_VALUE,
};

struct uavcan_protocol_param_NumericValue_s {
    enum uavcan_protocol_param_NumericValue_type_t uavcan_protocol_param_NumericValue_type;
    union {
        struct uavcan_protocol_param_Empty_s empty;
        int64_t integer_value;
        float real_value;
    };
};


uint32_t encode_uavcan_protocol_param_NumericValue(struct uavcan_protocol_param_NumericValue_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_param_NumericValue(const CanardRxTransfer* transfer, struct uavcan_protocol_param_NumericValue_s* msg);
void _encode_uavcan_protocol_param_NumericValue(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_param_NumericValue_s* msg, bool tao);
void _decode_uavcan_protocol_param_NumericValue(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_param_NumericValue_s* msg, bool tao);
