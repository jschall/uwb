#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.protocol.file.Path.h>

#define UAVCAN_PROTOCOL_FILE_GETDIRECTORYENTRYINFO_REQ_MAX_PACK_SIZE 205
#define UAVCAN_PROTOCOL_FILE_GETDIRECTORYENTRYINFO_REQ_DT_SIG 0x8C46E8AB568BDA79
#define UAVCAN_PROTOCOL_FILE_GETDIRECTORYENTRYINFO_REQ_DT_ID 46

struct uavcan_protocol_file_GetDirectoryEntryInfo_req_s {
    uint32_t entry_index;
    struct uavcan_protocol_file_Path_s directory_path;
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_file_GetDirectoryEntryInfo_req_descriptor;

uint32_t encode_uavcan_protocol_file_GetDirectoryEntryInfo_req(struct uavcan_protocol_file_GetDirectoryEntryInfo_req_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_file_GetDirectoryEntryInfo_req(const CanardRxTransfer* transfer, struct uavcan_protocol_file_GetDirectoryEntryInfo_req_s* msg);
void _encode_uavcan_protocol_file_GetDirectoryEntryInfo_req(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_file_GetDirectoryEntryInfo_req_s* msg, bool tao);
void _decode_uavcan_protocol_file_GetDirectoryEntryInfo_req(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_file_GetDirectoryEntryInfo_req_s* msg, bool tao);
