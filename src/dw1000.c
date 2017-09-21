#include "dw1000.h"
#include <hal.h>
#include <ch.h>
#include <common/timing.h>
#include <common/uavcan.h>
#include <bswap.h>

#include <string.h>
#include <stdio.h>

#define DW1000_SYSTEM_CONFIGURATION_FILE 0x04
#define DW1000_TRANSMIT_FRAME_CONTROL_FILE 0x08
#define DW1000_TRANSMIT_DATA_BUFFER_FILE 0x09
#define DW1000_SYSTEM_CONTROL_REGISTER_FILE 0x0D
#define DW1000_RECEIVE_DATA_BUFFER_FILE 0x11
#define DW1000_SYSTEM_EVENT_STATUS_REGISTER_FILE 0x0F


struct __attribute__((packed)) dw1000_transaction_header_s {
    struct { // octet 1
        uint8_t regfile:6;        // [0:5]
        uint8_t subidx_present:1; // [6]
        uint8_t write:1;          // [7]
    };
    struct { // octet 2
        uint8_t addressl:7;       // [0:6]
        uint8_t extended_address:1; // [7]
    };
    struct { // octet 3
        uint8_t addressh;         // [0:7]
    };
};

struct __attribute__((packed)) dw1000_tx_fctrl_s {
    struct { // register 08:00
        uint32_t TFLEN:7;    // [0:6]
        uint32_t TFLE:3;     // [7:9]
        uint32_t reserved:3; // [10:12]
        uint32_t TXBR:2;     // [13:14]
        uint32_t TR:1;       // [15]
        uint32_t TXPRF:2;    // [16:17]
        uint32_t TXPSR:2;    // [18:19]
        uint32_t PE:2;       // [20:21]
        uint32_t TXBOFFS:10; // [22:31]
    };
    struct { // register 08:04
        uint8_t IFSDELAY:8; // [0:7]
    };
};

// struct __attribute__((packed)) dw1000_sys_ctrl_s {
//     struct __attribute__((packed)) { // register 0D:00
//         uint8_t SFCST:1;    // [0]
//         uint8_t TXSTRT:1;   // [1]
//     };
// };

static void dw1000_write(struct dw1000_instance_s* instance, uint8_t regfile, uint16_t reg, const void* buf, uint32_t len);
static void dw1000_read(struct dw1000_instance_s* instance, uint8_t regfile, uint16_t reg, volatile void* buf, uint32_t len);
static void dw1000_reset(struct dw1000_instance_s* instance);

void dw1000_init(struct dw1000_instance_s* instance, uint8_t spi_idx, uint32_t select_line, uint32_t reset_line) {
    if (!instance) {
        return;
    }

    spi_device_init(&instance->spi_dev, spi_idx, select_line, 20000000, 8, 0);
    instance->reset_line = reset_line;
    dw1000_reset(instance);
}

void dw1000_rx_enable(struct dw1000_instance_s* instance) {
    uint32_t sys_cfg = (1<<9);
    dw1000_write(instance, DW1000_SYSTEM_CONFIGURATION_FILE, 0, &sys_cfg, sizeof(sys_cfg));
    uint32_t sys_ctrl = (1<<8);
    dw1000_write(instance, DW1000_SYSTEM_CONTROL_REGISTER_FILE, 0, &sys_ctrl, sizeof(sys_ctrl));
}

void dw1000_try_receive(struct dw1000_instance_s* instance) {
    volatile uint32_t sys_status = 0;
    dw1000_read(instance, DW1000_SYSTEM_EVENT_STATUS_REGISTER_FILE, 0, &sys_status, sizeof(sys_status));

    if (((sys_status>>13)&1) != 0) {
        volatile uint8_t rxbuf[12];
        char msg[80];
        dw1000_read(instance, DW1000_RECEIVE_DATA_BUFFER_FILE, 0, rxbuf, sizeof(rxbuf));
        memcpy(msg, rxbuf, 10);
        msg[10] = 0;
        uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "", msg);
        dw1000_write(instance, DW1000_SYSTEM_EVENT_STATUS_REGISTER_FILE, 0, &sys_status, sizeof(sys_status));
        uint32_t sys_ctrl = (1<<8)|(1<<24); // HRBPT
        dw1000_write(instance, DW1000_SYSTEM_CONTROL_REGISTER_FILE, 0, &sys_ctrl, sizeof(sys_ctrl));
    }
}

void dw1000_transmit(struct dw1000_instance_s* instance) {
    if (!instance) {
        return;
    }

    uint32_t sys_status = 0;
    dw1000_read(instance, 0x0f, 0, &sys_status, sizeof(sys_status));

//     if (((sys_status>>7)&1) != 0) {
//         uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "", "TXFRS 1");
//     } else {
//         uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "", "TXFRS 0");
//     }

    const char* txbuf = "HELLO WRLD";
    dw1000_write(instance, DW1000_TRANSMIT_DATA_BUFFER_FILE, 0, txbuf, strlen(txbuf));

//     char msg[80];
//     dw1000_read(instance, DW1000_TRANSMIT_DATA_BUFFER_FILE, 0, msg, strlen(txbuf));
//     uavcan_send_debug_logmessage(UAVCAN_LOGLEVEL_DEBUG, "", msg);

    // write tx_fctrl
//     struct dw1000_tx_fctrl_s tx_fctrl;
//     tx_fctrl.TFLEN = sizeof(msg)+2;
//     tx_fctrl.TFLE = 0;
//     tx_fctrl.reserved = 0;
//     tx_fctrl.TXBR = 0;
//     tx_fctrl.TR = 0;
//     tx_fctrl.TXPRF = 2;
//     tx_fctrl.TXPSR = 2;
//     tx_fctrl.PE = 0;
//     tx_fctrl.TXBOFFS = 0;
//     tx_fctrl.IFSDELAY = 0;
//
//     dw1000_write(instance, DW1000_TRANSMIT_FRAME_CONTROL_FILE, 0, &tx_fctrl, sizeof(tx_fctrl));

    uint32_t sys_ctrl = 1<<1;
    dw1000_write(instance, DW1000_SYSTEM_CONTROL_REGISTER_FILE, 0, &sys_ctrl, sizeof(sys_ctrl));


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

static void dw1000_write(struct dw1000_instance_s* instance, uint8_t regfile, uint16_t reg, const void* buf, uint32_t len) {
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

static void dw1000_read(struct dw1000_instance_s* instance, uint8_t regfile, uint16_t reg, volatile void* buf, uint32_t len) {
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
