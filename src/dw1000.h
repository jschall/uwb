#pragma once

#include <common/spi.h>

struct dw1000_instance_s {
    struct spi_device_s spi_dev;
    uint32_t reset_line;
};

enum dw1000_rx_frame_err_code_s {
    DW1000_RX_ERROR_NONE=0,
    DW1000_RX_ERROR_NULLPTR,
    DW1000_RX_ERROR_NO_FRAME_PRESENT,
    DW1000_RX_ERROR_PROVIDED_BUFFER_TOO_SMALL,
    DW1000_RX_ERROR_RXOVRR

};

struct dw1000_rx_frame_info_s {
    enum dw1000_rx_frame_err_code_s err_code;
    uint16_t len;
    uint64_t timestamp;
    int32_t rx_ttcko;
    uint32_t rx_ttcki;
};

void dw1000_init(struct dw1000_instance_s* instance, uint8_t spi_idx, uint32_t select_line, uint32_t reset_line);
struct dw1000_rx_frame_info_s dw1000_receive(struct dw1000_instance_s* instance, uint32_t buf_len, void* buf);
void dw1000_transmit(struct dw1000_instance_s* instance, uint32_t buf_len, void* buf);
void dw1000_try_receive(struct dw1000_instance_s* instance);
void dw1000_rx_enable(struct dw1000_instance_s* instance);
void dw1000_handle_interrupt(struct dw1000_instance_s* instance);
