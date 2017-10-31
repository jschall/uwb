#include <canard/uavcan.protocol.param.GetSet_Request.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_protocol_param_GetSet_req(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_protocol_param_GetSet_req(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_protocol_param_GetSet_req_descriptor = {
    UAVCAN_PROTOCOL_PARAM_GETSET_REQ_DT_SIG,
    UAVCAN_PROTOCOL_PARAM_GETSET_REQ_DT_ID,
    CanardTransferTypeRequest,
    sizeof(struct uavcan_protocol_param_GetSet_req_s),
    UAVCAN_PROTOCOL_PARAM_GETSET_REQ_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_protocol_param_GetSet_req(struct uavcan_protocol_param_GetSet_req_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_PROTOCOL_PARAM_GETSET_REQ_MAX_PACK_SIZE);
    _encode_uavcan_protocol_param_GetSet_req(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_protocol_param_GetSet_req(const CanardRxTransfer* transfer, struct uavcan_protocol_param_GetSet_req_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_protocol_param_GetSet_req(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_protocol_param_GetSet_req(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_param_GetSet_req_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 13, &msg->index);
    *bit_ofs += 13;
    _encode_uavcan_protocol_param_Value(buffer, bit_ofs, &msg->value, false);
    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 7, &msg->name_len);
        *bit_ofs += 7;
    }
    for (size_t i=0; i < msg->name_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 8, &msg->name[i]);
            *bit_ofs += 8;
    }
}

void _decode_uavcan_protocol_param_GetSet_req(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_param_GetSet_req_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 13, false, &msg->index);
    *bit_ofs += 13;
    _decode_uavcan_protocol_param_Value(transfer, bit_ofs, &msg->value, false);
    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 7, false, &msg->name_len);
        *bit_ofs += 7;
    }
    for (size_t i=0; i < msg->name_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->name[i]);
            *bit_ofs += 8;
    }
}
