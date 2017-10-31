#include <canard/uavcan.protocol.AccessCommandShell_Response.h>
#include <string.h>

static uint32_t encode_func(void* buffer, void* msg) {
    return encode_uavcan_protocol_AccessCommandShell_res(buffer, msg);
}

static uint32_t decode_func(CanardRxTransfer* transfer, void* msg) {
    return decode_uavcan_protocol_AccessCommandShell_res(transfer, msg);
}

const struct uavcan_message_descriptor_s uavcan_protocol_AccessCommandShell_res_descriptor = {
    UAVCAN_PROTOCOL_ACCESSCOMMANDSHELL_RES_DT_SIG,
    UAVCAN_PROTOCOL_ACCESSCOMMANDSHELL_RES_DT_ID,
    CanardTransferTypeResponse,
    sizeof(struct uavcan_protocol_AccessCommandShell_res_s),
    UAVCAN_PROTOCOL_ACCESSCOMMANDSHELL_RES_MAX_PACK_SIZE,
    encode_func,
    decode_func
};

uint32_t encode_uavcan_protocol_AccessCommandShell_res(struct uavcan_protocol_AccessCommandShell_res_s* msg, uint8_t* buffer) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, UAVCAN_PROTOCOL_ACCESSCOMMANDSHELL_RES_MAX_PACK_SIZE);
    _encode_uavcan_protocol_AccessCommandShell_res(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_uavcan_protocol_AccessCommandShell_res(const CanardRxTransfer* transfer, struct uavcan_protocol_AccessCommandShell_res_s* msg) {
    uint32_t bit_ofs = 0;
    _decode_uavcan_protocol_AccessCommandShell_res(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_uavcan_protocol_AccessCommandShell_res(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_protocol_AccessCommandShell_res_s* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 32, &msg->last_exit_status);
    *bit_ofs += 32;
    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->flags);
    *bit_ofs += 8;
    if (!tao) {
        canardEncodeScalar(buffer, *bit_ofs, 9, &msg->output_len);
        *bit_ofs += 9;
    }
    for (size_t i=0; i < msg->output_len; i++) {
            canardEncodeScalar(buffer, *bit_ofs, 8, &msg->output[i]);
            *bit_ofs += 8;
    }
}

void _decode_uavcan_protocol_AccessCommandShell_res(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_protocol_AccessCommandShell_res_s* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardDecodeScalar(transfer, *bit_ofs, 32, true, &msg->last_exit_status);
    *bit_ofs += 32;
    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->flags);
    *bit_ofs += 8;
    if (!tao) {
        canardDecodeScalar(transfer, *bit_ofs, 9, false, &msg->output_len);
        *bit_ofs += 9;
    }
    for (size_t i=0; i < msg->output_len; i++) {
            canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->output[i]);
            *bit_ofs += 8;
    }
}
