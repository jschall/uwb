#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_FILE_PATH_MAX_PACK_SIZE 201
#define UAVCAN_PROTOCOL_FILE_PATH_DT_SIG 0x12AEFC50878A43E2

#define UAVCAN_PROTOCOL_FILE_PATH_SEPARATOR 47

struct uavcan_protocol_file_Path_s {
    struct {uint8_t path_len; uint8_t path[200];};
};


uint32_t encode_uavcan_protocol_file_Path(struct uavcan_protocol_file_Path_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_file_Path(const CanardRxTransfer* transfer, struct uavcan_protocol_file_Path_s* msg);
void _encode_uavcan_protocol_file_Path(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_file_Path_s* msg, bool tao);
void _decode_uavcan_protocol_file_Path(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_file_Path_s* msg, bool tao);
