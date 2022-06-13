/* Copyright 2021 QMK
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdbool.h>
#include "gpio.h"

typedef int16_t spi_status_t;

#define SPI_STATUS_SUCCESS (0)
#define SPI_STATUS_ERROR (-1)
#define SPI_STATUS_TIMEOUT (-2)

#define SPI_TIMEOUT_IMMEDIATE (0)
#define SPI_TIMEOUT_INFINITE (0xFFFF)

#ifdef __cplusplus
extern "C" {
#endif
void _spi_init(void);

bool _spi_start(pin_t slavePin, bool lsbFirst, uint8_t mode, uint16_t divisor);

spi_status_t _spi_write(uint8_t data);

spi_status_t _spi_read(void);

spi_status_t _spi_transmit(const uint8_t *data, uint16_t length);

spi_status_t _spi_receive(uint8_t *data, uint16_t length);

void _spi_stop(void);

#define spi_init() _spi_init()
#define spi_start(p, l, m, d) _spi_start(p, l, m, d)
#define spi_write(d) _spi_write(d)
#define spi_read _spi_read
#define spi_transmit(d, l) _spi_transmit(d, l)
#define spi_receive(d, l) _spi_receive(d, l)
#define spi_stop _spi_stop

#ifdef __cplusplus
}
#endif
