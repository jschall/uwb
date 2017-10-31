#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_MAX_PACK_SIZE 18
#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_DT_SIG 0xB2A812620A11D40
#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_DT_ID 1

#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_MAX_REQUEST_PERIOD_MS 1000
#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_MIN_REQUEST_PERIOD_MS 600
#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_MAX_FOLLOWUP_DELAY_MS 400
#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_MIN_FOLLOWUP_DELAY_MS 0
#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_FOLLOWUP_TIMEOUT_MS 500
#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_MAX_LENGTH_OF_UNIQUE_ID_IN_REQUEST 6
#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_ANY_NODE_ID 0

struct uavcan_protocol_dynamic_node_id_Allocation_s {
    uint8_t node_id;
    bool first_part_of_unique_id;
    struct {uint8_t unique_id_len; uint8_t unique_id[16];};
};

extern const struct uavcan_message_descriptor_s uavcan_protocol_dynamic_node_id_Allocation_descriptor;

uint32_t encode_uavcan_protocol_dynamic_node_id_Allocation(struct uavcan_protocol_dynamic_node_id_Allocation_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_protocol_dynamic_node_id_Allocation(const CanardRxTransfer* transfer, struct uavcan_protocol_dynamic_node_id_Allocation_s* msg);
void _encode_uavcan_protocol_dynamic_node_id_Allocation(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_dynamic_node_id_Allocation_s* msg, bool tao);
void _decode_uavcan_protocol_dynamic_node_id_Allocation(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_dynamic_node_id_Allocation_s* msg, bool tao);
