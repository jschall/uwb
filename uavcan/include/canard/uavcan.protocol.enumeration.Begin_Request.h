#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_ENUMERATION_BEGIN_REQ_MAX_PACK_SIZE 95
#define UAVCAN_PROTOCOL_ENUMERATION_BEGIN_REQ_DT_SIG 0x196AE06426A3B5D8
#define UAVCAN_PROTOCOL_ENUMERATION_BEGIN_REQ_DT_ID 15

#define UAVCAN_PROTOCOL_ENUMERATION_BEGIN_REQ_TIMEOUT_CANCEL 0
#define UAVCAN_PROTOCOL_ENUMERATION_BEGIN_REQ_TIMEOUT_INFINITE 65535

struct uavcan_protocol_enumeration_Begin_req_s {
    uint16_t timeout_sec;
    struct {uint8_t parameter_name_len; uint8_t parameter_name[92];};
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_enumeration_Begin_req_descriptor;

uint32_t encode_uavcan_protocol_enumeration_Begin_req(struct uavcan_protocol_enumeration_Begin_req_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_enumeration_Begin_req(const CanardRxTransfer* transfer, struct uavcan_protocol_enumeration_Begin_req_s* msg);
void _encode_uavcan_protocol_enumeration_Begin_req(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_enumeration_Begin_req_s* msg, bool tao);
void _decode_uavcan_protocol_enumeration_Begin_req(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_enumeration_Begin_req_s* msg, bool tao);
