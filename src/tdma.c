#include "tdma.h"

static struct dw1000_instance_s uwb_instance;
static struct tdma_spec_s tdma_spec;
static bool request_data_slot;
static uint8_t curr_slot;
static bool data_slot_allocated;
static struct tx_spec_s tx_spec;

static void super_update_start_slot()
{
	tdma_spec.slot_size = SLOT_SIZE;
	tdma_spec.cnt++;
    dw1000_disable_transceiver(&uwb_instance);
    dw1000_transmit(&uwb_instance, sizeof(tdma_spec), (char*)&tdma_spec, false);
    //wait for start transmission to complete
    chThdSleepMicroseconds(500);
    //setup future transmit
	//create TWR data set to send

    dw1000_disable_transceiver(&uwb_instance);
	uint64_t scheduled_time = (dw1000_get_tx_stamp(&uwb_instance)+SLOT_SIZE_TICKS)&0xFFFFFFFFFFFFFE00ULL;
    if (dw1000_scheduled_transmit(&uwb_instance, scheduled_time, sizeof(tx_spec), (char*)&tx_spec, true)) {
		//goto waiting for next opportunity
    	curr_slot = START_SLOT;
    } else {
		curr_slot = DATA_SLOT;
	}
    curr_slot = DATA_SLOT;
}


static void handle_request_slot()
{
	struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(tx_spec), &tx_spec);
	if (rx_info.err_code == DW1000_RX_ERROR_NONE) {
		tdma_spec.num_tx_online++;
		tdma_spec.req_node_id = tx_spec.node_id;
		tdma_spec.res_data_slot = tdma_spec.num_tx_online;
        uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "New Node Discovered", "");
	}
}

static void sub_update_start_slot()
{
	struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(tdma_spec), &tdma_spec);
	if(rx_info.err_code == DW1000_RX_ERROR_NONE) {
		if(tdma_spec.cnt%tx_spec.node_id == 0 && !data_slot_allocated) {
			//check if it's our turn to ask for data slot
			request_data_slot = true;
		} else if(!data_slot_allocated) {
			return;
		}
		if (request_data_slot && tdma_spec.req_node_id == tx_spec.node_id) {
			tx_spec.data_slot_id = tdma_spec.res_data_slot;
			data_slot_allocated = true;
		} else if(request_data_slot) {
			handle_request_slot(rx_info);
			request_data_slot = false;
			return;
		}

		//setup future transmit
    	uint64_t scheduled_time = (rx_info.timestamp+(tx_spec.data_slot_id+1)*SLOT_SIZE_TICKS)&0xFFFFFFFFFFFFFE00ULL;
		//create TWR data set to send

        dw1000_disable_transceiver(&uwb_instance);
        if (dw1000_scheduled_transmit(&uwb_instance, scheduled_time, sizeof(tx_spec), (char*)&tx_spec, true)) {
        	curr_slot = START_SLOT;
        	tx_spec.pkt_cnt++;
        } else {
        	//goto waiting for next opportunity
			curr_slot = START_SLOT;
		}
	}
}

static void update_data_slot(bool supervisor)
{
	static uint8_t slot_cnt = 1;
	if (slot_cnt >= tdma_spec.num_tx_online) {
		slot_cnt = 1;
		curr_slot = REQUEST_SLOT;
	}
	struct dw1000_rx_frame_info_s rx_info = dw1000_receive(&uwb_instance, sizeof(tx_spec), &tx_spec);
	slot_cnt++;
}

static void req_data_slot(struct dw1000_rx_frame_info_s rx_info)
{
	uint64_t scheduled_time = (rx_info.timestamp+(tdma_spec.num_tx_online+1)*SLOT_SIZE_TICKS)&0xFFFFFFFFFFFFFE00ULL;
	dw1000_disable_transceiver(&uwb_instance);
    if (dw1000_scheduled_transmit(&uwb_instance, scheduled_time, sizeof(tx_spec), (char*)&tx_spec, true)) {
    	//goto waiting for next opportunity
    	curr_slot = START_SLOT;
    } else {
		curr_slot = DATA_SLOT;
	}
}

void tdma_init(uint8_t unique_id)
{
	tdma_spec.slot_size = SLOT_SIZE;
	tdma_spec.num_tx_online = 1;
	tdma_spec.res_data_slot = 0;
	tdma_spec.req_node_id = 0;
	tdma_spec.cnt = 0;
	tx_spec.node_id = unique_id;
	tx_spec.type = STATIC_TAG;
	tx_spec.pkt_cnt = 0;
	tx_spec.data_slot_id = 0;
}

void tdma_supervisor_run()
{
    dw1000_init(&uwb_instance, 3, BOARD_PAL_LINE_SPI3_UWB_CS, BOARD_PAL_LINE_UWB_NRST);
	while (true) {
		switch (curr_slot) {
			case START_SLOT:
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
        chThdSleepMicroseconds(1000);
	}
}

void tdma_subordinate_run()
{
	dw1000_init(&uwb_instance, 3, BOARD_PAL_LINE_SPI3_UWB_CS, BOARD_PAL_LINE_UWB_NRST);
    dw1000_rx_enable(&uwb_instance);
    while (true) {
		switch (curr_slot) {
			case START_SLOT:
				sub_update_start_slot();
				break;
			case DATA_SLOT:
				update_data_slot(false);
				break;
			default:
				break;
		};
        chThdSleepMicroseconds(1000);
	}
}