#include "twr.h"
#include "tdma.h"

struct {
	struct {
       struct ds_twr_data_s dat;
       uint8_t target_node_id;
    }item[2*MAX_NUM_DEVICES];
	uint8_t head;
	uint8_t tail;
}buffer;

static uint8_t curr_idx;
static uint8_t my_node_id;
static uint8_t my_slot_id;
static struct range_sol_s range_sol[MAX_NUM_DEVICES];
static uint8_t num_sol;
static uint16_t trip_cnt;
static float calib_data[3][3];
static uint16_t sample_count[3][3];
void twr_init(uint8_t _my_node_id, uint8_t _my_slot_id)
{
    my_node_id = _my_node_id;
    my_slot_id = _my_slot_id;
    curr_idx = 0;
    num_sol = 0;
    trip_cnt = 0;
    memset(range_sol, 0, MAX_NUM_DEVICES*sizeof(struct range_sol_s));

    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            calib_data[i][j] = 0.0f;
            sample_count[i][j] = 0;
        }
    }
}

static void record_range_sol(struct ds_twr_data_s *pkt)
{
    for (uint8_t i = 0; i < num_sol; i++) {
        //Have we already recoded this node before?
        if (range_sol[i].deviceA == pkt->deviceA && range_sol[i].deviceB == pkt->deviceB) {
            range_sol[i].tprop = pkt->tprop;
            range_sol[i].timestamp = millis();
            return;
        }
    }
    if (num_sol < MAX_NUM_DEVICES) {
        range_sol[num_sol].deviceA = pkt->deviceA;
        range_sol[num_sol].deviceB = pkt->deviceB;
        range_sol[num_sol].tprop = pkt->tprop;
        range_sol[num_sol].timestamp = millis();
        num_sol++;
    }
}

static void do_ds_twr(struct ds_twr_data_s *pkt)
{
    double tround1 = TIME_TO_METERS*(double)dw1000_wrap_timestamp(pkt->receive_tstamps[1] - pkt->transmit_tstamps[0]);
    double treply1 = TIME_TO_METERS*(double)dw1000_wrap_timestamp(pkt->transmit_tstamps[1] - pkt->receive_tstamps[0]);
    double tround2 = TIME_TO_METERS*(double)dw1000_wrap_timestamp(pkt->receive_tstamps[2] - pkt->transmit_tstamps[1]);
    double treply2 = TIME_TO_METERS*(double)dw1000_wrap_timestamp(pkt->transmit_tstamps[2] - pkt->receive_tstamps[1]);
    pkt->tprop = (float)(((tround1*tround2) - (treply1*treply2)) / (tround1+tround2+treply1+treply2));
}

static void do_ss_twr(struct ds_twr_data_s *pkt)
{
    float tround =  TIME_TO_METERS*(float)(pkt->receive_tstamps[1] - pkt->transmit_tstamps[0]);
    float treply =  TIME_TO_METERS*(float)(pkt->transmit_tstamps[1] - pkt->receive_tstamps[0]);
    pkt->tprop = (tround - treply)/2;
}

static void handle_pkt(uint8_t receive_node_id, int64_t receive_tstamp, struct ds_twr_data_s *pkt)
{
    bool new_data = false;
    if(((buffer.tail + 1)%MAX_NUM_DEVICES) == buffer.head && pkt->trip_status != DS_TWR_SOL) {
        //buffer is full
        return;
    }
    switch(pkt->trip_status) {
        case TRIP_START:
            //Update the first receive time
            pkt->receive_tstamps[0] = receive_tstamp;
            //update the state
            pkt->trip_status = TRIP_REPLY;
            //Push to buffer
            memcpy(&buffer.item[buffer.tail].dat, pkt, sizeof(struct ds_twr_data_s));
            buffer.item[buffer.tail].target_node_id = receive_node_id;
            new_data = true;
            pkt->tprop = 0.0f;
            break;
        case TRIP_REPLY:
            pkt->receive_tstamps[1] = receive_tstamp;
            //We have completed the trip for single sided two way ranging
            do_ss_twr(pkt);
            //update the state
            pkt->trip_status = SS_TWR_SOL;
            memcpy(&buffer.item[buffer.tail].dat, pkt, sizeof(struct ds_twr_data_s));
            buffer.item[buffer.tail].target_node_id = receive_node_id;
            new_data = true;
            break;
        case SS_TWR_SOL:
            pkt->receive_tstamps[2] = receive_tstamp;
            //We have completed the trip for double sided two way ranging
            do_ds_twr(pkt);
            record_range_sol(pkt);
            pkt->trip_status = DS_TWR_SOL;
            memcpy(&buffer.item[buffer.tail].dat, pkt, sizeof(struct ds_twr_data_s));
            buffer.item[buffer.tail].target_node_id = receive_node_id;
            new_data = true;
            break;
        case DS_TWR_SOL:
            //It's done just record the data
            record_range_sol(pkt);
            break;
        default:
            break;

    }
    //update the tail if new data to be published
    if(new_data) {
        buffer.tail++;
        buffer.tail %= MAX_NUM_DEVICES;
    }
}

void setup_next_trip(uint8_t *slot_map, uint8_t num_online)
{
    static uint8_t slot_cnt = 0;
    if (num_online == 0) {
        return;
    }
    slot_cnt++;
    if (slot_cnt == 2*num_online + 1) {
        slot_cnt = 0;
    } else {
        return;
    }

    num_online++;
    if(((buffer.tail + 1)%MAX_NUM_DEVICES) == buffer.head) {
        return;
    }
    if(curr_idx == my_slot_id) {
        curr_idx++;
        curr_idx %= num_online;
    }

    for (uint8_t i = 0; i < num_online; i++) {
        if (slot_map[curr_idx] == 0) {
            //we haven't discovered this slot yet
            //so we increment and return
            curr_idx++;
            curr_idx %= num_online;
        } else {
            break;
        }
    }
    if (slot_map[curr_idx] == 0) {
        return;
    }
    //uint8_t i = buffer.head;
    /*while(i != buffer.tail) {
        if (buffer.item[i].target_node_id == slot_map[curr_idx + my_slot_id]) {
            //check if we have anything already setup to be sent for this id as START_TRIP
            //if so lets skip this run and wait until the TWR is complete for that
            return;
        }
        i++;
        i %= MAX_NUM_DEVICES;
    }*/

    buffer.item[buffer.tail].target_node_id = slot_map[curr_idx];
    buffer.item[buffer.tail].dat.trip_id = trip_cnt++;
    buffer.item[buffer.tail].dat.deviceA = my_node_id;
    buffer.item[buffer.tail].dat.deviceB = slot_map[curr_idx];
    buffer.item[buffer.tail].dat.trip_status = TRIP_START;
    memset(buffer.item[buffer.tail].dat.transmit_tstamps, 0, 3*sizeof(uint64_t));
    memset(buffer.item[buffer.tail].dat.receive_tstamps, 0, 3*sizeof(uint64_t));
    curr_idx++;
    curr_idx %= num_online;
    buffer.tail++;
    buffer.tail %= MAX_NUM_DEVICES;
}

void parse_ranging_pkt(struct ds_twr_data_s *pkt, uint8_t recieve_node_id, 
    int64_t receive_tstamp)
{
    //Check if we are interested in the packet
    handle_pkt(recieve_node_id, receive_tstamp, pkt);
}

void send_ranging_pkt(struct ds_twr_data_s *pkt, uint16_t *target_node_id, 
    int64_t transmit_tstamp)
{
    //No data available
    memset(pkt, 0, sizeof(struct ds_twr_data_s));
    *target_node_id = 0;
    if (buffer.head == buffer.tail) {
        return;
    }
    switch(buffer.item[buffer.head].dat.trip_status) {
        case TRIP_START:
            //Copy and note first transmit time
            memcpy(pkt, &buffer.item[buffer.head].dat, sizeof(struct ds_twr_data_s));
            pkt->transmit_tstamps[0] = transmit_tstamp;
            *target_node_id = buffer.item[buffer.head].target_node_id;
            break;
        case TRIP_REPLY:
            //Copy and note second transmit time
            memcpy(pkt, &buffer.item[buffer.head].dat, sizeof(struct ds_twr_data_s));
            pkt->transmit_tstamps[1] = transmit_tstamp;
            *target_node_id = buffer.item[buffer.head].target_node_id;
            break;
        case SS_TWR_SOL:
            //Copy and note third transmit time
            memcpy(pkt, &buffer.item[buffer.head].dat, sizeof(struct ds_twr_data_s));
            pkt->transmit_tstamps[2] = transmit_tstamp;
            *target_node_id = buffer.item[buffer.head].target_node_id;
            break;
        case DS_TWR_SOL:
            //We have completed two way ranging publish the results
            memcpy(pkt, &buffer.item[buffer.head].dat, sizeof(struct ds_twr_data_s));
            *target_node_id = buffer.item[buffer.head].target_node_id;
            break;
        default:
            break;
    }
    buffer.head++;
    buffer.head %= MAX_NUM_DEVICES;
}


bool push_calib_data(float range, uint8_t id1, uint8_t id2)
{
    if (sample_count[id1][id2] != MAX_CAL_SAMPLES) {
        if(sample_count[id1][id2] > 0 && fabsf(range - calib_data[id1][id2]) > 10.0f) {
            return false;
        }
        calib_data[id1][id2] = calib_data[id1][id2]*sample_count[id1][id2] + range;
        sample_count[id1][id2]++;
        calib_data[id1][id2] /= sample_count[id1][id2];
    }
    if (sample_count[id1][id2] == MAX_CAL_SAMPLES) {
        //check if we have achieved maximum sample count for all device pairs
        for (uint8_t i = 0; i < 3; i++) {
            for (uint8_t j = 0; j < 3; j++) {
                if (i == j) {
                    continue;
                }
                if (sample_count[i][j] != 1000) {
                    return false;
                }
            }
        }
    } else {
        return false;
    }
    return true;
}
//Returns Aggregate Antenna Delay for given device id
float get_result(uint8_t id)
{
    float delta[3][3];
    float result;
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            if (i == j) {
                continue;
            }
            delta[i][j] = calib_data[i][j] - TRUE_RANGE;
        }
    }
    result = (delta[id][(id+1)%3] + delta[(id+1)%3][id])/2;
    result += (delta[id][(id+2)%3] + delta[(id+2)%3][id])/2;
    result -= (delta[(id+1)%3][(id+2)%3] + delta[(id+2)%3][(id+1)%3])/2;
    return result;
}

uint16_t get_sample_count(uint8_t id1, uint8_t id2)
{
    return sample_count[id1][id2];
}


float get_sample_dat(uint8_t id1, uint8_t id2)
{
    return calib_data[id1][id2];
}
