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
#include <common/timing.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define TIME_TO_METERS 0.0046917639786159f
#define METERS_TO_TIME 213.13945f
#define DW1000_CAL_CHAN_NUM DW1000_CHANNEL_7
#define TRUE_RANGE  0.5f
#define DW1000_CAL_PRF DW1000_PRF_64MHZ
#define MAX_CAL_SAMPLES 1000
enum trip_statuses {
    TRIP_START,
    TRIP_REPLY,
    SS_TWR_SOL,
    DS_TWR_SOL
};

struct ds_twr_data_s {
    uint8_t trip_id;        //Id of the trip we are making
    uint8_t deviceA;        //ID of trip instigator
    uint8_t deviceB;
    uint8_t trip_status;
    uint64_t transmit_tstamps[3];
    uint64_t receive_tstamps[3];
    float tprop;    //this will be in meters
};

struct range_sol_s {
    uint32_t timestamp; //CPU Timestamp
    uint8_t deviceA;
    uint8_t deviceB;
    float tprop;    //this will be in meters
};

void twr_init(uint8_t _my_node_id, uint8_t _my_slot_id);
void parse_ranging_pkt(struct ds_twr_data_s *pkt, uint8_t receive_node_id, 
    uint64_t receive_tstamp);
void send_ranging_pkt(struct ds_twr_data_s *pkt, uint8_t *target_node_id, 
    uint64_t transmit_tstamp);
void setup_next_trip(uint8_t *slot_map, uint8_t num_online);
bool push_calib_data(float range, uint8_t id1, uint8_t id2);
float get_result(uint8_t id);
float get_sample_dat(uint8_t id1, uint8_t id2);
uint16_t get_sample_count(uint8_t id1, uint8_t id2);
