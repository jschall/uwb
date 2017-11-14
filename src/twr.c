#include "twr.h"
#include "tdma.h"

static uint8_t curr_idx;
static uint8_t my_node_id;
static uint8_t my_slot_id;
static uint8_t num_sol;
static uint16_t trip_cnt;
static float calib_data[3][3];
static uint16_t sample_count[3][3];

//Contains list of data_sets for ranging
struct ds_twr_data_s range_pkt2send[MAX_NUM_DEVICES];


/*
    Common TWR methods
*/
void twr_init(uint8_t _my_node_id, uint8_t _my_slot_id)
{
    my_node_id = _my_node_id;
    my_slot_id = _my_slot_id;
    curr_idx = 0;
    num_sol = 0;
    trip_cnt = 0;

    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            calib_data[i][j] = 0.0f;
            sample_count[i][j] = 0;
        }
    }
    memset(range_pkt2send, 0, sizeof(range_pkt2send));
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

static void setup_next_trip(uint8_t receive_node_id, int64_t receive_tstamp, uint8_t next_trip_status)
{
    switch(next_trip_status) {
        case TRIP_INIT:
            //This pkt is meant to initiate trip initialisation by the opposite end as well
            range_pkt2send[receive_node_id].trip_id = 0;
            range_pkt2send[receive_node_id].trip_status = TRIP_INIT;
            memset(range_pkt2send[receive_node_id].transmit_tstamps, 0, 3*sizeof(int64_t));
            memset(range_pkt2send[receive_node_id].receive_tstamps, 0, 3*sizeof(int64_t));
            range_pkt2send[receive_node_id].tprop = 0.0f;
            break;
        case TRIP_START:
            //Start two way ranging trip
            range_pkt2send[receive_node_id].trip_id = 0;
            range_pkt2send[receive_node_id].trip_status = TRIP_START;
            memset(range_pkt2send[receive_node_id].transmit_tstamps, 0, 3*sizeof(int64_t));
            memset(range_pkt2send[receive_node_id].receive_tstamps, 0, 3*sizeof(int64_t));
            range_pkt2send[receive_node_id].tprop = 0.0f;
            break;
        case TRIP_REPLY:
            range_pkt2send[receive_node_id].trip_id++;
            range_pkt2send[receive_node_id].trip_status = TRIP_REPLY;
            range_pkt2send[receive_node_id].receive_tstamps[0] = receive_tstamp;
            break;
        case SS_TWR_SOL:
            range_pkt2send[receive_node_id].trip_id++;
            range_pkt2send[receive_node_id].trip_status = SS_TWR_SOL;
            range_pkt2send[receive_node_id].receive_tstamps[1] = receive_tstamp;
            //we have enough info to do single sided two way ranging, so do it
            do_ss_twr(&range_pkt2send[receive_node_id]);
            break;
        case DS_TWR_SOL:
            range_pkt2send[receive_node_id].trip_id++;
            range_pkt2send[receive_node_id].trip_status = DS_TWR_SOL;
            range_pkt2send[receive_node_id].receive_tstamps[2] = receive_tstamp;
            //we have enough info to do double sided two way ranging, so do it
            do_ds_twr(&range_pkt2send[receive_node_id]);
            break;
        case DS_TWR_SOL_RPT:
            range_pkt2send[receive_node_id].trip_id++;
            range_pkt2send[receive_node_id].trip_status = DS_TWR_SOL_RPT;
            //move the last 2 receive times to older positions
            range_pkt2send[receive_node_id].receive_tstamps[0] = range_pkt2send[receive_node_id].receive_tstamps[1];
            range_pkt2send[receive_node_id].receive_tstamps[1] = range_pkt2send[receive_node_id].receive_tstamps[2];
            range_pkt2send[receive_node_id].receive_tstamps[2] = receive_tstamp;
            //we have enough info to do double sided two way ranging, so do it
            do_ds_twr(&range_pkt2send[receive_node_id]);
            //also move older transmits up one pos for next range sol
            range_pkt2send[receive_node_id].transmit_tstamps[0] = range_pkt2send[receive_node_id].transmit_tstamps[1];
            range_pkt2send[receive_node_id].transmit_tstamps[1] = range_pkt2send[receive_node_id].transmit_tstamps[2];
            break;
        default:
            break;
    }
}

void setup_ranging_pkt_for_tx(uint8_t num_online, int64_t transmit_tstamp, struct message_spec_s *msg)
{
    for (uint8_t i = 0; i < num_online; i++) {
        switch(range_pkt2send[i].trip_status) {
            case TRIP_INIT:
                msg->ds_twr_data[i] = range_pkt2send[i];
                break;
            case TRIP_START:
                //Copy and note first transmit time
                range_pkt2send[i].transmit_tstamps[0] = transmit_tstamp;
                msg->ds_twr_data[i] = range_pkt2send[i];
                break;
            case TRIP_REPLY:
                //Copy and note second transmit time
                range_pkt2send[i].transmit_tstamps[1] = transmit_tstamp;
                msg->ds_twr_data[i] = range_pkt2send[i];
                break;
            case SS_TWR_SOL:
                //Copy and note third transmit time
                range_pkt2send[i].transmit_tstamps[2] = transmit_tstamp;
                msg->ds_twr_data[i] = range_pkt2send[i];
                break;
            case DS_TWR_SOL:
            case DS_TWR_SOL_RPT:
                //We have completed two way ranging publish the results
                range_pkt2send[i].transmit_tstamps[2] = transmit_tstamp;
                msg->ds_twr_data[i] = range_pkt2send[i];
                break;
            default:
                break;
        }
    }
}

/*
 Calibration Methods
*/
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

//ANCHOR update Method
void update_anchor(struct message_spec_s *msg, struct tx_spec_s *tx_spec, int64_t receive_tstamp)
{
    if (msg->tx_spec.type == ANCHOR && msg->tx_spec.ant_delay_cal) {
        if (range_pkt2send[msg->tx_spec.data_slot_id].trip_status == TRIP_INIT &&
            msg->ds_twr_data[tx_spec->data_slot_id].trip_status != TRIP_INIT &&
            msg->ds_twr_data[tx_spec->data_slot_id].trip_status != TRIP_START) {
            //Send packet to Initialise/Reset trip
            setup_next_trip(msg->tx_spec.data_slot_id, 0, TRIP_INIT);

        } else if (msg->ds_twr_data[tx_spec->data_slot_id].trip_status == DS_TWR_SOL_RPT) {
            if (range_pkt2send[msg->tx_spec.data_slot_id].trip_status == DS_TWR_SOL_RPT ||
                range_pkt2send[msg->tx_spec.data_slot_id].trip_status == DS_TWR_SOL) {
                // record the tprop in the pkt for calibration

                //we stay in this state unless we receive command to do otherwise
                range_pkt2send[msg->tx_spec.data_slot_id] = msg->ds_twr_data[tx_spec->data_slot_id];
                setup_next_trip(msg->tx_spec.data_slot_id, receive_tstamp, DS_TWR_SOL_RPT);
            }
        } else {
            range_pkt2send[msg->tx_spec.data_slot_id] = msg->ds_twr_data[tx_spec->data_slot_id];
            setup_next_trip(msg->tx_spec.data_slot_id, receive_tstamp, msg->ds_twr_data[tx_spec->data_slot_id].trip_status+1);    
        }
    }
}

//Tag update Method
void update_tag(struct message_spec_s *msg, struct tx_spec_s *tx_spec, int64_t receive_tstamp)
{

}
