#include "dw1000.h"
#include "dw1000_internal.h"
#include <hal.h>
#include <ch.h>
#include <common/timing.h>
#include <common/uavcan.h>
#include <bswap.h>

#include <string.h>
#include <stdio.h>

static void dw1000_write(struct dw1000_instance_s* instance, uint8_t regfile, uint16_t reg, uint32_t len, const void* buf);
static void dw1000_read(struct dw1000_instance_s* instance, uint8_t regfile, uint16_t reg, uint32_t len, void* buf);
static void dw1000_reset(struct dw1000_instance_s* instance);
static void dw1000_clear_double_buffered_status_bits(struct dw1000_instance_s* instance);
static void dw1000_disable_transceiver(struct dw1000_instance_s* instance);
static void dw1000_swap_rx_buffers(struct dw1000_instance_s* instance);
static void dw1000_write8(struct dw1000_instance_s* instance, uint8_t regfile, uint16_t reg, uint8_t val);
static void dw1000_write16(struct dw1000_instance_s* instance, uint8_t regfile, uint16_t reg, uint16_t val);
static void dw1000_write32(struct dw1000_instance_s* instance, uint8_t regfile, uint16_t reg, uint32_t val);
static void dw1000_otp_read(struct dw1000_instance_s* instance, uint16_t otp_addr, uint8_t otp_field_len, void* buf);

void dw1000_init(struct dw1000_instance_s* instance, uint8_t spi_idx, uint32_t select_line, uint32_t reset_line) {
    if (!instance) {
        return;
    }

    spi_device_init(&instance->spi_dev, spi_idx, select_line, 20000000, 8, 0);
    instance->reset_line = reset_line;
    dw1000_reset(instance);

    // DW1000 User Manual Section 2.5.5.1
    dw1000_write16(instance, 0x23, 0x04, 0x8870);

    // DW1000 User Manual Section 2.5.5.2
    dw1000_write32(instance, 0x23, 0x0C, 0x2502A907);

    // DW1000 User Manual Section 2.5.5.3
    dw1000_write32(instance, 0x27, 0x08, 0x311A002D);

    // DW1000 User Manual Section 2.5.5.4
    dw1000_write8(instance, 0x2E, 0x0806, 0x0D);

    // DW1000 User Manual Section 2.5.5.5
    dw1000_write16(instance, 0x2E, 0x1806, 0x1607);

    // DW1000 User Manual Section 2.5.5.6
    dw1000_write32(instance, 0x1E, 0x00, 0x0E082848);

    // DW1000 User Manual Section 2.5.5.7
    dw1000_write32(instance, 0x28, 0x0C, 0x001E3FE0);

    // DW1000 User Manual Section 2.5.5.8
    dw1000_write8(instance, 0x2A, 0x0B, 0xC0);

    // DW1000 User Manual Section 2.5.5.9
    dw1000_write8(instance, 0x2B, 0x0B, 0xBE);

    // DW1000 User Manual Section 2.5.5.10
    dw1000_write16(instance, 0x36, 0x00, 0x0301);
    spi_device_set_max_speed_hz(&instance->spi_dev, 3000000);
    dw1000_write16(instance, 0x2D, 0x06, 0x8000);
    chThdSleepMicroseconds(150);
    dw1000_write16(instance, 0x36, 0x00, 0x0200);
    spi_device_set_max_speed_hz(&instance->spi_dev, 20000000);

    // DW1000 User Manual Section 2.5.5.11
    uint8_t ldotune_cal[5];
    dw1000_otp_read(instance, 0x04, 4, &ldotune_cal[0]);
    dw1000_otp_read(instance, 0x05, 1, &ldotune_cal[4]);
    if (ldotune_cal[0] != 0) {
        dw1000_write16(instance, 0x2C, 0x00, (1<<11)|(1<<12));
        dw1000_write(instance, 0x28, 0x30, sizeof(ldotune_cal), ldotune_cal);
    } else {
        dw1000_write16(instance, 0x2C, 0x00, (1<<11));
    }

    // Enable rx auto-re-enable and rx double-buffering
    struct dw1000_sys_cfg_s sys_cfg;
    memset(&sys_cfg, 0, sizeof(sys_cfg));
    sys_cfg.HIRQ_POL = 1;
    sys_cfg.RXAUTR = 1;
    dw1000_write(instance, DW1000_SYSTEM_CONFIGURATION_FILE, 0, sizeof(sys_cfg), &sys_cfg);
}

void dw1000_rx_enable(struct dw1000_instance_s* instance) {
    if (!instance) {
        return;
    }

    struct dw1000_sys_status_s sys_status;

    // Read SYS_STATUS to check that HSRBP == ICRBP
    dw1000_read(instance, DW1000_SYSTEM_EVENT_STATUS_REGISTER_FILE, 0, sizeof(sys_status), &sys_status);

    if (sys_status.HSRBP != sys_status.ICRBP) {
        // Issue the HRBPT command
        dw1000_swap_rx_buffers(instance);
    }

    // Set RXENAB bit
    struct dw1000_sys_ctrl_s sys_ctrl;
    memset(&sys_ctrl, 0, sizeof(sys_ctrl));
    sys_ctrl.RXENAB = 1;
    dw1000_write(instance, DW1000_SYSTEM_CONTROL_REGISTER_FILE, 0, sizeof(sys_ctrl), &sys_ctrl);
}

uint16_t dw1000_receive(struct dw1000_instance_s* instance, uint32_t buf_len, void* buf) {
    if (!instance || !buf) {
        return 0;
    }

    struct dw1000_sys_status_s sys_status;
    struct dw1000_rx_finfo_s rx_finfo;
    uint16_t ret;

    // Read SYS_STATUS
    dw1000_read(instance, DW1000_SYSTEM_EVENT_STATUS_REGISTER_FILE, 0, sizeof(sys_status), &sys_status);

    // Check if a good frame is in the buffer
    if (!sys_status.RXFCG) {
        return 0;
    }

    // Read RX_FINFO
    dw1000_read(instance, DW1000_RX_FRAME_INFORMATION_REGISTER_FILE, 0, sizeof(rx_finfo), &rx_finfo);

    // Check if the frame fits in the provided buffer
    if (rx_finfo.RXFLEN-2 <= buf_len) {
        // Read RX_BUFFER
        dw1000_read(instance, DW1000_RX_FRAME_BUFFER_FILE, 0, rx_finfo.RXFLEN-2, buf);
        ret = rx_finfo.RXFLEN-2;
    } else {
        ret = 0; // mitch: include some indication of this failure in the return status struct perhaps. alternatively consider returning what fits.
    }

    // Read SYS_STATUS
    dw1000_read(instance, DW1000_SYSTEM_EVENT_STATUS_REGISTER_FILE, 0, sizeof(sys_status), &sys_status);

    // Check RXOVRR
    if (sys_status.RXOVRR) {
        // Frames must be discarded (do not read frames) due to corrupted registers and TRXOFF command issued
        dw1000_disable_transceiver(instance);

        // Receiver must be reset to exit errored state
        dw1000_rx_enable(instance);

        return 0; // mitch: include some indication of this failure in the return status struct perhaps.
    }

    // Check HSRBP==ICRBP
    if (sys_status.HSRBP == sys_status.ICRBP) {
        // Mask, clear and unmask RX event flags in SYS_STATUS reg:0F; bits FCE, FCG, DFR, LDE_DONE
        dw1000_clear_double_buffered_status_bits(instance);
    }

    // Issue the HRBPT command
    dw1000_swap_rx_buffers(instance); // mitch: moved out of conditional

    return ret;
}

void dw1000_transmit(struct dw1000_instance_s* instance, uint32_t buf_len, void* buf) {
    if (!instance) {
        return;
    }

    if (buf_len > 125) {
        return;
    }

    // Write tx data to data buffer
    dw1000_write(instance, DW1000_TRANSMIT_DATA_BUFFER_FILE, 0, buf_len, buf);

    // Configure tx parameters
    struct dw1000_tx_fctrl_s tx_fctrl;
    dw1000_read(instance, DW1000_TRANSMIT_FRAME_CONTROL_FILE, 0, sizeof(tx_fctrl), &tx_fctrl);
    tx_fctrl.TFLEN = buf_len+2;
    dw1000_write(instance, DW1000_TRANSMIT_FRAME_CONTROL_FILE, 0, sizeof(tx_fctrl), &tx_fctrl);

    // Start tx
    uint32_t sys_ctrl = 1<<1;
    dw1000_write(instance, DW1000_SYSTEM_CONTROL_REGISTER_FILE, 0, sizeof(sys_ctrl), &sys_ctrl);
}

static void dw1000_clear_double_buffered_status_bits_and_optionally_disable_transceiver(struct dw1000_instance_s* instance, bool transceiver_disable) {
    if (!instance) {
        return;
    }

    struct dw1000_sys_mask_s sys_mask_prev;
    struct dw1000_sys_mask_s sys_mask;
    struct dw1000_sys_ctrl_s sys_ctrl;
    struct dw1000_sys_status_s sys_status;

    // Read SYS_MASK
    dw1000_read(instance, DW1000_SYSTEM_EVENT_MASK_REGISTER_FILE, 0, sizeof(sys_mask), &sys_mask);
    sys_mask_prev = sys_mask;

    // Mask double buffered status bits FCE, FCG, DFR, LDE_DONE
    sys_mask.MRXFCE = 0;
    sys_mask.MRXFCG = 0;
    sys_mask.MRXDFR = 0;
    sys_mask.MLDEDONE = 0;

    // Write SYS_MASK
    dw1000_write(instance, DW1000_SYSTEM_EVENT_MASK_REGISTER_FILE, 0, sizeof(sys_mask), &sys_mask);

    if (transceiver_disable) {
        // Issue TRXOFF
        memset(&sys_ctrl, 0, sizeof(sys_ctrl));
        sys_ctrl.TRXOFF = 1;

        // Write SYS_CTRL
        dw1000_write(instance, DW1000_SYSTEM_CONTROL_REGISTER_FILE, 0, sizeof(sys_ctrl), &sys_ctrl);
    }

    // Clear double buffered status bits FCE, FCG, DFR, LDE_DONE
    memset(&sys_status, 0, sizeof(sys_status));
    sys_status.RXFCE = 1;
    sys_status.RXFCG = 1;
    sys_status.RXDFR = 1;
    sys_status.LDEDONE = 1;

    // Write SYS_STATUS
    dw1000_write(instance, DW1000_SYSTEM_EVENT_STATUS_REGISTER_FILE, 0, sizeof(sys_status), &sys_status);

    // Unmask double buffered status bits FCE, FCG, DFR, LDE_DONE
    sys_mask = sys_mask_prev;

    // Write SYS_MASK
    dw1000_write(instance, DW1000_SYSTEM_EVENT_MASK_REGISTER_FILE, 0, sizeof(sys_mask), &sys_mask);
}

static void dw1000_swap_rx_buffers(struct dw1000_instance_s* instance) {
    // Issue the HRBPT command
    struct dw1000_sys_ctrl_s sys_ctrl;
    memset(&sys_ctrl,0,sizeof(sys_ctrl));
    sys_ctrl.HRBPT = 1;
    dw1000_write(instance, DW1000_SYSTEM_CONTROL_REGISTER_FILE, 0, sizeof(sys_ctrl), &sys_ctrl);
}

static void dw1000_clear_double_buffered_status_bits(struct dw1000_instance_s* instance) {
    dw1000_clear_double_buffered_status_bits_and_optionally_disable_transceiver(instance, false);
}

static void dw1000_disable_transceiver(struct dw1000_instance_s* instance) {
    dw1000_clear_double_buffered_status_bits_and_optionally_disable_transceiver(instance, true);
}

void dw1000_handle_interrupt(struct dw1000_instance_s* instance) {
    if (!instance) {
        return;
    }

}

static void dw1000_reset(struct dw1000_instance_s* instance) {
    if (!instance) {
        return;
    }

    palClearLine(instance->reset_line);
    palSetLineMode(instance->reset_line, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
    chThdSleepMilliseconds(2);
    palSetLineMode(instance->reset_line, 0);
    chThdSleepMilliseconds(2);
}

static void dw1000_write8(struct dw1000_instance_s* instance, uint8_t regfile, uint16_t reg, uint8_t val) {
    dw1000_write(instance, regfile, reg, sizeof(val), &val);
}

static void dw1000_write16(struct dw1000_instance_s* instance, uint8_t regfile, uint16_t reg, uint16_t val) {
    dw1000_write(instance, regfile, reg, sizeof(val), &val);
}

static void dw1000_write32(struct dw1000_instance_s* instance, uint8_t regfile, uint16_t reg, uint32_t val) {
    dw1000_write(instance, regfile, reg, sizeof(val), &val);
}

static void dw1000_write(struct dw1000_instance_s* instance, uint8_t regfile, uint16_t reg, uint32_t len, const void* buf) {
    if (!instance || !buf) {
        return;
    }

    struct __attribute__((packed)) dw1000_transaction_header_s header;
    header.subidx_present = 1;
    header.write = 1;
    header.extended_address = 1;
    header.regfile = regfile;
    header.addressl = reg;
    header.addressh = reg>>7;

    spi_device_begin(&instance->spi_dev);
    spi_device_send(&instance->spi_dev, sizeof(header), &header);
    spi_device_send(&instance->spi_dev, len, buf);
    spi_device_end(&instance->spi_dev);
}

static void dw1000_read(struct dw1000_instance_s* instance, uint8_t regfile, uint16_t reg, uint32_t len, void* buf) {
    if (!instance || !buf) {
        return;
    }

    struct __attribute__((packed)) dw1000_transaction_header_s header;
    header.subidx_present = 1;
    header.write = 0;
    header.extended_address = 1;
    header.regfile = regfile;
    header.addressl = reg;
    header.addressh = reg>>7;

    spi_device_begin(&instance->spi_dev);
    spi_device_send(&instance->spi_dev, sizeof(header), &header);
    spi_device_receive(&instance->spi_dev, len, buf);
    spi_device_end(&instance->spi_dev);
}

static void dw1000_otp_read(struct dw1000_instance_s* instance, uint16_t otp_addr, uint8_t otp_field_len, void* buf) {
    if (!instance || !buf) {
        return;
    }

    // DW1000 User Manual Section 6.3.3
    dw1000_write16(instance, 0x2D, 0x04, otp_addr);
    dw1000_write8(instance, 0x2D, 0x06, 0x03);
    dw1000_write8(instance, 0x2D, 0x06, 0x01);
    dw1000_read(instance, 0x2A, 0x04, otp_field_len, buf);
    dw1000_write8(instance, 0x2D, 0x06, 0x00);
}
