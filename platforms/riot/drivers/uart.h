// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>
#include <stdbool.h>

void    _uart_init(uint32_t baud);
void    _uart_write(uint8_t data);
uint8_t _uart_read(void);
void    _uart_transmit(const uint8_t *data, uint16_t length);
void    _uart_receive(uint8_t *data, uint16_t length);
bool    _uart_available(void);

#define uart_init(b) _uart_init(b)
#define uart_write(d) _uart_write(d)
#define uart_read _uart_read
#define uart_transmit(d, l) _uart_transmit(d, l)
#define uart_receive(d, l) _uart_receive(d, l)
#define uart_available _uart_available
