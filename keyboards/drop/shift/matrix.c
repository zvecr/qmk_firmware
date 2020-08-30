// Copyright 2022 zvecr <git@zvecr.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"
#include "spi_master.h"

#define SR_KC_RCLK_PIN              B6
#define SR_KC_OE_N_PIN              B7
#define SR_KC_DATAOUT_PIN           B8
#define SR_KC_SCLK_PIN              B9

#define CLOCK_DELAY 10

static void _shift_init_impl(void) {
    setPinOutput(SR_KC_RCLK_PIN);
    setPinOutput(SR_KC_DATAOUT_PIN);
    setPinOutput(SR_KC_SCLK_PIN);
}

static void _shift_out_impl(const uint8_t *data, uint16_t length) {
    writePinLow(SR_KC_RCLK_PIN);
    for (uint16_t i = 0; i < length; i++) {
        uint8_t val = data[i];

        // shift out msb first
        for (uint8_t bit = 0; bit < 8; bit++) {
            writePin(SR_KC_DATAOUT_PIN, !!(val & (1 << bit)));
            writePin(SR_KC_SCLK_PIN, true);
            wait_us(CLOCK_DELAY);

            writePin(SR_KC_SCLK_PIN, false);
            wait_us(CLOCK_DELAY);
        }
    }
    writePinHigh(SR_KC_RCLK_PIN);
}

static void _shift_out(const uint8_t *data, uint16_t length) { _shift_out_impl(data, length); }

static void _shift_enable(void) {
    setPinOutput(SR_KC_OE_N_PIN);
    writePinLow(SR_KC_OE_N_PIN);
}

static void _shift_disable(void) {
    setPinOutput(SR_KC_OE_N_PIN);
    writePinHigh(SR_KC_OE_N_PIN);
}

static void _shift_init(void) {
    _shift_disable();
    _shift_init_impl();
}

static void matrix_shift_out(uint16_t data) {
    uint8_t tmp[2] = {
        (data >> 8) & 0xFF,  // Shift in bits 8-15
        data & 0xFF,         // Shift in bits 0-7
    };
    _shift_out(tmp, 2);
}

static void matrix_init_shifter(void) {
    _shift_init();

    matrix_shift_out(0);

    _shift_enable();
}

static pin_t row_pins[MATRIX_ROWS] = { A0, A1, A2, A3, A4, A5, A6, A7 };

static void select_col(uint8_t col) {
    matrix_shift_out(1 << col);
}

static void unselect_cols(void) {
    matrix_shift_out(0);
}

static void matrix_init_pins(void) {
    unselect_cols();
    for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
        setPinInputHigh(row_pins[x]);
    }
}

void matrix_read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col, matrix_row_t row_shifter) {
    // Select col
    select_col(current_col);
    matrix_io_delay();

    // For each row...
    for (uint8_t row_index = 0; row_index < MATRIX_ROWS; row_index++) {
        // Check row pin state
        if (readPin(row_pins[row_index]) == 0) {
            // Pin LO, set col bit
            current_matrix[row_index] |= row_shifter;
        } else {
            // Pin HI, clear col bit
            current_matrix[row_index] &= ~row_shifter;
        }
    }
}

void matrix_init_custom(void) {
    matrix_init_shifter();
    matrix_init_pins();
}

bool matrix_scan_custom(matrix_row_t curr_matrix[]) {
    matrix_row_t raw_matrix[MATRIX_ROWS] = {0};

    // Set col, read rows
    matrix_row_t row_shifter = MATRIX_ROW_SHIFTER;
    for (uint8_t current_col = 0; current_col < MATRIX_COLS; current_col++, row_shifter <<= 1) {
        matrix_read_rows_on_col(raw_matrix, current_col, row_shifter);
    }

    bool changed = memcmp(raw_matrix, curr_matrix, sizeof(raw_matrix)) != 0;
    if (changed) memcpy(raw_matrix, curr_matrix, sizeof(raw_matrix));

    return changed;
}
