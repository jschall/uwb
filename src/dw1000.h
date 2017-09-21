#pragma once

#include <common/spi.h>

struct dw1000_instance_s {
    struct spi_device_s spi_dev;
    uint32_t reset_line;
};

void dw1000_init(struct dw1000_instance_s* instance, uint8_t spi_idx, uint32_t select_line, uint32_t reset_line);
void dw1000_transmit(struct dw1000_instance_s* instance);
void dw1000_try_receive(struct dw1000_instance_s* instance);
void dw1000_rx_enable(struct dw1000_instance_s* instance);
