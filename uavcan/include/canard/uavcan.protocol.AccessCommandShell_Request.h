#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_ACCESSCOMMANDSHELL_REQ_MAX_PACK_SIZE 130
#define UAVCAN_PROTOCOL_ACCESSCOMMANDSHELL_REQ_DT_SIG 0x59276B5921C9246E
#define UAVCAN_PROTOCOL_ACCESSCOMMANDSHELL_REQ_DT_ID 6

#define UAVCAN_PROTOCOL_ACCESSCOMMANDSHELL_REQ_NEWLINE 10
#define UAVCAN_PROTOCOL_ACCESSCOMMANDSHELL_REQ_MIN_OUTPUT_LIFETIME_SEC 10
#define UAVCAN_PROTOCOL_ACCESSCOMMANDSHELL_REQ_FLAG_RESET_SHELL 1
#define UAVCAN_PROTOCOL_ACCESSCOMMANDSHELL_REQ_FLAG_CLEAR_OUTPUT_BUFFERS 2
#define UAVCAN_PROTOCOL_ACCESSCOMMANDSHELL_REQ_FLAG_READ_STDOUT 64
#define UAVCAN_PROTOCOL_ACCESSCOMMANDSHELL_REQ_FLAG_READ_STDERR 128

struct uavcan_protocol_AccessCommandShell_req_s {
    uint8_t flags;
    struct {uint8_t input_len; uint8_t input[128];};
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_AccessCommandShell_req_descriptor;

uint32_t encode_uavcan_protocol_AccessCommandShell_req(struct uavcan_protocol_AccessCommandShell_req_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_AccessCommandShell_req(const CanardRxTransfer* transfer, struct uavcan_protocol_AccessCommandShell_req_s* msg);
void _encode_uavcan_protocol_AccessCommandShell_req(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_AccessCommandShell_req_s* msg, bool tao);
void _decode_uavcan_protocol_AccessCommandShell_req(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_AccessCommandShell_req_s* msg, bool tao);
