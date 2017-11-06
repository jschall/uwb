#include "tdma.h"

static struct dw1000_instance_s uwb_instance;
static struct tdma_spec_s tdma_spec;
static bool request_data_slot;
static uint8_t curr_slot;
static bool data_slot_allocated;
static struct tx_spec_s tx_spec;
static bool send_log_now;
static char print_dat[100];
static float prev_rx_tstamp;
//maintains list of slot to node id map
uint8_t slot_id_list[MAX_NUM_DEVICES];
static bool start_sent;
static uint64_t last_tx_stamp;
static uint64_t slot_start_timestamp;
/*

    Initialize TDMA vars

*/

#define DEBUG_PRINT  1

void tdma_init(uint8_t unique_id, uint8_t unit_type)
{
    tdma_spec.slot_size = SLOT_SIZE;
    tdma_spec.num_tx_online = 0;
    tdma_spec.res_data_slot = 0;
    tdma_spec.req_node_id = 0;
    tdma_spec.cnt = 0;
    tx_spec.node_id = unique_id;
    tx_spec.type = STATIC_TAG;
    tx_spec.pkt_cnt = 0;
    tx_spec.data_slot_id = 255;
    curr_slot = START_SLOT;
    start_sent = false;
    memset(slot_id_list, 0, sizeof(slot_id_list));
    if (unit_type == TDMA_SUPERVISOR) {
        twr_init(unique_id, 0);
    }
}

/*

    Supervisor Runner

*/
#if MODULE_TYPE == MODULE_TYPE_SUPER

static void super_update_start_slot()
{
    struct message_spec_s msg;
    tdma_spec.slot_size = SLOT_SIZE;
    tdma_spec.cnt++;
    tx_spec.pkt_cnt++;
    //pack message
    msg.tdma_spec = tdma_spec;
    msg.tx_spec = tx_spec;
    msg.target_node_id = 0; //Reset Target Node ID
    dw1000_disable_transceiver(&uwb_instance);
    uint64_t scheduled_time = (dw1000_get_sys_time(&uwb_instance)+ARB_TIME_SYS_TICKS)&0xFFFFFFFFFFFFFE00ULL;
    setup_next_trip(slot_id_list, tdma_spec.num_tx_online);
    send_ranging_pkt(&msg.ds_twr_data, &msg.target_node_id, scheduled_time + dw1000_get_ant_delay(&uwb_instance));
    dw1000_scheduled_transmit(&uwb_instance, scheduled_time, sizeof(msg), &msg, false);
    dw1000_rx_enable(&uwb_instance);
}

static void handle_request_slot()
{
    struct message_spec_s msg;
    struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(msg), &msg);

    if (rx_info.err_code == DW1000_RX_ERROR_NONE) {
        for (uint8_t i = 0; i < tdma_spec.num_tx_online; i++) {
            if(slot_id_list[i] == msg.tx_spec.node_id) {
                tdma_spec.req_node_id = msg.tx_spec.node_id;
                tdma_spec.res_data_slot = i + 1;
#ifdef DEBUG_PRINT
                uavcan_acquire();
                sprintf(print_dat,"Repeat Slot: NID:%x TX_TSTAMP: %f PREV_MSG: %f THIS_MSG:%f", \
                    msg.tx_spec.node_id, dw1000_get_tx_stamp(&uwb_instance)/UWB_SYS_TICKS, prev_rx_tstamp, rx_info.timestamp/UWB_SYS_TICKS);
                uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "Super: ", print_dat);
                uavcan_release();
#endif
                return;
            }
        }
        if(tdma_spec.num_tx_online > MAX_NUM_DEVICES) {
            //We only support upto MAX_NUM_DEVICES Devices
            return;
        }
        tdma_spec.num_tx_online++;
        tdma_spec.req_node_id = msg.tx_spec.node_id;
        tdma_spec.res_data_slot = tdma_spec.num_tx_online;
        //record allocated slot id
        slot_id_list[tdma_spec.res_data_slot] = msg.tx_spec.node_id;
        uavcan_acquire();
        uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "Super: ", "New Node Discovered");
        uavcan_release();
    }
}

static void update_data_slot(bool supervisor)
{
    struct message_spec_s msg;
    //We shall receive a data packet sometime in the middle of this sleep
    struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(msg), &msg);
    //Handle Data Packet
    if (rx_info.err_code == DW1000_RX_ERROR_NONE) {
        prev_rx_tstamp = rx_info.timestamp/UWB_SYS_TICKS;
        if (msg.target_node_id == tx_spec.node_id) {
            parse_ranging_pkt(&msg.ds_twr_data, msg.tx_spec.node_id, rx_info.timestamp);
        }
#ifdef DEBUG_PRINT
        if(msg.target_node_id == 255) {
            uavcan_acquire();
            sprintf(print_dat,"Wrong Request: NID:%x TX_TSTAMP: %f PREV_MSG: %f THIS_MSG:%f", \
                msg.tx_spec.node_id, dw1000_get_tx_stamp(&uwb_instance)/UWB_SYS_TICKS, prev_rx_tstamp, rx_info.timestamp/UWB_SYS_TICKS);
            uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "Super: ", print_dat);
            uavcan_release();
        }
#endif
    }

    dw1000_rx_enable(&uwb_instance);
}

static void update_curr_slot()
{
    static uint64_t last_start_time;
    if (last_start_time == 0 || (micros() - last_start_time) >= ((tdma_spec.num_tx_online+2)*SLOT_SIZE)) {
        curr_slot = START_SLOT;
        last_start_time = micros();
    } else if (tdma_spec.num_tx_online > 0 && (micros() - last_start_time) < ((tdma_spec.num_tx_online + 1)*SLOT_SIZE)) {
        curr_slot = DATA_SLOT;
    } else {
        curr_slot = REQUEST_SLOT;
    }
}

void tdma_supervisor_run()
{
    dw1000_init(&uwb_instance, 3, BOARD_PAL_LINE_SPI3_UWB_CS, BOARD_PAL_LINE_UWB_NRST);
    dw1000_rx_enable(&uwb_instance);
    uint16_t cnt = 0;
    tx_spec.data_slot_id = 0;
    while (true) {
        update_curr_slot(); //update slots based on CPU clock
        switch (curr_slot) {
            case START_SLOT:
                cnt++;
                //we only send start here incase of faillure or initialisation
                super_update_start_slot();
                break;
            case DATA_SLOT:
                update_data_slot(true);
                break;
            case REQUEST_SLOT:
                handle_request_slot();
                break;
            default:
                break;
        };
        chThdSleepMicroseconds(SLOT_SIZE/4);
        /*
        if (cnt % 2000 == 0) {
            print_tdma_spec();
            send_log_now = true;
        } else {
            send_log_now = false;
        }*/
    }
}
#endif

/*

    Subordinate Runner

*/

#if MODULE_TYPE == MODULE_TYPE_SUB
static void req_data_slot(struct dw1000_rx_frame_info_s rx_info)
{
    struct message_spec_s msg;
    uint64_t scheduled_time = (rx_info.timestamp+DW1000_SID2ST(tdma_spec.num_tx_online+1))&0xFFFFFFFFFFFFFE00ULL;
    //pack message
    msg.tdma_spec = tdma_spec;
    msg.tx_spec = tx_spec;
    msg.target_node_id = 255; //we are requesting data slot

    dw1000_disable_transceiver(&uwb_instance);
    if(dw1000_scheduled_transmit(&uwb_instance, scheduled_time, sizeof(msg), &msg, false)) {
        uavcan_acquire();
        sprintf(print_dat, "Requesting Data Slot @ %d for %x", tdma_spec.num_tx_online+1, tx_spec.node_id);
        uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "Sub: ", print_dat);
        uavcan_release();
    } else {
        uavcan_acquire();
        uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "Sub: ", "Requesting Data Slot Failed");
        uavcan_release();
    }
    //Go and wait for time slot allocation
    dw1000_rx_enable(&uwb_instance);
}

static void update_subordinate()
{
    struct message_spec_s msg;
    struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(msg), &msg);
    static bool transmit_scheduled = false;
    if(rx_info.err_code == DW1000_RX_ERROR_NONE) {
        slot_id_list[msg.tx_spec.data_slot_id] = msg.tx_spec.node_id;
        if(msg.tx_spec.data_slot_id == 0) { //this is a message from supervisor
            transmit_scheduled = false;
            //Also setup ranging request of our own
            setup_next_trip(slot_id_list, tdma_spec.num_tx_online);
            slot_start_timestamp = rx_info.timestamp;
            tdma_spec = msg.tdma_spec;
            //check if it's our turn to ask for data slot, if yes request
            if(tdma_spec.cnt%256 == tx_spec.node_id && !data_slot_allocated) {
                req_data_slot(rx_info);
            }

            if (!data_slot_allocated && tdma_spec.req_node_id == tx_spec.node_id) {
                tx_spec.data_slot_id = tdma_spec.res_data_slot;
                data_slot_allocated = true;
                twr_init(tx_spec.node_id, tx_spec.data_slot_id);
            }
        } /*else {
            //***Handle message Here****
            if (msg.tx_spec.data_slot_id > tx_spec.data_slot_id) {
                slot_id_list[msg.tx_spec.data_slot_id - tx_spec.data_slot_id] = msg.tx_spec.node_id;
            }
        }*/
        if(!data_slot_allocated) {
            return;
        }
        if(msg.target_node_id == tx_spec.node_id) {
            parse_ranging_pkt(&msg.ds_twr_data, msg.tx_spec.node_id, rx_info.timestamp);
        }
    }
    if (!transmit_scheduled && data_slot_allocated) {
        if ((dw1000_get_sys_time(&uwb_instance) - slot_start_timestamp) >= (DW1000_SID2ST(tx_spec.data_slot_id-1) + ARB_TIME_SYS_TICKS)) {
            dw1000_disable_transceiver(&uwb_instance);
            //setup future transmit
            uint64_t scheduled_time = (slot_start_timestamp+DW1000_SID2ST(tx_spec.data_slot_id))&0xFFFFFFFFFFFFFE00ULL;
            //create TWR data set to send
            //pack message
            msg.tdma_spec = tdma_spec;
            msg.tx_spec = tx_spec;
            msg.target_node_id = 0; // reset target node id
            send_ranging_pkt(&msg.ds_twr_data, &msg.target_node_id, scheduled_time + dw1000_get_ant_delay(&uwb_instance));
            if (dw1000_scheduled_transmit(&uwb_instance, scheduled_time, sizeof(msg), &msg, true)) {
                tx_spec.pkt_cnt++;
            }
            transmit_scheduled = true;
        }
    }
}

void tdma_subordinate_run()
{
    dw1000_init(&uwb_instance, 3, BOARD_PAL_LINE_SPI3_UWB_CS, BOARD_PAL_LINE_UWB_NRST);
    dw1000_rx_enable(&uwb_instance);
    uint16_t cnt = 0;

    while (true) {
        update_subordinate();
        //Send Status
        /*if (cnt % 4000 <= 8) {
            print_tdma_spec();
            send_log_now = true;
        } else {
            send_log_now = false;
        }*/
        cnt++;
        chThdSleepMicroseconds(SLOT_SIZE/4);
    }
}
#endif
/*

    TDMA Sniffer Runner

*/
#if MODULE_TYPE == MODULE_TYPE_SNIFFER
static bool sample_collected = false;
static void print_tdma_spec()
{
    sprintf(print_dat,"ONLINE: %d REQ_NID: %x CNT: %d", tdma_spec.num_tx_online, tdma_spec.req_node_id , tdma_spec.cnt);
    uavcan_acquire();
    uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "TDMA", print_dat);
    uavcan_release();
}

static uint8_t cal_node_id_list[3] = {0};
static uint8_t num_cal_devices = 0;
static void print_twr(struct ds_twr_data_s twr)
{
    //sprintf(print_dat, "TID: %d DeviceA: %x DeviceB: %x TSTAT: %d", twr.trip_id, twr.deviceA, twr.deviceB, twr.trip_status);
    //uavcan_acquire();
    //uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "TWR", print_dat);
    //uavcan_release();
    if(twr.trip_status == DS_TWR_SOL) {
        int8_t id1 = -1, id2 = -1;
        for (uint8_t i = 0; i < 3; i++) {
            if (cal_node_id_list[i] == twr.deviceA) {
                id1 = i;
            }
            if (cal_node_id_list[i] == twr.deviceB) {
                id2 = i;
            }
        }
        sprintf(print_dat,"Sample Cnt: %d DeviceA: %x DeviceB: %x Dist: %f/%f", get_sample_count(id1, id2), twr.deviceA, twr.deviceB, twr.tprop, get_sample_dat(id1,id2));
        uavcan_acquire();
        uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "TWR", print_dat);
        uavcan_release();
        if(sample_collected) {
            uavcan_acquire();
            sprintf(print_dat, " D0x%x: %f/%d D0x%x: %f/%d D0x%x: %f/%d",
             /*get_sample_dat(0,1), get_sample_dat(0,2), get_sample_dat(1,0), get_sample_dat(1,2),
             get_sample_dat(2,0), get_sample_dat(2,1),*/
             cal_node_id_list[0],
             get_result(0),(uint32_t)(get_result(0)*METERS_TO_TIME), cal_node_id_list[1],
             get_result(1), (uint32_t)(get_result(1)*METERS_TO_TIME), cal_node_id_list[2],
             get_result(2), (uint32_t)(get_result(2)*METERS_TO_TIME));
            uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "TWR", print_dat);
            uavcan_release();
        }
    }

    /*sprintf(print_dat, "TID: %d TSTAT: %d T1: %lld T2: %lld T3: %lld", twr.trip_id, twr.trip_status, 
        twr.transmit_tstamps[0], twr.transmit_tstamps[1], twr.transmit_tstamps[2]);
    uavcan_acquire();
    uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "TWR", print_dat);
    uavcan_release();

    sprintf(print_dat, "TPROP: %f R1: %lld R2: %lld R3: %lld", twr.tprop,
                twr.receive_tstamps[0], twr.receive_tstamps[1], twr.receive_tstamps[2]);
    uavcan_acquire();
    uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "TWR", print_dat);
    uavcan_release();*/
}
static void process_twr(struct ds_twr_data_s twr, struct tx_spec_s tx_spec)
{
    if(tx_spec.data_slot_id < 3) {
        sample_collected = false;
        cal_node_id_list[tx_spec.data_slot_id] = tx_spec.node_id;
    }
    if(twr.trip_status == DS_TWR_SOL) {
        int8_t id1 = -1, id2 = -1;
        for (uint8_t i = 0; i < 3; i++) {
            if (cal_node_id_list[i] == twr.deviceA) {
                id1 = i;
            }
            if (cal_node_id_list[i] == twr.deviceB) {
                id2 = i;
            }
        }
        if (id1 != -1 && id2 != -1) {
            if(push_calib_data(twr.tprop, id1, id2)) {
                sample_collected = true;
            }
        }
    }
}

static void print_info(struct message_spec_s msg, struct dw1000_rx_frame_info_s rx_info)
{
    if(!send_log_now) {
        return;
    }
    //print_tdma_spec();
    /*sprintf(print_dat,"ONLINE:%d T: %.0f SLOT: %d NID: %x \nPKT_CNT: %d TX_SLOT: %d TNID: %d", \
        msg.tdma_spec.num_tx_online, rx_info.timestamp/UWB_SYS_TICKS, curr_slot, msg.tx_spec.node_id, msg.tx_spec.pkt_cnt , \
        msg.tx_spec.data_slot_id, msg.target_node_id);
    uavcan_acquire();
    uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "RX", print_dat);
    uavcan_release();*/
    print_twr(msg.ds_twr_data);
}

void tdma_sniffer_run()
{
    dw1000_init(&uwb_instance, 3, BOARD_PAL_LINE_SPI3_UWB_CS, BOARD_PAL_LINE_UWB_NRST);
    dw1000_rx_enable(&uwb_instance);
    struct message_spec_s msg;
    uint32_t cnt = 0;
    curr_slot = 0;
    while(true) {
        struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(msg), &msg);
        if(rx_info.err_code == DW1000_RX_ERROR_NONE) {
            cnt++;
            process_twr(msg.ds_twr_data, msg.tx_spec);
            tdma_spec = msg.tdma_spec;
            print_info(msg, rx_info);
        }
        if(cnt % 200 <= 20) {
            send_log_now = true;
        } else {
            send_log_now = false;
        }
        chThdSleepMicroseconds(SLOT_SIZE/4);
    }
}
#endif
