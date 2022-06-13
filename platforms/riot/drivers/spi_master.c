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

#include "periph/spi.h"
#include "spi_master.h"
#undef spi_init

#ifndef SPI_BUS_ID
#    define SPI_BUS_ID    (SPI_DEV(0))
#endif

static pin_t currentSlavePin = NO_PIN;

__attribute__((weak)) void _spi_init(void) {
    static bool is_initialised = false;
    if (!is_initialised) {
        is_initialised = true;

        spi_init(SPI_BUS_ID);
    }
}

bool _spi_start(pin_t slavePin, bool lsbFirst, uint8_t mode, uint16_t divisor) {
    if (currentSlavePin != NO_PIN || slavePin == NO_PIN) {
        return false;
    }

    if (lsbFirst) { // not supported
        return false;
    }

    spi_mode_t spi_mode;
    switch (mode) {
        case 0:
            spi_mode = SPI_MODE_0;
            break;
        case 1:
            spi_mode = SPI_MODE_1;
            break;
        case 2:
            spi_mode = SPI_MODE_2;
            break;
        case 3:
            spi_mode = SPI_MODE_3;
            break;
        default:
            return false;
    }

    // TODO: divisor -> clock?
    spi_clk_t spi_clock = SPI_CLK_1MHZ;

    currentSlavePin  = slavePin;
    setPinOutput(currentSlavePin);
    writePinLow(currentSlavePin);

    spi_acquire(SPI_BUS_ID, SPI_CS_UNDEF, spi_mode, spi_clock);
    return true;
}

spi_status_t _spi_write(uint8_t data) {
    spi_transfer_bytes(SPI_BUS_ID, SPI_CS_UNDEF, false, &data, NULL, 1);
    return SPI_STATUS_SUCCESS;
}

spi_status_t _spi_read(void) {
    uint8_t data = 0;
    spi_transfer_bytes(SPI_BUS_ID, SPI_CS_UNDEF, false, NULL, &data, 1);
    return data;
}

spi_status_t _spi_transmit(const uint8_t *data, uint16_t length) {
    spi_transfer_bytes(SPI_BUS_ID, SPI_CS_UNDEF, false, data, NULL, length);
    return SPI_STATUS_SUCCESS;
}

spi_status_t _spi_receive(uint8_t *data, uint16_t length) {
    spi_transfer_bytes(SPI_BUS_ID, SPI_CS_UNDEF, false, NULL, data, length);
    return SPI_STATUS_SUCCESS;
}

void _spi_stop(void) {
    if (currentSlavePin != NO_PIN) {
        writePinHigh(currentSlavePin);
        currentSlavePin = NO_PIN;

        spi_release(SPI_BUS_ID);
    }
}
