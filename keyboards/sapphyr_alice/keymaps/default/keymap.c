#include QMK_KEYBOARD_H

enum my_layers {
    _QWERTY,
    _FNMS,
    _NAVMED
};

#define FNMS MO(_FNMS)
#define SPC_NAV LT(_NAVMED, KC_SPC)
#define CAP_CTL CTL_T(KC_CAPS)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT(
    KC_GESC, KC_GRV,  KC_1,  KC_2,    KC_3,   KC_4,    KC_5,  KC_6,  KC_7,   KC_8,  KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_DEL,
    KC_PGUP, KC_TAB,  KC_Q,  KC_W,    KC_E,   KC_R,    KC_T,         KC_Y,   KC_U,  KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSPC,
    KC_PGDN, CAP_CTL, KC_A,  KC_S,    KC_D,   KC_F,    KC_G,         KC_H,   KC_J,  KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
             KC_LSFT, KC_Z,  KC_X,    KC_C,   KC_V,    KC_B,         KC_B,   KC_N,  KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, FNMS,
             KC_LCTL,              KC_LALT,    KC_LCMD, FNMS,         SPC_NAV,       KC_RALT,                              KC_RCTL
  ),

  [_FNMS] = LAYOUT(
    RGB_TOG, _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______,
    RGB_MOD, _______, _______, _______, _______, _______, _______,        _______, _______, KC_MS_U, _______, _______, _______, _______, RESET,
    VLK_TOG, _______, _______, _______, _______, _______, _______,        KC_BTN1, KC_MS_L, KC_MS_D, KC_MS_R, KC_BTN2, _______,         _______,
             _______, BL_INC,   BL_DEC, BL_TOGG, BL_BRTG, _______,        RGB_SAI, RGB_HUI, RGB_VAI, RGB_SAD, RGB_HUD, RGB_VAD, _______, _______,
             AG_TOGG,                   _______,  _______, _______,        _______,          _______,                        _______
  ),

  [_NAVMED] = LAYOUT(
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______,        _______, _______,   KC_UP, _______, _______, _______, _______, RESET,
    _______, _______, _______, _______, _______, _______, KC_MPLY,        _______, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______,         EEP_RST,
             _______, _______, _______, _______, _______, KC_VOLD,        KC_VOLU, _______, _______, _______, _______, _______, _______, _______,
             _______,                   _______,  _______, _______,        _______,          _______,                        _______
  )
};
