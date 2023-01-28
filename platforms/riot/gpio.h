// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "periph/gpio.h"
#include "pin_defs.h"

typedef gpio_t pin_t;

#define setPinInput(pin) gpio_init(pin, GPIO_IN)
#define setPinInputHigh(pin) gpio_init(pin, GPIO_IN_PU)
#define setPinInputLow(pin) gpio_init(pin, GPIO_IN_PD)
#define setPinOutput(pin) gpio_init(pin, GPIO_OUT)
#define writePinHigh(pin) gpio_set(pin)
#define writePinLow(pin) gpio_clear(pin)
#define writePin(pin, level) gpio_write(pin, level)
#define readPin(pin) gpio_read(pin)
#define togglePin(pin) gpio_toggle(pin)
