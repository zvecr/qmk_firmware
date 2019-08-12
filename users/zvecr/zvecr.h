#pragma once

#include <quantum.h>

// Layer Declarations
enum {
    _QWERTY = 0,
    _LOWER,
    _RAISE,
    _META,
    _ADJUST,
};

// Layer utils
#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define META MO(_META)
#define ADJUST MO(_ADJUST)
#define RAI_SPC LT(_RAISE, KC_SPC)
#define LOW_SPC LT(_LOWER, KC_SPC)
#define LOW_BSPC LT(_LOWER, KC_BSPC)
#define LOW_ESC LT(_LOWER, KC_ESC)
#define ADJ_MENU LT(_ADJUST, KC_APP)

// Bodge keycodes
#define KC_CAD LALT(LCTL(KC_DEL))
#define KC_SNIP LWIN(LSFT(KC_S))
#define KC_ISOT S(KC_NUHS)
#define KC_ISOP S(KC_NUBS)

// Macro Declarations
enum custom_keycodes {
    MACRO_1 = SAFE_RANGE,
    MACRO_2,
    MACRO_3,
    MACRO_4,
    MACRO_5,
};
