#include "ch.h"
#include "hal.h"
#include <common/timing.h>
#include <common/helpers.h>
#include "dw1000.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <common/param.h>
#include <common/uavcan.h>
#include "uavcan_node.h"

//TODO: Convert these to parameter
#define MAX_DATA_SIZE 127
#define MAX_HEADER_SIZE 93
#define TX_RATE 6800000

#define SLOT_SIZE 1000ULL	//1ms
#define TOTAL_AVAILABLE_SLOTS 127


#define TDMA_SUPERVISOR_ID 0x4A
#define SYSTIME_FREQ 125000000ULL
#define SLOT_SIZE_TICKS (SYSTIME_FREQ/SLOT_SIZE)


enum tdma_slots {
	START_SLOT,
	DATA_SLOT,
	REQUEST_SLOT
};

enum tx_types {
	MOBILE_ANCHOR,
	MOBILE_TAG,
	STATIC_TAG
};

struct ds_twr_data {
	uint8_t target_node_id;
	uint32_t tround1;
	uint32_t treply1;
	uint32_t tround2;
	uint32_t treply2;
};

struct tdma_spec_s {
	uint32_t slot_size;
	uint32_t num_tx_online;
	uint8_t res_data_slot;
	uint8_t req_node_id;
	uint8_t cnt;
};

struct tx_spec_s {
	uint8_t type;
	uint8_t node_id;
	uint8_t pkt_cnt;
	uint8_t data_slot_id;
};

struct message_spec_s {
	struct tx_spec_s tx_spec;
	uint32_t target_node_id;
	uint64_t transmit_time;
};


void tdma_init(uint8_t unique_id);
void tdma_supervisor_run();
void tdma_subordinate_run();