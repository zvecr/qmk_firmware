// Copyright 2024 Nick Brassel (@tzarc)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <stdint.h>
#include <stddef.h> // offsetof

#include "compiler_support.h"
#include "eeconfig.h"
#include "util.h"

// Dummy struct only used to calculate offsets
typedef struct PACKED {
    uint16_t magic;
    uint8_t  debug;
    uint8_t  default_layer;
    uint16_t keymap;
    uint32_t keyboard;
    uint32_t user;
    uint8_t  handedness;
#ifdef BACKLIGHT_ENABLE
    uint8_t  backlight;
#endif
#ifdef AUDIO_ENABLE
    uint8_t  audio;
#endif
#ifdef RGBLIGHT_ENABLE
    uint32_t rgblight;
    uint8_t  rgblight_ext;
#endif
#ifdef UNICODE_COMMON_ENABLE
    uint8_t  unicode;
#endif
#ifdef STENO_ENABLE
    uint8_t  steno;
#endif
#ifdef RGB_MATRIX_ENABLE
    uint64_t rgb_matrix;
#endif
#ifdef LED_MATRIX_ENABLE
        uint32_t led_matrix;
#endif
#ifdef HAPTIC_ENABLE
    uint32_t haptic;
#endif
#ifdef CONNECTION_ENABLE
    uint8_t  connection;
#endif
} eeprom_core_t;

/* EEPROM parameter address */
#define EECONFIG_MAGIC (uint16_t *)(offsetof(eeprom_core_t, magic))
#define EECONFIG_DEBUG (uint8_t *)(offsetof(eeprom_core_t, debug))
#define EECONFIG_DEFAULT_LAYER (uint8_t *)(offsetof(eeprom_core_t, default_layer))
#define EECONFIG_KEYMAP (uint16_t *)(offsetof(eeprom_core_t, keymap))
#define EECONFIG_KEYBOARD (uint32_t *)(offsetof(eeprom_core_t, keyboard))
#define EECONFIG_USER (uint32_t *)(offsetof(eeprom_core_t, user))
#define EECONFIG_HANDEDNESS (uint8_t *)(offsetof(eeprom_core_t, handedness))
#define EECONFIG_BACKLIGHT (uint8_t *)(offsetof(eeprom_core_t, backlight))
#define EECONFIG_AUDIO (uint8_t *)(offsetof(eeprom_core_t, audio))
#define EECONFIG_RGBLIGHT (uint32_t *)(offsetof(eeprom_core_t, rgblight))
#define EECONFIG_RGBLIGHT_EXTENDED (uint8_t *)(offsetof(eeprom_core_t, rgblight_ext))
#define EECONFIG_UNICODEMODE (uint8_t *)(offsetof(eeprom_core_t, unicode))
#define EECONFIG_STENOMODE (uint8_t *)(offsetof(eeprom_core_t, steno))
#define EECONFIG_RGB_MATRIX (uint64_t *)(offsetof(eeprom_core_t, rgb_matrix))
#define EECONFIG_LED_MATRIX (uint32_t *)(offsetof(eeprom_core_t, led_matrix))
#define EECONFIG_HAPTIC (uint32_t *)(offsetof(eeprom_core_t, haptic))
#define EECONFIG_CONNECTION (uint8_t *)(offsetof(eeprom_core_t, connection))

// Size of EEPROM being used for core data storage
#define EECONFIG_BASE_SIZE ((uint8_t)sizeof(eeprom_core_t))

#define EECONFIG_KB_DATABLOCK ((uint8_t *)(EECONFIG_BASE_SIZE))
#define EECONFIG_USER_DATABLOCK ((uint8_t *)((EECONFIG_BASE_SIZE) + (EECONFIG_KB_DATA_SIZE)))
#define EECONFIG_MODULES_DATABLOCK ((uint8_t *)((EECONFIG_BASE_SIZE) + (EECONFIG_KB_DATA_SIZE) + (EECONFIG_USER_DATA_SIZE)))

// Actual value is injected via the generated community_post_config.h
#ifndef EECONFIG_MODULE_DATA_SIZE
#    define EECONFIG_MODULE_DATA_SIZE 0
#endif

// Size of EEPROM being used, other code can refer to this for available EEPROM
#define EECONFIG_SIZE ((EECONFIG_BASE_SIZE) + (EECONFIG_KB_DATA_SIZE) + (EECONFIG_USER_DATA_SIZE) + (EECONFIG_MODULE_DATA_SIZE))

STATIC_ASSERT((intptr_t)EECONFIG_HANDEDNESS == 14, "EEPROM handedness offset is incorrect");
