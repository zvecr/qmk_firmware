// Copyright 2022 Massdrop, Inc.
// SPDX-License-Identifier: GPL-2.0-or-later
#include "rgb_matrix.h"
#include "eeprom.h"

#ifdef RGB_MATRIX_ENABLE
const is31_led PROGMEM g_is31_leds[RGB_MATRIX_LED_COUNT] = {
// top underglow sd2-sd17
    { 0, B_2,  A_2,  C_2  },
    { 0, B_3,  A_3,  C_3  },
    { 0, B_5,  A_5,  C_5  },
    { 0, B_6,  A_6,  C_6  },
    { 0, B_7,  A_7,  C_7  },
    { 0, B_9,  A_9,  C_9  },
    { 0, B_10, A_10, C_10 },
    { 0, B_12, A_12, C_12 },
    { 0, B_13, A_13, C_13 },
    { 0, B_14, A_14, C_14 },
    { 0, B_16, A_16, C_16 },
    { 1, B_12, A_12, C_12 },

// sd1 + function + sd18
    { 0, B_1,  A_1,  C_1  },
    { 0, E_2,  D_2,  F_2  },
    { 0, E_3,  D_3,  F_3  },
    { 0, E_4,  D_4,  F_4  },
    { 0, E_5,  D_5,  F_5  },
    { 0, E_6,  D_6,  F_6  },
    { 0, E_7,  D_7,  F_7  },
    { 0, E_8,  D_8,  F_8  },
    { 0, E_9,  D_9,  F_9  },
    { 0, E_10, D_10, F_10 },
    { 0, E_11, D_11, F_11 },
    { 0, E_12, D_12, F_12 },
    { 0, E_13, D_13, F_13 },
    { 0, E_14, D_14, F_14 },
    { 0, E_15, D_15, F_15 },
    // <enc gap>
    { 1, B_13, A_13, C_13 },

// sd45 + num + sd20
    { 0, H_1,  G_1,  I_1  },
    { 0, H_2,  G_2,  I_2  },
    { 0, H_3,  G_3,  I_3  },
    { 0, H_4,  G_4,  I_4  },
    { 0, H_5,  G_5,  I_5  },
    { 0, H_6,  G_6,  I_6  },
    { 0, H_7,  G_7,  I_7  },
    { 0, H_8,  G_8,  I_8  },
    { 0, H_9,  G_9,  I_9  },
    { 0, H_10, G_10, I_10 },
    { 0, H_11, G_11, I_11 },
    { 0, H_12, G_12, I_12 },
    { 0, H_13, G_13, I_13 },
    { 0, H_14, G_14, I_14 },
    { 0, H_15, G_15, I_15 },
    { 0, H_16, G_16, I_16 },
    { 1, B_15, A_15, C_15 },

// 44+ qwer 21
    { 0, K_1,  J_1,  L_1  },
    { 0, K_2,  J_2,  L_2  },
    { 0, K_3,  J_3,  L_3  },
    { 0, K_4,  J_4,  L_4  },
    { 0, K_5,  J_5,  L_5  },
    { 0, K_6,  J_6,  L_6  },
    { 0, K_7,  J_7,  L_7  },
    { 0, K_8,  J_8,  L_8  },
    { 0, K_9,  J_9,  L_9  },
    { 0, K_10, J_10, L_10 },
    { 0, K_11, J_11, L_11 },
    { 0, K_12, J_12, L_12 },
    { 0, K_13, J_13, L_13 },
    { 0, K_14, J_14, L_14 },
    { 0, K_15, J_15, L_15 },
    { 0, K_16, J_16, L_16 },
    { 1, B_16, A_16, C_16 },

// asdf
    { 1, B_2,  A_2,  C_2  },
    { 1, B_3,  A_3,  C_3  },
    { 1, B_4,  A_4,  C_4  },
    { 1, B_5,  A_5,  C_5  },
    { 1, B_6,  A_6,  C_6  },
    { 1, B_7,  A_7,  C_7  },
    { 1, B_8,  A_8,  C_8  },
    { 1, B_9,  A_9,  C_9  },
    { 1, B_10, A_10, C_10 },
    { 1, B_11, A_11, C_11 },
    { 1, E_12, D_12, F_12 },
    { 1, E_13, D_13, F_13 },
    { 1, E_14, D_14, F_14 },
    { 1, H_15, G_15, I_15 },

// 43 + zxcv + 22
    { 1, B_1,  A_1,  C_1  },
    { 1, E_2,  D_2,  F_2  },
    { 1, E_3,  D_3,  F_3  },
    { 1, E_4,  D_4,  F_4  },
    { 1, E_5,  D_5,  F_5  },
    { 1, E_6,  D_6,  F_6  },
    { 1, E_7,  D_7,  F_7  },
    { 1, E_8,  D_8,  F_8  },
    { 1, E_9,  D_9,  F_9  },
    { 1, E_10, D_10, F_10 },
    { 1, E_11, D_11, F_11 },
    { 1, H_12, G_12, I_12 },
    { 1, H_13, G_13, I_13 },
    { 1, H_14, G_14, I_14 },
    { 1, E_15, D_15, F_15 },

// 41 + mods + 23
    { 1, H_1,  G_1,  I_1  },
    { 1, H_2,  G_2,  I_2  },
    { 1, H_3,  G_3,  I_3  },
    { 1, H_4,  G_4,  I_4  },
    { 1, H_5,  G_5,  I_5  },
    { 1, H_6,  G_6,  I_6  },
    { 1, H_7,  G_7,  I_7  },
    { 1, H_8,  G_8,  I_8  },
    { 1, H_9,  G_9,  I_9  },
    { 1, H_10, G_10, I_10 },
    { 1, H_11, G_11, I_11 },
    { 1, E_16, D_16, F_16 },

// bottom underglow 24 - 39
    { 1, K_2,  J_2,  L_2  },
    { 1, K_3,  J_3,  L_3  },
    { 1, K_4,  J_4,  L_4  },
    { 1, K_6,  J_6,  L_6  },
    { 1, K_10, J_10, L_10 },
    { 1, K_13, J_13, L_13 },
    { 1, K_15, J_15, L_15 },
    { 1, H_16, G_16, I_16 }
};
#endif

#ifdef ENCODER_ENABLE
bool encoder_update_kb(uint8_t index, bool clockwise) {
    if (!encoder_update_user(index, clockwise)) return false;

    if (clockwise) {
        tap_code(KC_VOLU);
    } else {
        tap_code(KC_VOLD);
    }
    return false; 
}
#endif

#ifdef XAP_ENABLE

#define EECONFIG_KB_LED ((uint8_t*)(EECONFIG_SIZE + 64))
#define KB_LED_CONFIG_VERSION 1

#define INVALID_EFFECT 0xFF

uint8_t rgb_matrix2xap(uint8_t val);
uint8_t xap2rgb_matrix(uint8_t val);

rgb_config_t rgb_layers[DYNAMIC_KEYMAP_LAYER_COUNT] = {0};

EECONFIG_DEBOUNCE_HELPER(kb_led, EECONFIG_KB_LED, rgb_layers);

void eeconfig_update_kb_led_default(void) {
    uint32_t version = KB_LED_CONFIG_VERSION;

    // defaults?
    rgb_layers[1].enable = 1;
    rgb_layers[1].mode = RGB_MATRIX_SOLID_COLOR;
    rgb_layers[1].hsv = (HSV){HSV_BLUE};


    eeconfig_flush_kb_led(true);
    eeconfig_update_kb(version);
}

void keyboard_post_init_kb(void) {
    if (!eeconfig_is_enabled()) {
        eeconfig_init();
    }

    uint32_t version = eeconfig_read_kb();
    eeconfig_init_kb_led();

    if (version != KB_LED_CONFIG_VERSION) {
        eeconfig_update_kb_led_default();
        eeconfig_update_rgb_matrix_default();
    }

    keyboard_post_init_user();
}

bool xap_respond_kb_get_rgb_layer(xap_token_t token, const void *data, size_t length) {
    if (length != sizeof(uint8_t)) {
        return false;
    }

    uint8_t layer;
    memcpy(&layer, data, sizeof(uint8_t));

    xap_route_kb_get_rgb_layer_t ret = {0};

    rgb_config_t* conf = &rgb_layers[layer];

    ret.enable = conf->enable;
    ret.mode   = rgb_matrix2xap(conf->mode);
    ret.hue    = conf->hsv.h;
    ret.sat    = conf->hsv.s;
    ret.val    = conf->hsv.v;

    return xap_respond_data(token, &ret, sizeof(ret));
}

bool xap_respond_kb_set_rgb_layer(xap_token_t token, const void *data, size_t length) {
    if (length != sizeof(xap_route_kb_set_rgb_layer_arg_t)) {
        return false;
    }

    xap_route_kb_set_rgb_layer_arg_t *arg = (xap_route_kb_set_rgb_layer_arg_t *)data;

    uint8_t mode = xap2rgb_matrix(arg->mode);
    if (mode == INVALID_EFFECT) {
        return false;
    }

    rgb_config_t* conf = &rgb_layers[arg->layer];

    conf->enable = arg->enable;
    conf->mode = mode;
    conf->hsv = (HSV){arg->hue, arg->sat, arg->val};

    xap_respond_success(token);
    return true;
}

bool xap_respond_kb_save_rgb_layers(xap_token_t token, const void *data, size_t length) {
    eeconfig_flush_kb_led(true);

    xap_respond_success(token);
    return true;
}

static uint8_t last_layer = 0;
static rgb_config_t rgb_matrix_backup;

void rgb_matrix_pre_task(void) {
    // backup config
    rgb_matrix_backup = rgb_matrix_config;

    uint8_t cur_layer = get_highest_layer(layer_state);

    rgb_config_t* conf = &rgb_layers[cur_layer];
    if(conf->enable) {
        rgb_matrix_config.hsv = conf->hsv;
        rgb_matrix_config.mode = conf->mode;
    }

    // first iteration of different effect
    //   - trigger "rgb_task_state = STARTING" to ensure correct effect init
    if(last_layer != cur_layer) {
        last_layer = cur_layer;

        void rgb_matrix_reset_task_state(void);
        rgb_matrix_reset_task_state();
    }
}

void rgb_matrix_post_task(void) {
    // restore config
    rgb_matrix_config = rgb_matrix_backup;
}
#endif
