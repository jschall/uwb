#include "twr.h"
#include "tdma.h"
#define SQR(x) ((x)*(x))

static uint8_t trip_id[MAX_NUM_DEVICES];
static uint8_t twr_status[MAX_NUM_DEVICES];
static int64_t calib_data[5][5];
static uint16_t sample_count[5][5];

//Contains list of data_sets for ranging
static int64_t rx_tstamp_list[MAX_NUM_DEVICES][2];
static int64_t tx_tstamp_list[MAX_NUM_DEVICES][2];
static int32_t range[MAX_NUM_DEVICES];
static float body_pos[MAX_NUM_DEVICES][3];
static float ant_delay;
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

static int64_t do_ds_twr(uint8_t _trip_id, int64_t tround1, int64_t treply2, int64_t receive_tstamp, uint8_t module_num)
{
    int64_t tround2, treply1;
    switch(_trip_id) {
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
        if (tround2 > (int64_t)(4*MAX_NUM_DEVICES*SLOT_SIZE*UWB_SYS_TICKS) || treply1 > (int64_t)(4*MAX_NUM_DEVICES*SLOT_SIZE*UWB_SYS_TICKS)) {
            twr_status[module_num] = TWR_RESET;
            trip_id[module_num] = 0;
        }
        int64_t tsum = tround1+tround2+treply1+treply2;
        if(tsum == 0) {
            return 0.0f;
        }
        int64_t res = ((tround1*tround2) - (treply1*treply2)) / tsum;
        if (res < 0) {
            uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "CAL", "%ld %ld %ld %ld %ld", (int32_t)res, (int32_t)tround1, (int32_t)tround2, (int32_t)treply1, (int32_t)treply2);
        }
        return res;
    } else {
        return 0.0f;
    }
}

static void calc_for_transmit(uint8_t _trip_id, int64_t *tround1, int64_t *treply2, int64_t transmit_tstamp, uint8_t module_num)
{
    switch(_trip_id) {
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
    if (twr_status[module_num] == TWR_LOCKED) {
        /*if (tround1 > (int64_t)(10*MAX_NUM_DEVICES*SLOT_SIZE*UWB_SYS_TICKS) || treply2 > (int64_t)(10*MAX_NUM_DEVICES*SLOT_SIZE*UWB_SYS_TICKS)) {
            twr_status[module_num] = TWR_RESET;
            trip_id[module_num] = 0;
        }*/
    }
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
            if (twr_status[msg->tx_spec.data_slot_id] == TWR_RESET) {
                twr_status[msg->tx_spec.data_slot_id] = TWR_INIT;
            }
            break;
        case TWR_LOCKED:
            if (twr_status[msg->tx_spec.data_slot_id] == TWR_RESET) {
                //we have commanded reset do nothing until init
                return;
            }
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

//Returns Aggregate Antenna Delay for given device id set

static float get_true_range(uint8_t id1, uint8_t id2)
{
    return sqrtf(SQR(body_pos[id1][0] - body_pos[id2][0]) +
                 SQR(body_pos[id1][1] - body_pos[id2][1]) +
                 SQR(body_pos[id1][2] - body_pos[id2][2]));
}

static float do_single_cal(uint8_t id0, uint8_t id1, uint8_t id2)
{
    float delta[3][3];
    float result;
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            if (i == j) {
                continue;
            }
            delta[i][j] = calib_data[i][j] - get_true_range(i, j);
        }
    }
    result = (delta[id0][id1] + delta[id1][id0])/2;
    result += (delta[id0][id2] + delta[id2][id0])/2;
    result -= (delta[id1][id2] + delta[id2][id1])/2;
    return result;
}


static void do_cal(uint8_t num_devices, uint8_t node_id)
{
    uint8_t num_runs = 0;
    for (uint8_t i = 1; i < num_devices; i++) {
        for (uint8_t j = i+1; j < num_devices; j++) {
            ant_delay += do_single_cal(node_id, (node_id+i)%num_devices, (node_id+j)%num_devices);
            num_runs++;
        }
    }
    ant_delay /= num_runs;
    ant_delay /= DW1000_TIME_TO_METERS;
    ant_delay /= 2.0f;
}

static uint16_t get_sample_count(uint8_t id1, uint8_t id2)
{
    return sample_count[id1][id2];
}


static int64_t get_sample_dat(uint8_t id1, uint8_t id2)
{
    return calib_data[id1][id2];
}

float get_ant_delay()
{
    return ant_delay;
}

bool is_data_collection_complete(uint8_t num_devices)
{
    if (num_devices < 3) {
        return false;
    }
    for (uint8_t i = 0; i < num_devices; i++) {
        for (uint8_t j = 0; j < num_devices; j++) {
            if ((i != j) && (get_sample_count(i, j) < 1000)) {
                return false;
            }
        }
    }
    return true;
}

void update_twr_cal_tx(struct message_spec_s *msg, int64_t transmit_tstamp)
{
    update_twr_tx(msg, transmit_tstamp);
}

bool update_twr_cal_rx(struct message_spec_s *msg, struct tx_spec_s *tx_spec, int64_t receive_tstamp)
{
    for (uint8_t i = 0; i < MAX_NUM_DEVICES; i++) {
        if (msg->ds_twr_data[i].twr_status == TWR_LOCKED) {
            push_calib_data(msg->ds_twr_data[i].tprop, msg->tx_spec.data_slot_id, i);
        }
    }
    update_twr_rx(msg, tx_spec, receive_tstamp);
    body_pos[msg->tx_spec.data_slot_id][0] = msg->tx_spec.body_pos[0];
    body_pos[msg->tx_spec.data_slot_id][1] = msg->tx_spec.body_pos[1];
    body_pos[msg->tx_spec.data_slot_id][2] = msg->tx_spec.body_pos[2];

    body_pos[tx_spec->data_slot_id][0] = tx_spec->body_pos[0];
    body_pos[tx_spec->data_slot_id][1] = tx_spec->body_pos[1];
    body_pos[tx_spec->data_slot_id][2] = tx_spec->body_pos[2];

    if (twr_status[msg->tx_spec.data_slot_id] == TWR_LOCKED) {
        //push our own range as well
        push_calib_data(range[msg->tx_spec.data_slot_id], tx_spec->data_slot_id, msg->tx_spec.data_slot_id);
    }
    if (is_data_collection_complete(msg->tdma_spec.num_slots)) {
        do_cal(msg->tdma_spec.num_slots, tx_spec->node_id);
        tx_spec->ant_delay_cal_status = ANT_DELAY_CAL_COMPLETED;
        tx_spec->ant_delay = (uint32_t)ant_delay;
        return true;
    }
    return false;
}

void print_cal_status(struct tdma_spec_s *tdma_spec, struct tx_spec_s *tx_spec)
{
    uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "\n\nCAL", "\n\nNID: %x NUM_SLOTS: %d CS: %d ANT_DELAY: %f", 
        tx_spec->node_id, tdma_spec->num_slots, tx_spec->ant_delay_cal_status, ant_delay);
    for (uint8_t i = 0; i < tdma_spec->num_slots; i++) {
        for (uint8_t j = 0; j < tdma_spec->num_slots; j++) {
            if (i != j) {
                uavcan_send_debug_msg(UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG, "CAL", "SID: %d %d DCOUNT: %d ", i, j, get_sample_count(i,j));// (int32_t)get_sample_dat(i,j));
            }
        }
    }
}

int32_t get_range(uint8_t id)
{
    return range[id];
}
