#include <canard/uavcan.protocol.GetNodeInfo_Response.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_protocol_GetNodeInfo_res(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_protocol_GetNodeInfo_res(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_protocol_GetNodeInfo_res_descriptor = {
    UAVCAN_PROTOCOL_GETNODEINFO_RES_DT_SIG,
    UAVCAN_PROTOCOL_GETNODEINFO_RES_DT_ID,
    CanardTransferTypeResponse,
    sizeof(struct uavcan_protocol_GetNodeInfo_res_s),
    UAVCAN_PROTOCOL_GETNODEINFO_RES_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_protocol_GetNodeInfo_res(struct uavcan_protocol_GetNodeInfo_res_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_PROTOCOL_GETNODEINFO_RES_MAX_PACK_SIZE);
    _encode_uavcan_protocol_GetNodeInfo_res(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_protocol_GetNodeInfo_res(const CanardRxTransfer* transfer, struct uavcan_protocol_GetNodeInfo_res_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_protocol_GetNodeInfo_res(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_protocol_GetNodeInfo_res(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_GetNodeInfo_res_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    _encode_uavcan_protocol_NodeStatus(buffer, bit_ofs, &msg->status, false);
    _encode_uavcan_protocol_SoftwareVersion(buffer, bit_ofs, &msg->software_version, false);
    _encode_uavcan_protocol_HardwareVersion(buffer, bit_ofs, &msg->hardware_version, false);
    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 7, &msg->name_len);
        *bit_ofs += 7;
    }
    for (size_t i=0; i < msg->name_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 8, &msg->name[i]);
            *bit_ofs += 8;
    }
}

void _decode_uavcan_protocol_GetNodeInfo_res(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_GetNodeInfo_res_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    _decode_uavcan_protocol_NodeStatus(transfer, bit_ofs, &msg->status, false);
    _decode_uavcan_protocol_SoftwareVersion(transfer, bit_ofs, &msg->software_version, false);
    _decode_uavcan_protocol_HardwareVersion(transfer, bit_ofs, &msg->hardware_version, false);
    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 7, false, &msg->name_len);
        *bit_ofs += 7;
    }
    for (size_t i=0; i < msg->name_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->name[i]);
            *bit_ofs += 8;
    }
}
