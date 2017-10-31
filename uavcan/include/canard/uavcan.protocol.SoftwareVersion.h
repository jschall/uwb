#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_SOFTWAREVERSION_MAX_PACK_SIZE 15
#define UAVCAN_PROTOCOL_SOFTWAREVERSION_DT_SIG 0xDD46FD376527FEA1

#define UAVCAN_PROTOCOL_SOFTWAREVERSION_OPTIONAL_FIELD_FLAG_VCS_COMMIT 1
#define UAVCAN_PROTOCOL_SOFTWAREVERSION_OPTIONAL_FIELD_FLAG_IMAGE_CRC 2

struct uavcan_protocol_SoftwareVersion_s {
    uint8_t major;
    uint8_t minor;
    uint8_t optional_field_flags;
    uint32_t vcs_commit;
    uint64_t image_crc;
};


uint32_t encode_uavcan_protocol_SoftwareVersion(struct uavcan_protocol_SoftwareVersion_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_SoftwareVersion(const CanardRxTransfer* transfer, struct uavcan_protocol_SoftwareVersion_s* msg);
void _encode_uavcan_protocol_SoftwareVersion(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_SoftwareVersion_s* msg, bool tao);
void _decode_uavcan_protocol_SoftwareVersion(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_SoftwareVersion_s* msg, bool tao);
