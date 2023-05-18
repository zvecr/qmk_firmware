// Copyright 2023 Massdrop, Inc.
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"
#include "eeprom.h"

#if defined(XAP_ENABLE) && !defined(DISABLE_XAP_RGB_LAYERS)

#define INVALID_EFFECT 0xFF

uint8_t rgb_matrix_effect_to_id(uint8_t val);
uint8_t rgb_matrix_id_to_effect(uint8_t val);

rgb_config_t rgb_layers[DYNAMIC_KEYMAP_LAYER_COUNT] = {0};

EECONFIG_DEBOUNCE_HELPER(rgb_layers, EECONFIG_KB_DATABLOCK, rgb_layers);

void keyboard_post_init_kb(void) {
    if(!eeconfig_is_kb_datablock_valid()) {
        eeconfig_init_kb_datablock();
    }

    eeconfig_init_rgb_layers();

    keyboard_post_init_user();
}

bool xap_execute_kb_get_rgb_layer(xap_token_t token, uint8_t layer) {
    xap_route_kb_get_rgb_layer_t ret = {0};

    rgb_config_t* conf = &rgb_layers[layer];

    ret.enable = conf->enable;
    ret.mode   = rgb_matrix_effect_to_id(conf->mode);
    ret.hue    = conf->hsv.h;
    ret.sat    = conf->hsv.s;
    ret.val    = conf->hsv.v;

    return xap_respond_data(token, &ret, sizeof(ret));
}

bool xap_execute_kb_set_rgb_layer(xap_token_t token, xap_route_kb_set_rgb_layer_arg_t* arg) {
    uint8_t mode = rgb_matrix_id_to_effect(arg->mode);
    if (mode == INVALID_EFFECT) {
        return false;
    }

    rgb_config_t* conf = &rgb_layers[arg->layer];

    conf->enable = arg->enable;
    conf->mode = mode;
    conf->hsv = (HSV){arg->hue, arg->sat, arg->val};

    return xap_respond_success(token);
}

bool xap_execute_kb_save_rgb_layers(xap_token_t token, const void *data, size_t length) {
    eeconfig_flush_rgb_layers(true);

    return xap_respond_success(token);
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