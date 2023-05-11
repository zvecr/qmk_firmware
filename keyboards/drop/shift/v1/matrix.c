// Copyright 2022 zvecr <git@zvecr.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"
#include "spi_master.h"

#define SR_KC_RCLK_PIN B6
#define SR_KC_OE_N_PIN B7

static void _shift_out(const uint8_t *data, uint16_t length) {
    spi_start(SR_KC_RCLK_PIN, false, 0, 0);

    spi_transmit(data, length);

    spi_stop();
}

static void _shift_out_u16(uint16_t data) {
    uint8_t tmp[2] = {
        (data >> 8) & 0xFF,  // Shift in bits 15-8
        data & 0xFF,         // Shift in bits 7-0
    };
    _shift_out(tmp, 2);
}

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
    spi_init();
}

static pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;

static void select_col(uint8_t col) {
    //
    _shift_out_u16(1 << col);
}

static void unselect_cols(void) {
    //
    _shift_out_u16(0);
}

static void matrix_init_pins(void) {
    unselect_cols();
    for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
        setPinInputLow(row_pins[x]);
    }
}

static void matrix_read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col, matrix_row_t row_shifter) {
    select_col(current_col);
    matrix_io_delay();

    // For each row...
    for (uint8_t row_index = 0; row_index < MATRIX_ROWS; row_index++) {
        // Check row pin state
        if (readPin(row_pins[row_index]) != 0) {
            // Pin LO, set col bit
            current_matrix[row_index] |= row_shifter;
        } else {
            // Pin HI, clear col bit
            current_matrix[row_index] &= ~row_shifter;
        }
    }

    // No need to Unselect row as the next `select_col` will blank everything
}

void matrix_init_custom(void) {
    _shift_init();
    _shift_enable();
    matrix_init_pins();
}

bool matrix_scan_custom(matrix_row_t curr_matrix[]) {
    matrix_row_t temp_matrix[MATRIX_ROWS] = {0};

    // Set col, read rows
    matrix_row_t row_shifter = MATRIX_ROW_SHIFTER;
    for (uint8_t current_col = 0; current_col < MATRIX_COLS; current_col++, row_shifter <<= 1) {
        matrix_read_rows_on_col(temp_matrix, current_col, row_shifter);
    }

    bool changed = memcmp(curr_matrix, temp_matrix, sizeof(temp_matrix)) != 0;
    if (changed) memcpy(curr_matrix, temp_matrix, sizeof(temp_matrix));

    return changed;
}
