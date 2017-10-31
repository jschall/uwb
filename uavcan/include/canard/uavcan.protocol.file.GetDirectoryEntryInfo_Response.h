#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>

#include <canard/uavcan.protocol.file.EntryType.h>
#include <canard/uavcan.protocol.file.Error.h>
#include <canard/uavcan.protocol.file.Path.h>

#define UAVCAN_PROTOCOL_FILE_GETDIRECTORYENTRYINFO_RES_MAX_PACK_SIZE 204
#define UAVCAN_PROTOCOL_FILE_GETDIRECTORYENTRYINFO_RES_DT_SIG 0x8C46E8AB568BDA79
#define UAVCAN_PROTOCOL_FILE_GETDIRECTORYENTRYINFO_RES_DT_ID 46

struct uavcan_protocol_file_GetDirectoryEntryInfo_res_s {
    struct uavcan_protocol_file_Error_s error;
    struct uavcan_protocol_file_EntryType_s entry_type;
    struct uavcan_protocol_file_Path_s entry_full_path;
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_file_GetDirectoryEntryInfo_res_descriptor;

uint32_t encode_uavcan_protocol_file_GetDirectoryEntryInfo_res(struct uavcan_protocol_file_GetDirectoryEntryInfo_res_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_file_GetDirectoryEntryInfo_res(const CanardRxTransfer* transfer, struct uavcan_protocol_file_GetDirectoryEntryInfo_res_s* msg);
void _encode_uavcan_protocol_file_GetDirectoryEntryInfo_res(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_file_GetDirectoryEntryInfo_res_s* msg, bool tao);
void _decode_uavcan_protocol_file_GetDirectoryEntryInfo_res(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_file_GetDirectoryEntryInfo_res_s* msg, bool tao);
