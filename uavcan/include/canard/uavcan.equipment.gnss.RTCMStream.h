#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_GNSS_RTCMSTREAM_MAX_PACK_SIZE 130
#define UAVCAN_EQUIPMENT_GNSS_RTCMSTREAM_DT_SIG 0x1F56030ECB171501
#define UAVCAN_EQUIPMENT_GNSS_RTCMSTREAM_DT_ID 1062

#define UAVCAN_EQUIPMENT_GNSS_RTCMSTREAM_PROTOCOL_ID_UNKNOWN 0
#define UAVCAN_EQUIPMENT_GNSS_RTCMSTREAM_PROTOCOL_ID_RTCM2 2
#define UAVCAN_EQUIPMENT_GNSS_RTCMSTREAM_PROTOCOL_ID_RTCM3 3

struct uavcan_equipment_gnss_RTCMStream_s {
    uint8_t protocol_id;
    struct {uint8_t data_len; uint8_t data[128];};
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_gnss_RTCMStream_descriptor;

uint32_t encode_uavcan_equipment_gnss_RTCMStream(struct uavcan_equipment_gnss_RTCMStream_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_gnss_RTCMStream(const CanardRxTransfer* transfer, struct uavcan_equipment_gnss_RTCMStream_s* msg);
void _encode_uavcan_equipment_gnss_RTCMStream(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_gnss_RTCMStream_s* msg, bool tao);
void _decode_uavcan_equipment_gnss_RTCMStream(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_gnss_RTCMStream_s* msg, bool tao);
