// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* Include the vendor specific pin defs */
#if __has_include_next("_pin_defs.h")
#    include_next "_pin_defs.h"
#endif

#define LINE_PIN0 GPIO_PIN(0, 0)
#define LINE_PIN1 GPIO_PIN(0, 1)
#define LINE_PIN2 GPIO_PIN(0, 2)
#define LINE_PIN3 GPIO_PIN(0, 3)
#define LINE_PIN4 GPIO_PIN(0, 4)
#define LINE_PIN5 GPIO_PIN(0, 5)
#define LINE_PIN6 GPIO_PIN(0, 6)
