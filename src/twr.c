#include "twr.h"
#include "tdma.h"


static uint8_t trip_id[MAX_NUM_DEVICES];
static uint8_t twr_status[MAX_NUM_DEVICES];
static float calib_data[3][3];
static uint16_t sample_count[3][3];

//Contains list of data_sets for ranging
static int64_t rx_tstamp_list[MAX_NUM_DEVICES][2];
static int64_t tx_tstamp_list[MAX_NUM_DEVICES][2];
static float range[MAX_NUM_DEVICES];
/*
    Common TWR methods
*/
void twr_init()
{

    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            calib_data[i][j] = 0.0f;
            sample_count[i][j] = 0;
        }
    }
    memset(rx_tstamp_list, 0, sizeof(rx_tstamp_list));
    memset(tx_tstamp_list, 0, sizeof(tx_tstamp_list));
    memset(trip_id, 0, sizeof(trip_id));
    memset(twr_status, 0, sizeof(twr_status));
}

static float do_ds_twr(uint8_t trip_id, int64_t tround1, int64_t treply2, int64_t receive_tstamp, uint8_t module_num)
{
    uint64_t tround2, treply1;
    switch(trip_id) {
        case 0:
            rx_tstamp_list[module_num][0] = receive_tstamp;
            tround2 = rx_tstamp_list[module_num][0] - tx_tstamp_list[module_num][1];
            treply1 = tx_tstamp_list[module_num][1] - rx_tstamp_list[module_num][1];
            break;
        case 1:
            rx_tstamp_list[module_num][0] = receive_tstamp;
            tround2 = rx_tstamp_list[module_num][0] - tx_tstamp_list[module_num][0];
            treply1 = tx_tstamp_list[module_num][0] - rx_tstamp_list[module_num][1];
            break;
        case 2:
            rx_tstamp_list[module_num][1] = receive_tstamp;
            tround2 = rx_tstamp_list[module_num][1] - tx_tstamp_list[module_num][0];
            treply1 = tx_tstamp_list[module_num][0] - rx_tstamp_list[module_num][0];
            break;
        case 3:
            rx_tstamp_list[module_num][1] = receive_tstamp;
            tround2 = rx_tstamp_list[module_num][1] - tx_tstamp_list[module_num][1];
            treply1 = tx_tstamp_list[module_num][1] - rx_tstamp_list[module_num][0];
            break;
        default:
            return 0.0f;
    }
    tround2 = dw1000_wrap_timestamp(tround2);
    treply1 = dw1000_wrap_timestamp(treply1);
    if (twr_status[module_num] == TWR_LOCKED) {
        int64_t tsum = tround1+tround2+treply1+treply2;
        if(tsum == 0) {
            return 0.0f;
        }
        return DW1000_TIME_TO_METERS * (((float)((tround1*tround2) - (treply1*treply2))) / ((float)tsum));
    } else {
        return 0.0f;
    }
}

static void calc_for_transmit(uint8_t trip_id, int64_t *tround1, int64_t *treply2, int64_t transmit_tstamp, uint8_t module_num)
{
    switch(trip_id) {
        case 0:
            tx_tstamp_list[module_num][0] = transmit_tstamp;
            *tround1 = rx_tstamp_list[module_num][1] - tx_tstamp_list[module_num][1];
            *treply2 = tx_tstamp_list[module_num][0] - rx_tstamp_list[module_num][1];
            break;
        case 1:
            tx_tstamp_list[module_num][0] = transmit_tstamp;
            *tround1 = rx_tstamp_list[module_num][0] - tx_tstamp_list[module_num][1];
            *treply2 = tx_tstamp_list[module_num][0] - rx_tstamp_list[module_num][0];
            break;
        case 2:
            tx_tstamp_list[module_num][1] = transmit_tstamp;
            *tround1 = rx_tstamp_list[module_num][0] - tx_tstamp_list[module_num][0];
            *treply2 = tx_tstamp_list[module_num][1] - rx_tstamp_list[module_num][0];
            break;
        case 3:
            tx_tstamp_list[module_num][1] = transmit_tstamp;
            *tround1 = rx_tstamp_list[module_num][1] - tx_tstamp_list[module_num][0];
            *treply2 = tx_tstamp_list[module_num][1] - rx_tstamp_list[module_num][1];
            break;
        default:
            return;
    }
    *tround1 = dw1000_wrap_timestamp(*tround1);
    *treply2 = dw1000_wrap_timestamp(*treply2);
}

//update twr methods
void update_twr_rx(struct message_spec_s *msg, struct tx_spec_s *tx_spec, int64_t receive_tstamp)
{
    switch(msg->ds_twr_data[tx_spec->data_slot_id].twr_status) {
        case TWR_RESET:
            twr_status[msg->tx_spec.data_slot_id] = TWR_INIT;
            trip_id[msg->tx_spec.data_slot_id] = 0;
            return;
        case TWR_INIT:
            if (twr_status[msg->tx_spec.data_slot_id] == TWR_RESET && 
                trip_id[msg->tx_spec.data_slot_id] == 0) {
                twr_status[msg->tx_spec.data_slot_id] = TWR_INIT;
            }
            break;
        case TWR_LOCKED:
            if (twr_status[msg->tx_spec.data_slot_id] == TWR_INIT) {
                twr_status[msg->tx_spec.data_slot_id] = TWR_LOCKED;
            }
            break;
        default:
            return;
    };
    trip_id[msg->tx_spec.data_slot_id] = msg->ds_twr_data[tx_spec->data_slot_id].trip_id;
    range[msg->tx_spec.data_slot_id] = do_ds_twr(trip_id[msg->tx_spec.data_slot_id], 
                                                 msg->ds_twr_data[tx_spec->data_slot_id].tround1, 
                                                 msg->ds_twr_data[tx_spec->data_slot_id].treply2,
                                                 receive_tstamp, msg->tx_spec.data_slot_id);
    //increment trip id
    trip_id[msg->tx_spec.data_slot_id]++;
    trip_id[msg->tx_spec.data_slot_id] %= 4;

    if (trip_id[msg->tx_spec.data_slot_id] == 3 && twr_status[msg->tx_spec.data_slot_id] == TWR_INIT) {
        twr_status[msg->tx_spec.data_slot_id] = TWR_LOCKED;
    }
}

void update_twr_tx(struct message_spec_s *msg, int64_t transmit_tstamp)
{
    for (uint8_t i = 0; i < MAX_NUM_DEVICES; i++) {
        calc_for_transmit(trip_id[i], &msg->ds_twr_data[i].tround1, &msg->ds_twr_data[i].treply2, transmit_tstamp, i);
        msg->ds_twr_data[i].trip_id = trip_id[i];
        msg->ds_twr_data[i].tprop = range[i];
        msg->ds_twr_data[i].twr_status = twr_status[i];
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
