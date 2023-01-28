// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>
#include "gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

int16_t analogReadPin(pin_t pin);

#ifdef __cplusplus
}
#endif
