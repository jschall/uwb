#pragma once
#include "ch.h"
#include "hal.h"
#include <modules/timing/timing.h>
#include <common/helpers.h>
#include <modules/dw1000/dw1000.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <modules/param/param.h>
#include <modules/uavcan/uavcan.h>
#include "twr.h"
#include <modules/uavcan_debug/uavcan_debug.h>
//TODO: Convert these to parameter
#define MAX_HEADER_SIZE 93
#define TX_RATE 6800000
#define ARB_TIME ((SLOT_SIZE/3) + (SLOT_SIZE/10))

#define SLOT_SIZE 2500ULL    //2.5ms
#define TOTAL_AVAILABLE_SLOTS 10
#define MAX_NUM_DEVICES TOTAL_AVAILABLE_SLOTS

//TODO: Implement Supervisor switch in case of timeout
//TODO: Implement Delayed Receive for more failsafe receive


#define UWB_SYS_TICKS 63897.6f
#define ARB_TIME_SYS_TICKS ((uint64_t)(ARB_TIME*UWB_SYS_TICKS))
#define DW1000_SID2ST(x) ((uint64_t)(((x)*(UWB_SYS_TICKS*SLOT_SIZE))))


enum tdma_slots {
    START_SLOT,
    DATA_SLOT,
    REQUEST_SLOT
};

enum tx_types {
    ANCHOR = 0,
    TAG,
};

enum tdma_types {
    TDMA_SUPERVISOR = 0,
    TDMA_SUBORDINATE,
    TDMA_SNIFFER
};

struct tdma_spec_s {
    uint32_t slot_size;
    uint32_t num_tx_online;
    uint8_t res_data_slot;
    uint8_t req_node_id;
    uint32_t cnt;
};

struct tx_spec_s {
    uint8_t type;
    uint8_t node_id;
    uint8_t ant_delay_cal;
    uint32_t ant_delay;
    uint32_t pkt_cnt;
    uint8_t data_slot_id;
};

struct message_spec_s {
    struct tdma_spec_s tdma_spec;
    struct tx_spec_s tx_spec;
    struct ds_twr_data_s ds_twr_data;
    uint16_t target_node_id;
};


void tdma_init(uint8_t unit_type, struct tx_spec_s tx_spec);
void tdma_supervisor_run(void);
void tdma_subordinate_run(void);
void tdma_sniffer_run(void);
