/*
    Rules for Two Way Ranging:

    Types of Devices:
    1. Tag: The one trying to know its position.
    2. Anchor: The one that is static and only wants to be calibrated to
       act as a precise reference
    3. Monitor: The one used for debugging purposes and could maintain 
       position and data of online devices

    TWR Trip Status:
    1. TRIP_START
    2. TRIP_REPLY
    3. SS_TWR_SOL/TRIP_FINAL
    4  DS_TWR_SOL

    TRIP1 is always initiated either by an Anchor with lower Data Slot ID 
    in TDMA stream or by a Tag. Trip Identification is done by incrementing
    id field in conjunction with trip_node_id which records
    the TWR instigator's node id.

    States are always transitioned during receive
    When no TWR trip is engaged, each node will scroll through ranging record 
    and publish it on the network.
*/
#pragma once
#include <modules/timing/timing.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <modules/driver_dw1000/dw1000.h>

#define DW1000_CAL_CHAN_NUM DW1000_CHANNEL_7
#define TRUE_RANGE  0.3f
#define DW1000_CAL_PRF DW1000_PRF_64MHZ
#define MAX_CAL_SAMPLES 1000
struct message_spec_s;
struct tx_spec_s;

enum twr_statuses {
    TWR_RESET = 0,
    TWR_INIT,
    TWR_LOCKED
};

struct __attribute__((packed)) ds_twr_data_s {
    uint8_t trip_id;        //ID of the trip we are making
    uint8_t twr_status;
    int64_t treply2;
    int64_t tround1;
    float tprop;    //this will be in meters
};

struct range_sol_s {
    uint32_t timestamp; //CPU Timestamp
    uint8_t deviceA;
    uint8_t deviceB;
    float tprop;    //this will be in meters
};

void twr_init(void);
void update_twr_tx(struct message_spec_s *msg, int64_t transmit_tstamp);
void update_twr_rx(struct message_spec_s *msg, struct tx_spec_s *tx_spec, int64_t receive_tstamp);

//Calibration methods
void update_twr_cal_tx(struct message_spec_s *msg, int64_t transmit_tstamp);
void update_twr_cal_rx(struct message_spec_s *msg, struct tx_spec_s *tx_spec, int64_t receive_tstamp);
float get_result(uint8_t id);
float get_sample_dat(uint8_t id1, uint8_t id2);
uint16_t get_sample_count(uint8_t id1, uint8_t id2);

#include "tdma.h"