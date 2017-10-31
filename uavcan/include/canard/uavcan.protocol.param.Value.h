#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.protocol.param.Empty.h>

#define UAVCAN_PROTOCOL_PARAM_VALUE_MAX_PACK_SIZE 130
#define UAVCAN_PROTOCOL_PARAM_VALUE_DT_SIG 0x29F14BF484727267

enum uavcan_protocol_param_Value_type_t {
    UAVCAN_PROTOCOL_PARAM_VALUE_TYPE_EMPTY,
    UAVCAN_PROTOCOL_PARAM_VALUE_TYPE_INTEGER_VALUE,
    UAVCAN_PROTOCOL_PARAM_VALUE_TYPE_REAL_VALUE,
    UAVCAN_PROTOCOL_PARAM_VALUE_TYPE_BOOLEAN_VALUE,
    UAVCAN_PROTOCOL_PARAM_VALUE_TYPE_STRING_VALUE,
};

struct uavcan_protocol_param_Value_s {
    enum uavcan_protocol_param_Value_type_t uavcan_protocol_param_Value_type;
    union {
        struct uavcan_protocol_param_Empty_s empty;
        int64_t integer_value;
        float real_value;
        uint8_t boolean_value;
        struct {uint8_t string_value_len; uint8_t string_value[128];};
    };
};


uint32_t encode_uavcan_protocol_param_Value(struct uavcan_protocol_param_Value_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_param_Value(const CanardRxTransfer* transfer, struct uavcan_protocol_param_Value_s* msg);
void _encode_uavcan_protocol_param_Value(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_param_Value_s* msg, bool tao);
void _decode_uavcan_protocol_param_Value(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_param_Value_s* msg, bool tao);
