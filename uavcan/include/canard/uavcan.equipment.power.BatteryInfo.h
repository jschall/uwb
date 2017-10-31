#pragma once
#include <canard.h>
#include <stdbool.h>
#include <stdint.h>
#include <common/uavcan.h>


#define UAVCAN_EQUIPMENT_POWER_BATTERYINFO_MAX_PACK_SIZE 55
#define UAVCAN_EQUIPMENT_POWER_BATTERYINFO_DT_SIG 0x249C26548A711966
#define UAVCAN_EQUIPMENT_POWER_BATTERYINFO_DT_ID 1092

#define UAVCAN_EQUIPMENT_POWER_BATTERYINFO_STATUS_FLAG_IN_USE 1
#define UAVCAN_EQUIPMENT_POWER_BATTERYINFO_STATUS_FLAG_CHARGING 2
#define UAVCAN_EQUIPMENT_POWER_BATTERYINFO_STATUS_FLAG_CHARGED 4
#define UAVCAN_EQUIPMENT_POWER_BATTERYINFO_STATUS_FLAG_TEMP_HOT 8
#define UAVCAN_EQUIPMENT_POWER_BATTERYINFO_STATUS_FLAG_TEMP_COLD 16
#define UAVCAN_EQUIPMENT_POWER_BATTERYINFO_STATUS_FLAG_OVERLOAD 32
#define UAVCAN_EQUIPMENT_POWER_BATTERYINFO_STATUS_FLAG_BAD_BATTERY 64
#define UAVCAN_EQUIPMENT_POWER_BATTERYINFO_STATUS_FLAG_NEED_SERVICE 128
#define UAVCAN_EQUIPMENT_POWER_BATTERYINFO_STATUS_FLAG_BMS_ERROR 256
#define UAVCAN_EQUIPMENT_POWER_BATTERYINFO_STATUS_FLAG_RESERVED_A 512
#define UAVCAN_EQUIPMENT_POWER_BATTERYINFO_STATUS_FLAG_RESERVED_B 1024
#define UAVCAN_EQUIPMENT_POWER_BATTERYINFO_STATE_OF_HEALTH_UNKNOWN 127

struct uavcan_equipment_power_BatteryInfo_s {
    float temperature;
    float voltage;
    float current;
    float average_power_10sec;
    float remaining_capacity_wh;
    float full_charge_capacity_wh;
    float hours_to_full_charge;
    uint16_t status_flags;
    uint8_t state_of_health_pct;
    uint8_t state_of_charge_pct;
    uint8_t state_of_charge_pct_stdev;
    uint8_t battery_id;
    uint32_t model_instance_id;
    struct {uint8_t model_name_len; uint8_t model_name[31];};
};

extern const struct uavcan_message_descriptor_s uavcan_equipment_power_BatteryInfo_descriptor;

uint32_t encode_uavcan_equipment_power_BatteryInfo(struct uavcan_equipment_power_BatteryInfo_s* msg, uint8_t* buffer);
uint32_t decode_uavcan_equipment_power_BatteryInfo(const CanardRxTransfer* transfer, struct uavcan_equipment_power_BatteryInfo_s* msg);
void _encode_uavcan_equipment_power_BatteryInfo(uint8_t* buffer, uint32_t* bit_ofs, struct uavcan_equipment_power_BatteryInfo_s* msg, bool tao);
void _decode_uavcan_equipment_power_BatteryInfo(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct uavcan_equipment_power_BatteryInfo_s* msg, bool tao);
