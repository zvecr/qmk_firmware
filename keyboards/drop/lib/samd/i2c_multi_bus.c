// Copyright 2023 Massdrop, Inc.
// SPDX-License-Identifier: GPL-2.0-or-later

#include "i2c_multi_bus.h"

uint8_t g_i2c_bus_id = 1;

void i2c_claim_bus(uint8_t id) { g_i2c_bus_id = id; }
