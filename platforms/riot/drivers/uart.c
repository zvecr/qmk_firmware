// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "periph/uart.h"
#include "isrpipe.h"

#include "uart.h"
#undef uart_init
#undef uart_write

#ifndef SERIAL_DRIVER
#    define SERIAL_DRIVER (UART_DEV(0))
#endif

#ifndef SD1_WRDLEN
#    define SD1_WRDLEN UART_DATA_BITS_5
#endif

#ifndef SD1_PARITY
#    define SD1_PARITY UART_PARITY_NONE
#endif

#ifndef SD1_STPBIT
#    define SD1_STPBIT UART_STOP_BITS_1
#endif

#define RX_BUFSIZE 64

static uint8_t   _rx_buf_mem[RX_BUFSIZE];
static isrpipe_t rx_isrpipe = ISRPIPE_INIT(_rx_buf_mem);

void _uart_init(uint32_t baud) {
    uart_init(SERIAL_DRIVER, baud, (uart_rx_cb_t)isrpipe_write_one, &rx_isrpipe);
    uart_mode(SERIAL_DRIVER, SD1_WRDLEN, SD1_PARITY, SD1_STPBIT);
}

void _uart_write(uint8_t data) {
    uart_write(SERIAL_DRIVER, &data, 1);
}

uint8_t _uart_read(void) {
    uint8_t data;
    isrpipe_read(&rx_isrpipe, &data, 1);
    return data;
}

void _uart_transmit(const uint8_t *data, uint16_t length) {
    uart_write(SERIAL_DRIVER, data, length);
}

void _uart_receive(uint8_t *data, uint16_t length) {
    isrpipe_read(&rx_isrpipe, data, length);
}

bool _uart_available(void) {
    return tsrb_avail(&rx_isrpipe.tsrb) > 0;
}
