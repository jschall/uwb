#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_FILE_ENTRYTYPE_MAX_PACK_SIZE 1
#define UAVCAN_PROTOCOL_FILE_ENTRYTYPE_DT_SIG 0x6924572FBB2086E5

#define UAVCAN_PROTOCOL_FILE_ENTRYTYPE_FLAG_FILE 1
#define UAVCAN_PROTOCOL_FILE_ENTRYTYPE_FLAG_DIRECTORY 2
#define UAVCAN_PROTOCOL_FILE_ENTRYTYPE_FLAG_SYMLINK 4
#define UAVCAN_PROTOCOL_FILE_ENTRYTYPE_FLAG_READABLE 8
#define UAVCAN_PROTOCOL_FILE_ENTRYTYPE_FLAG_WRITEABLE 16

struct uavcan_protocol_file_EntryType_s {
    uint8_t flags;
};


uint32_t encode_uavcan_protocol_file_EntryType(struct uavcan_protocol_file_EntryType_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_file_EntryType(const CanardRxTransfer* transfer, struct uavcan_protocol_file_EntryType_s* msg);
void _encode_uavcan_protocol_file_EntryType(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_file_EntryType_s* msg, bool tao);
void _decode_uavcan_protocol_file_EntryType(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_file_EntryType_s* msg, bool tao);
