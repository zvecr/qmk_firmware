// Copyright 2023 Massdrop, Inc.
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>

/*
 * Replace the regular I2C_BUS_ID constant
 * to a magic index we can dynamically update
 */

extern uint8_t g_i2c_bus_id;

#define I2C_BUS_ID (I2C_DEV(g_i2c_bus_id))

void i2c_claim_bus(uint8_t id);
