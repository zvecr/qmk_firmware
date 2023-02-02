// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "periph/spi.h"
#include "spi_master.h"
#undef spi_init

#ifndef SPI_BUS_ID
#    define SPI_BUS_ID (SPI_DEV(0))
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
    spi_clk_t spi_clock = SPI_CLK_5MHZ;

    currentSlavePin = slavePin;
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
