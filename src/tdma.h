#pragma once
#include "ch.h"
#include "hal.h"
#include <modules/timing/timing.h>
#include <common/helpers.h>
#include <modules/driver_dw1000/dw1000.h>
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

#define SLOT_SIZE 4000ULL    //8ms
#define TOTAL_AVAILABLE_SLOTS 10
#define MAX_NUM_DEVICES TOTAL_AVAILABLE_SLOTS
#define INITIAL_BACKOFF_MASK 0x1
//TODO: Implement Supervisor switch in case of timeout
//TODO: Implement Delayed Receive for more failsafe receive


#define UWB_SYS_TICKS 63897.6f
#define DEFER_TIME ((SLOT_SIZE/200) + (SLOT_SIZE/500))
#define ARB_TIME SLOT_SIZE/2
#define ARB_TIME_SYS_TICKS ((uint64_t)(ARB_TIME*UWB_SYS_TICKS))
#define DW1000_SID2ST(x) ((uint64_t)(((x)*(UWB_SYS_TICKS*SLOT_SIZE))))

enum {
    RTS_MAGIC = 0xAF60,
    CTS_MAGIC,
    DS_MAGIC,
    DACK_MAGIC
};

enum tdma_slots {
    START_SLOT,
    DATA_SLOT,
    REQUEST_SLOT
};

enum {
    SEND_NONE,
    SEND_RTS,
    SEND_CTS,
    SEND_DS,
    SEND_DATA,
    SEND_DACK
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

struct __attribute__((packed)) tdma_spec_s {
    uint8_t num_slots;
    uint8_t tags_online;
    uint8_t anchors_online;
    uint8_t target_body_id;
    uint8_t res_data_slot;
    uint8_t req_node_id;
    uint8_t cnt;
    uint16_t slot_size;
};

struct __attribute__((packed)) tx_spec_s {
    uint8_t type;
    uint8_t node_id;
    uint8_t body_id;
    uint8_t data_slot_id;
    uint8_t ant_delay_cal;
    uint32_t ant_delay;
    uint32_t pkt_cnt;
};

#define MSG_HEADER_SIZE (sizeof(struct tdma_spec_s)+sizeof(struct tx_spec_s))
#define MSG_PAYLOAD_SIZE(x) (x*sizeof(struct ds_twr_data_s))
struct __attribute__((packed)) message_spec_s {
    struct tdma_spec_s tdma_spec;
    struct tx_spec_s tx_spec;
    struct ds_twr_data_s ds_twr_data[MAX_NUM_DEVICES];
};

struct __attribute__((packed)) body_comm_pkt {
    uint16_t magic;
    uint8_t body_id;
    uint8_t target_body_id;
};

void tdma_supervisor_init(struct tx_spec_s _tx_spec, uint8_t target_body_id, struct worker_thread_s* worker_thread, struct worker_thread_s* listener_thread);
void tdma_subordinate_init(struct tx_spec_s _tx_spec, uint32_t ant_delay);
void tdma_subordinate_run(void);
void tdma_sniffer_run(void);
bool is_medium_access_msg(struct message_spec_s *_msg);
