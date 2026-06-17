// Copyright 2026 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>
#include "steno_keycodes.h"

#define BOLT_STROKE_SIZE 4
#define GEMINI_STROKE_SIZE 6
#define PLOVER_HID_STROKE_SIZE 8

#if defined(STENO_ENABLE_PLOVER_HID)
#    define MAX_STROKE_SIZE PLOVER_HID_STROKE_SIZE
#elif defined(STENO_ENABLE_GEMINI)
#    define MAX_STROKE_SIZE GEMINI_STROKE_SIZE
#else
#    define MAX_STROKE_SIZE BOLT_STROKE_SIZE
#endif

typedef enum {
    STENO_MODE_GEMINI,
    STENO_MODE_BOLT,
    STENO_MODE_PLOVER_HID,
} steno_mode_t;

void steno_init(void);

void steno_task(void);

void eeconfig_update_steno_default(void);

steno_mode_t steno_get_mode(void);

#if NUM_STENO_PROTOCOLS > 1

void steno_set_mode_noeeprom(steno_mode_t mode);

void steno_set_mode(steno_mode_t mode);

void steno_mode_next_noeeprom(void);

void steno_mode_next(void);

void steno_mode_previous_noeeprom(void);

void steno_mode_previous(void);

#endif

extern uint8_t chord[MAX_STROKE_SIZE];

void steno_clear_chord(void);
