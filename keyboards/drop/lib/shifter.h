// Copyright 2022 zvecr <git@zvecr.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>

void shift_init(void);

void shift_enable(void);

void shift_disable(void);

void shift_out(const uint8_t *data, uint16_t length);
