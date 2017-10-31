#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_NODESTATUS_MAX_PACK_SIZE 7
#define UAVCAN_PROTOCOL_NODESTATUS_DT_SIG 0xF0868D0C1A7C6F1
#define UAVCAN_PROTOCOL_NODESTATUS_DT_ID 341

#define UAVCAN_PROTOCOL_NODESTATUS_MAX_BROADCASTING_PERIOD_MS 1000
#define UAVCAN_PROTOCOL_NODESTATUS_MIN_BROADCASTING_PERIOD_MS 2
#define UAVCAN_PROTOCOL_NODESTATUS_OFFLINE_TIMEOUT_MS 3000
#define UAVCAN_PROTOCOL_NODESTATUS_HEALTH_OK 0
#define UAVCAN_PROTOCOL_NODESTATUS_HEALTH_WARNING 1
#define UAVCAN_PROTOCOL_NODESTATUS_HEALTH_ERROR 2
#define UAVCAN_PROTOCOL_NODESTATUS_HEALTH_CRITICAL 3
#define UAVCAN_PROTOCOL_NODESTATUS_MODE_OPERATIONAL 0
#define UAVCAN_PROTOCOL_NODESTATUS_MODE_INITIALIZATION 1
#define UAVCAN_PROTOCOL_NODESTATUS_MODE_MAINTENANCE 2
#define UAVCAN_PROTOCOL_NODESTATUS_MODE_SOFTWARE_UPDATE 3
#define UAVCAN_PROTOCOL_NODESTATUS_MODE_OFFLINE 7

struct uavcan_protocol_NodeStatus_s {
    uint32_t uptime_sec;
    uint8_t health;
    uint8_t mode;
    uint8_t sub_mode;
    uint16_t vendor_specific_status_code;
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_NodeStatus_descriptor;

uint32_t encode_uavcan_protocol_NodeStatus(struct uavcan_protocol_NodeStatus_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_NodeStatus(const CanardRxTransfer* transfer, struct uavcan_protocol_NodeStatus_s* msg);
void _encode_uavcan_protocol_NodeStatus(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_NodeStatus_s* msg, bool tao);
void _decode_uavcan_protocol_NodeStatus(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_NodeStatus_s* msg, bool tao);
