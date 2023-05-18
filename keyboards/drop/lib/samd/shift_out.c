// Copyright 2023 Massdrop, Inc.
// SPDX-License-Identifier: GPL-2.0-or-later

#include "wait.h"
#include "gpio.h"

#define SR_USE_BITBANG

// Bodge for when spi_master is not available
#ifdef SR_USE_BITBANG
#    define CLOCK_DELAY 10

void shift_init_impl(void) {
    setPinOutput(SR_RCLK_PIN);
    setPinOutput(SR_DATAOUT_PIN);
    setPinOutput(SR_SCLK_PIN);
}

void shift_out_impl(const uint8_t *data, uint16_t length) {
    writePinLow(SR_RCLK_PIN);
    for (uint16_t i = 0; i < length; i++) {
        uint8_t val = data[i];

        // shift out lsb first
        for (uint8_t bit = 0; bit < 8; bit++) {
            writePin(SR_DATAOUT_PIN, !!(val & (1 << bit)));
            writePin(SR_SCLK_PIN, true);
            wait_us(CLOCK_DELAY);

            writePin(SR_SCLK_PIN, false);
            wait_us(CLOCK_DELAY);
        }
    }
    writePinHigh(SR_RCLK_PIN);
}

#else
#    include "spi_master.h"
void shift_init_impl(void) { spi_init(); }

void shift_out_impl(const uint8_t *data, uint16_t length) {
    spi_start(SR_RCLK_PIN, true, 0, 0);

    spi_transmit(data, length);

    spi_stop();
}
#endif

// ***************************************************************

void shift_out(const uint8_t *data, uint16_t length) { shift_out_impl(data, length); }

void shift_enable(void) {
    setPinOutput(SR_OE_PIN);
    writePinLow(SR_OE_PIN);
}

void shift_disable(void) {
    setPinOutput(SR_OE_PIN);
    writePinHigh(SR_OE_PIN);
}

void shift_init(void) {
    shift_disable();
    shift_init_impl();
}
