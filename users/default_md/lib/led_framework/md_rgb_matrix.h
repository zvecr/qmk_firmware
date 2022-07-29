/*
Copyright 2018 Massdrop Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "rgb_matrix.h"

// TODO: remove bodges
#define TOGGLE_FLAG_AND_PRINT(v, d) v = !v
#define rgb_matrix_set_enabled(s) (s ? rgb_matrix_enable() : rgb_matrix_disable())


//*******************************************************************************************************************
// Usually in <keyboard>/config_led.h but here are some defaults instead
// #ifndef LED_GCR_MAX
// #    define LED_GCR_MAX 165  // Max GCR value (0 - 255) WARNING: Raising this value may overload the LED drivers and USB bus
// #endif
// #ifndef LED_GCR_STEP
// #    define LED_GCR_STEP 10  // GCR increment/decrement value
// #endif

#ifndef ANIMATION_SPEED_STEP
#    define ANIMATION_SPEED_STEP 1
#endif

#ifndef BREATHE_STEP
#    define BREATHE_STEP 1
#endif
#ifndef BREATHE_MIN_STEP
#    define BREATHE_MIN_STEP 0
#endif
#ifndef BREATHE_MAX_STEP
#    define BREATHE_MAX_STEP 255
#endif

//*******************************************************************************************************************

// extern uint8_t gcr_breathe;
// extern uint8_t gcr_actual;
// extern uint8_t gcr_actual_last;

// void gcr_compute(void);

// void md_rgb_matrix_indicators_advanced(uint8_t led_min, uint8_t led_max);

#define EF_NONE 0x00000000      // No effect
#define EF_OVER 0x00000001      // Overwrite any previous color information with new
#define EF_SCR_L 0x00000002     // Scroll left
#define EF_SCR_R 0x00000004     // Scroll right
#define EF_SUBTRACT 0x00000008  // Subtract color values

typedef struct led_setup_s {
    float    hs;   // Band begin at percent
    float    he;   // Band end at percent
    uint8_t  rs;   // Red start value
    uint8_t  re;   // Red end value
    uint8_t  gs;   // Green start value
    uint8_t  ge;   // Green end value
    uint8_t  bs;   // Blue start value
    uint8_t  be;   // Blue end value
    uint32_t ef;   // Animation and color effects
    uint8_t  end;  // Set to signal end of the setup
} led_setup_t;

extern const uint8_t led_setups_count;
extern void*         led_setups[];

// LED Extra Instructions
#define LED_FLAG_NULL 0x00                // Matching and coloring not used (default)
#define LED_FLAG_MATCH_ID 0x01            // Match on the ID of the LED (set id#'s to desired bit pattern, first LED is id 1)
#define LED_FLAG_MATCH_LAYER 0x02         // Match on the current active layer (set layer to desired match layer)
#define LED_FLAG_USE_RGB 0x10             // Use a specific RGB value (set r, g, b to desired output color values)
#define LED_FLAG_USE_PATTERN 0x20         // Use a specific pattern ID (set pattern_id to desired output pattern)
#define LED_FLAG_USE_ROTATE_PATTERN 0x40  // Use pattern the user has cycled to manually

typedef struct led_instruction_s {
    uint16_t flags;  // Bitfield for LED instructions
    uint32_t id0;    // Bitwise id, IDs 0-31
    uint32_t id1;    // Bitwise id, IDs 32-63
    uint32_t id2;    // Bitwise id, IDs 64-95
    uint32_t id3;    // Bitwise id, IDs 96-127
    uint32_t id4;    // Bitwise id, IDs 128-159
    uint32_t id5;    // Bitwise id, IDs 160-191
    uint8_t  layer;
    uint8_t  r;
    uint8_t  g;
    uint8_t  b;
    uint8_t  pattern_id;
    uint8_t  end;
} led_instruction_t;

extern led_instruction_t led_instructions[];

//*******************************************************************************************************************

#define LED_MODE_NORMAL 0  // Must be 0
#define LED_MODE_KEYS_ONLY 1
#define LED_MODE_NON_KEYS_ONLY 2
#define LED_MODE_INDICATORS_ONLY 3
#define LED_MODE_MAX_INDEX LED_MODE_INDICATORS_ONLY  // Must be highest value

#define LED_EDGE_MODE_ALL 0                        // All edge LEDs are active (Must be 0)
#define LED_EDGE_MODE_ALTERNATE 1                  // Alternate mode of edge LEDs are active (Intention is for 'only every other edge LED' to be active)
#define LED_EDGE_MODE_MAX LED_EDGE_MODE_ALTERNATE  // Must be the highest valued LED edge mode

//*******************************************************************************************************************

typedef struct led_config_s {
    uint8_t desired_gcr;
    uint8_t animation_breathing;
    uint8_t animation_id;
    float   animation_speed;
    uint8_t lighting_mode;
    uint8_t enabled;
    uint8_t animation_breathe_cur;
    uint8_t animation_direction;
    uint8_t animation_breathe_dir;
    uint8_t animation_orientation;
    uint8_t animation_circular;
    float   edge_brightness;
    float   ratio_brightness;
    uint8_t edge_mode;
} md_led_config_t;

extern md_led_config_t md_led_config;

void md_led_changed(void);

#define gcr_desired md_led_config.desired_gcr
#define led_animation_breathing md_led_config.animation_breathing
#define led_animation_id md_led_config.animation_id
#define led_animation_speed md_led_config.animation_speed
#define led_lighting_mode md_led_config.lighting_mode
#define led_enabled md_led_config.enabled
#define led_animation_breathe_cur md_led_config.animation_breathe_cur
#define led_animation_direction md_led_config.animation_direction
#define breathe_dir md_led_config.animation_breathe_dir
#define led_animation_orientation md_led_config.animation_orientation
#define led_animation_circular md_led_config.animation_circular
#define led_edge_brightness md_led_config.edge_brightness
#define led_ratio_brightness md_led_config.ratio_brightness
#define led_edge_mode md_led_config.edge_mode
