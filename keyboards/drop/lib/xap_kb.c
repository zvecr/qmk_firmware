#include "quantum.h"
#include "eeprom.h"

#if defined(XAP_ENABLE) && !defined(DISABLE_XAP_RGB_LAYERS)

#define INVALID_EFFECT 0xFF

uint8_t rgb_matrix2xap(uint8_t val);
uint8_t xap2rgb_matrix(uint8_t val);

rgb_config_t rgb_layers[DYNAMIC_KEYMAP_LAYER_COUNT] = {0};

EECONFIG_DEBOUNCE_HELPER(rgb_layers, EECONFIG_KB_DATABLOCK, rgb_layers);

void keyboard_post_init_kb(void) {
    if(!eeconfig_is_kb_datablock_valid()) {
        eeconfig_init_kb_datablock();
    }

    eeconfig_init_rgb_layers();

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
    eeconfig_flush_rgb_layers(true);

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