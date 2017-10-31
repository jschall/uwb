#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_ENUMERATION_BEGIN_RES_MAX_PACK_SIZE 1
#define UAVCAN_PROTOCOL_ENUMERATION_BEGIN_RES_DT_SIG 0x196AE06426A3B5D8
#define UAVCAN_PROTOCOL_ENUMERATION_BEGIN_RES_DT_ID 15

#define UAVCAN_PROTOCOL_ENUMERATION_BEGIN_RES_ERROR_OK 0
#define UAVCAN_PROTOCOL_ENUMERATION_BEGIN_RES_ERROR_INVALID_MODE 1
#define UAVCAN_PROTOCOL_ENUMERATION_BEGIN_RES_ERROR_INVALID_PARAMETER 2
#define UAVCAN_PROTOCOL_ENUMERATION_BEGIN_RES_ERROR_UNSUPPORTED 3
#define UAVCAN_PROTOCOL_ENUMERATION_BEGIN_RES_ERROR_UNKNOWN 255

struct uavcan_protocol_enumeration_Begin_res_s {
    uint8_t error;
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_enumeration_Begin_res_descriptor;

uint32_t encode_uavcan_protocol_enumeration_Begin_res(struct uavcan_protocol_enumeration_Begin_res_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_enumeration_Begin_res(const CanardRxTransfer* transfer, struct uavcan_protocol_enumeration_Begin_res_s* msg);
void _encode_uavcan_protocol_enumeration_Begin_res(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_enumeration_Begin_res_s* msg, bool tao);
void _decode_uavcan_protocol_enumeration_Begin_res(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_enumeration_Begin_res_s* msg, bool tao);
