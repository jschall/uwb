#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_FILE_ERROR_MAX_PACK_SIZE 2
#define UAVCAN_PROTOCOL_FILE_ERROR_DT_SIG 0xA83071FFEA4FAE15

#define UAVCAN_PROTOCOL_FILE_ERROR_OK 0
#define UAVCAN_PROTOCOL_FILE_ERROR_UNKNOWN_ERROR 32767
#define UAVCAN_PROTOCOL_FILE_ERROR_NOT_FOUND 2
#define UAVCAN_PROTOCOL_FILE_ERROR_IO_ERROR 5
#define UAVCAN_PROTOCOL_FILE_ERROR_ACCESS_DENIED 13
#define UAVCAN_PROTOCOL_FILE_ERROR_IS_DIRECTORY 21
#define UAVCAN_PROTOCOL_FILE_ERROR_INVALID_VALUE 22
#define UAVCAN_PROTOCOL_FILE_ERROR_FILE_TOO_LARGE 27
#define UAVCAN_PROTOCOL_FILE_ERROR_OUT_OF_SPACE 28
#define UAVCAN_PROTOCOL_FILE_ERROR_NOT_IMPLEMENTED 38

struct uavcan_protocol_file_Error_s {
    int16_t value;
};


uint32_t encode_uavcan_protocol_file_Error(struct uavcan_protocol_file_Error_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_file_Error(const CanardRxTransfer* transfer, struct uavcan_protocol_file_Error_s* msg);
void _encode_uavcan_protocol_file_Error(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_file_Error_s* msg, bool tao);
void _decode_uavcan_protocol_file_Error(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_file_Error_s* msg, bool tao);
