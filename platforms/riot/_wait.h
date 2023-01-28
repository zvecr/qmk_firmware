// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>
#include "xtimer.h"
#include "clk.h"

#define wait_ms(ms) xtimer_msleep(ms)

#define wait_us(us)                                  \
    do {                                             \
        if (us > 30) {                               \
            xtimer_usleep(us);                       \
        } else {                                     \
            xtimer_spin(xtimer_ticks_from_usec(us)); \
        }                                            \
    } while (0)

#include "_wait.c"

/* For GPIOs on ARM-based MCUs, the input pins are sampled by the clock of the bus
 * to which the GPIO is connected.
 * The connected buses differ depending on the various series of MCUs.
 * And since the instruction execution clock of the CPU and the bus clock of GPIO are different,
 * there is a delay of several clocks to read the change of the input signal.
 *
 * Define this delay with the GPIO_INPUT_PIN_DELAY macro.
 * If the GPIO_INPUT_PIN_DELAY macro is not defined, the following default values will be used.
 * (A fairly large value of 0.25 microseconds is set.)
 */
#ifndef GPIO_INPUT_PIN_DELAY
#    define GPIO_INPUT_PIN_DELAY (coreclk() / 1000000L / 4)
#endif

#define waitInputPinDelay() wait_cpuclock(GPIO_INPUT_PIN_DELAY)
