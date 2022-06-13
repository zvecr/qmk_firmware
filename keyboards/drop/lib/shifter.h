// Copyright 2022 zvecr <git@zvecr.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>

void shift_init(void);

void shift_enable(void);

void shift_disable(void);

void shift_out(const uint8_t *data, uint16_t length);

static inline void shift_out_u16(uint16_t data) {
    uint8_t tmp[2] = {
        data & 0xFF,         // Shift in bits 7-0
        (data >> 8) & 0xFF,  // Shift in bits 15-8
    };
    shift_out(tmp, 2);
}
