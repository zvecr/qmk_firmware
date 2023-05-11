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

#include <math.h>
#include "md_rgb_matrix.h"
#include "eeconfig.h"
#include "eeprom.h"

#define FLUSH_TIMEOUT 5000
#define EECONFIG_MD_LED ((uint8_t*)(EECONFIG_SIZE + 64))
#define MD_LED_CONFIG_VERSION 3

// TODO?: wire these up to keymap.c
md_led_config_t md_led_config = {0};

EECONFIG_DEBOUNCE_HELPER(md_led, EECONFIG_MD_LED, md_led_config);

void eeconfig_update_md_led_default(void) {
    uint32_t version = MD_LED_CONFIG_VERSION;

    // gcr_desired               = LED_GCR_MAX;
    led_animation_orientation = 0;
    led_animation_direction   = 0;
    led_animation_breathing   = 0;
    led_animation_id          = 0;
    led_animation_speed       = 4.0f;
    led_lighting_mode         = LED_MODE_NORMAL;
    led_enabled               = 1;
    led_animation_breathe_cur = BREATHE_MIN_STEP;
    breathe_dir               = 1;
    led_animation_circular    = 0;
    led_edge_brightness       = 1.0f;
    led_ratio_brightness      = 1.0f;
    led_edge_mode             = LED_EDGE_MODE_ALL;

    eeconfig_flush_md_led(true);
    eeconfig_update_kb(version);
}

void md_led_changed(void) { eeconfig_flag_md_led(true); }

// TODO: use real task rather than this bodge
void housekeeping_task_kb(void) { eeconfig_flush_md_led_task(FLUSH_TIMEOUT); }

// uint8_t gcr_breathe;
float breathe_mult = 1;
float pomod        = 0;

void keyboard_post_init_kb(void) {
    if (!eeconfig_is_enabled()) {
        eeconfig_init();
    }

    uint32_t version = eeconfig_read_kb();
    eeconfig_init_md_led();

    if (version != MD_LED_CONFIG_VERSION) {
        eeconfig_update_md_led_default();
        eeconfig_update_rgb_matrix_default();
    }

    keyboard_post_init_user();
}

void eeconfig_init_kb(void) {
    static md_led_config_t blank_led_config = {0};

    eeconfig_update_kb(0);
    eeconfig_write_md_led(&blank_led_config);

    eeconfig_init_user();
}

void md_rgb_matrix_init(void) {}

void md_rgb_matrix_effect_start(void) {
    breathe_mult = 1;

    if (led_animation_breathing) {
        //+60us 119 LED
        led_animation_breathe_cur += BREATHE_STEP * breathe_dir;

        if (led_animation_breathe_cur >= BREATHE_MAX_STEP)
            breathe_dir = -1;
        else if (led_animation_breathe_cur <= BREATHE_MIN_STEP)
            breathe_dir = 1;

        // Brightness curve created for 256 steps, 0 - ~98%
        breathe_mult = 0.000015 * led_animation_breathe_cur * led_animation_breathe_cur;
        if (breathe_mult > 1)
            breathe_mult = 1;
        else if (breathe_mult < 0)
            breathe_mult = 0;
    }

    // This should only be performed once per frame
    pomod = (float)((g_rgb_timer / 10) % (uint32_t)(1000.0f / led_animation_speed)) / 10.0f * led_animation_speed;
    pomod *= 100.0f;
    pomod = (uint32_t)pomod % 10000;
    pomod /= 100.0f;

    // mimic behaviour of gcr_compute
    void push_gcr(uint8_t value);
    push_gcr(rgb_matrix_config.hsv.v);
}

void push_gcr(uint8_t value) {
    // ignore if the value has already been set
    static uint8_t last = 0xFF;
    if(last == value) {
        return;
    }
    last = value;

#define ISSI_COMMANDREGISTER 0xFD
#define ISSI_COMMANDREGISTER_WRITELOCK 0xFE
#define ISSI_INTERRUPTMASKREGISTER 0xF0
#define ISSI_INTERRUPTSTATUSREGISTER 0xF1

#define ISSI_PAGE_LEDCONTROL 0x00  // PG0
#define ISSI_PAGE_PWM 0x01         // PG1
#define ISSI_PAGE_AUTOBREATH 0x02  // PG2
#define ISSI_PAGE_FUNCTION 0x03    // PG3

#define ISSI_REG_CONFIGURATION 0x00  // PG3
#define ISSI_REG_GLOBALCURRENT 0x01  // PG3
#define ISSI_REG_RESET 0x11          // PG3
#define ISSI_REG_SWPULLUP 0x0F       // PG3
#define ISSI_REG_CSPULLUP 0x10       // PG3

#ifdef DRIVER_ADDR_1
    IS31FL3733_write_register(DRIVER_ADDR_1, ISSI_COMMANDREGISTER_WRITELOCK, 0xC5);
    IS31FL3733_write_register(DRIVER_ADDR_1, ISSI_COMMANDREGISTER, ISSI_PAGE_FUNCTION);
    IS31FL3733_write_register(DRIVER_ADDR_1, ISSI_REG_GLOBALCURRENT, rgb_matrix_config.hsv.v);
#endif

#ifdef DRIVER_ADDR_2
    IS31FL3733_write_register(DRIVER_ADDR_2, ISSI_COMMANDREGISTER_WRITELOCK, 0xC5);
    IS31FL3733_write_register(DRIVER_ADDR_2, ISSI_COMMANDREGISTER, ISSI_PAGE_FUNCTION);
    IS31FL3733_write_register(DRIVER_ADDR_2, ISSI_REG_GLOBALCURRENT, rgb_matrix_config.hsv.v);
#endif

#ifdef DRIVER_ADDR_3
    IS31FL3733_write_register(DRIVER_ADDR_3, ISSI_COMMANDREGISTER_WRITELOCK, 0xC5);
    IS31FL3733_write_register(DRIVER_ADDR_3, ISSI_COMMANDREGISTER, ISSI_PAGE_FUNCTION);
    IS31FL3733_write_register(DRIVER_ADDR_3, ISSI_REG_GLOBALCURRENT, rgb_matrix_config.hsv.v);
#endif
}

static void led_run_pattern(led_setup_t* f, float* ro, float* go, float* bo, float pos) {
    float po;

    while (f->end != 1) {
        po = pos;  // Reset po for new frame

        // Add in any moving effects
        if ((!led_animation_direction && f->ef & EF_SCR_R) || (led_animation_direction && (f->ef & EF_SCR_L))) {
            po -= pomod;

            if (po > 100)
                po -= 100;
            else if (po < 0)
                po += 100;
        } else if ((!led_animation_direction && f->ef & EF_SCR_L) || (led_animation_direction && (f->ef & EF_SCR_R))) {
            po += pomod;

            if (po > 100)
                po -= 100;
            else if (po < 0)
                po += 100;
        }

        // Check if LED's po is in current frame
        if (po < f->hs) {
            f++;
            continue;
        }
        if (po > f->he) {
            f++;
            continue;
        }
        // note: < 0 or > 100 continue

        // Calculate the po within the start-stop percentage for color blending
        po = (po - f->hs) / (f->he - f->hs);

        // Add in any color effects
        if (f->ef & EF_OVER) {
            *ro = (po * (f->re - f->rs)) + f->rs;  // + 0.5;
            *go = (po * (f->ge - f->gs)) + f->gs;  // + 0.5;
            *bo = (po * (f->be - f->bs)) + f->bs;  // + 0.5;
        } else if (f->ef & EF_SUBTRACT) {
            *ro -= (po * (f->re - f->rs)) + f->rs;  // + 0.5;
            *go -= (po * (f->ge - f->gs)) + f->gs;  // + 0.5;
            *bo -= (po * (f->be - f->bs)) + f->bs;  // + 0.5;
        } else {
            *ro += (po * (f->re - f->rs)) + f->rs;  // + 0.5;
            *go += (po * (f->ge - f->gs)) + f->gs;  // + 0.5;
            *bo += (po * (f->be - f->bs)) + f->bs;  // + 0.5;
        }

        f++;
    }
}

#define RGB_MAX_DISTANCE 232.9635f

void md_rgb_matrix_effect_render(uint8_t i) {
    float ro = 0;
    float go = 0;
    float bo = 0;
    float po;

    uint8_t highest_active_layer = get_highest_layer(layer_state);

    if (led_animation_circular) {
        // TODO: should use min/max values from LED configuration instead of
        // hard-coded 224, 64
        // po = sqrtf((powf(fabsf((disp.width / 2) - (led_cur->x - disp.left)), 2) + powf(fabsf((disp.height / 2) - (led_cur->y - disp.bottom)), 2))) / disp.max_distance * 100;
        po = sqrtf((powf(fabsf((224 / 2) - (float)g_led_config.point[i].x), 2) + powf(fabsf((64 / 2) - (float)g_led_config.point[i].y), 2))) / RGB_MAX_DISTANCE * 100;
    } else {
        if (led_animation_orientation) {
            po = (float)g_led_config.point[i].y / 64.f * 100;
        } else {
            po = (float)g_led_config.point[i].x / 224.f * 100;
        }
    }

    if (led_edge_mode == LED_EDGE_MODE_ALTERNATE && HAS_FLAGS(g_led_config.flags[i], 0x80)) {
        // Do not act on this LED (Edge alternate lighting mode)
    } else if (led_lighting_mode == LED_MODE_KEYS_ONLY && HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
        // Do not act on this LED
    } else if (led_lighting_mode == LED_MODE_NON_KEYS_ONLY && !HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
        // Do not act on this LED
    } else if (led_lighting_mode == LED_MODE_INDICATORS_ONLY) {
        // Do not act on this LED (Only show indicators)
    } else {
        led_instruction_t* led_cur_instruction = led_instructions;
        while (!led_cur_instruction->end) {
            // Check if this applies to current layer
            if ((led_cur_instruction->flags & LED_FLAG_MATCH_LAYER) && (led_cur_instruction->layer != highest_active_layer)) {
                goto next_iter;
            }

            // Check if this applies to current index
            if (led_cur_instruction->flags & LED_FLAG_MATCH_ID) {
                uint8_t   modid    = i / 32;                             // Calculate which id# contains the led bit
                uint32_t  modidbit = 1 << (i % 32);                      // Calculate the bit within the id#
                uint32_t* bitfield = &led_cur_instruction->id0 + modid;  // Add modid as offset to id0 address. *bitfield is now idX of the led id
                if (~(*bitfield) & modidbit) {                           // Check if led bit is not set in idX
                    goto next_iter;
                }
            }

            if (led_cur_instruction->flags & LED_FLAG_USE_RGB) {
                ro = led_cur_instruction->r;
                go = led_cur_instruction->g;
                bo = led_cur_instruction->b;
            } else if (led_cur_instruction->flags & LED_FLAG_USE_PATTERN) {
                led_run_pattern(led_setups[led_cur_instruction->pattern_id], &ro, &go, &bo, po);
            } else if (led_cur_instruction->flags & LED_FLAG_USE_ROTATE_PATTERN) {
                led_run_pattern(led_setups[led_animation_id], &ro, &go, &bo, po);
            }

        next_iter:
            led_cur_instruction++;
        }

        if (ro > 255)
            ro = 255;
        else if (ro < 0)
            ro = 0;
        if (go > 255)
            go = 255;
        else if (go < 0)
            go = 0;
        if (bo > 255)
            bo = 255;
        else if (bo < 0)
            bo = 0;

        if (led_animation_breathing) {
            ro *= breathe_mult;
            go *= breathe_mult;
            bo *= breathe_mult;
        }
    }

    // Adjust edge LED brightness
    if (led_edge_brightness != 1 && HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
        ro *= led_edge_brightness;
        go *= led_edge_brightness;
        bo *= led_edge_brightness;
    }

    // Adjust ratio of key vs. underglow (edge) LED brightness
    if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW) && led_ratio_brightness > 1.0) {
        // Decrease edge (underglow) LEDs
        ro *= (2.0 - led_ratio_brightness);
        go *= (2.0 - led_ratio_brightness);
        bo *= (2.0 - led_ratio_brightness);
    } else if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_KEYLIGHT) && led_ratio_brightness < 1.0) {
        // Decrease KEY LEDs
        ro *= led_ratio_brightness;
        go *= led_ratio_brightness;
        bo *= led_ratio_brightness;
    }

    rgb_matrix_set_color(i, (uint8_t)ro, (uint8_t)go, (uint8_t)bo);
}
