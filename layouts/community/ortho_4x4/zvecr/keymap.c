#include QMK_KEYBOARD_H

// Layer Declarations
enum {
    _NUM = 0,
    _FUNC,
    _MACRO,
    _META,
};

// Macro Declarations
enum custom_keycodes {
    MACRO_01 = SAFE_RANGE,
    MACRO_02,
    MACRO_03,
    MACRO_04,
    MACRO_05,
    MACRO_06,
    MACRO_07,
    MACRO_08,
    MACRO_09,
    MACRO_10,
    MACRO_11,
    MACRO_12,
    MACRO_13,
    MACRO_14,
    MACRO_15,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
 /*
  * ,---------------------------.
  * |      |   /  |   *  |   -  |
  * |------+------+------+------|
  * |   7  |   8  |   9  |   +  |
  * |------+------+------+------|
  * |   4  |   5  |   6  |   .  |
  * |------+------+------+------|
  * |   1  |  2   |  3   |   0  |
  * `---------------------------'
   */
  [_NUM] = LAYOUT_ortho_4x4( \
    TO(1),    KC_PSLS,  KC_PAST,  KC_PMNS,  \
    KC_KP_7,  KC_KP_8,  KC_KP_9,  KC_PPLS,  \
    KC_KP_4,  KC_KP_5,  KC_KP_6,  KC_KP_DOT,\
    KC_KP_1,  KC_KP_2,  KC_KP_3,  KC_KP_0   \
  ),

  /*
   * ,---------------------------.
   * |      | INT1 | INT2 | INT5 |
   * |------+------+------+------|
   * |  F13 |  F13 |  F13 |  F13 |
   * |------+------+------+------|
   * |  F13 |  F13 |  F13 |  F13 |
   * |------+------+------+------|
   * |  F13 |  F13 |  F13 |  F13 |
   * `---------------------------'
   */
  [_FUNC] = LAYOUT_ortho_4x4( \
    TO(2),    KC_INT1,  KC_INT2,  KC_INT5, \
    KC_F13,   KC_F14,   KC_F15,   KC_F16,  \
    KC_F17,   KC_F18,   KC_F19,   KC_F20,  \
    KC_F21,   KC_F22,   KC_F23,   KC_F24   \
  ),

  /*
   * ,---------------------------.
   * |      |      |      |      |
   * |------+------+------+------|
   * |      |      |      |      |
   * |------+------+------+------|
   * |      |      |      |      |
   * |------+------+------+------|
   * |      |      |      |      |
   * `---------------------------'
   */
  [_MACRO] = LAYOUT_ortho_4x4( \
    TO(3),    MACRO_13, MACRO_14, MACRO_15, \
    MACRO_09, MACRO_10, MACRO_11, MACRO_12,\
    MACRO_05, MACRO_06, MACRO_07, MACRO_08,\
    MACRO_01, MACRO_02, MACRO_03, MACRO_04 \
  ),

  /* Meta - dump
   * ,---------------------------.
   * |      |      |      |      |
   * |------+------+------+------|
   * |      |      |      |      |
   * |------+------+------+------|
   * |      |      |      |      |
   * |------+------+------+------|
   * |      |      |      |      |
   * `---------------------------'
   */
  [_META] = LAYOUT_ortho_4x4( \
    TO(0),    RGB_TOG,  RGB_MOD,  RGB_M_P, \
    _______,  _______,  _______,  _______, \
    _______,  _______,  _______,  _______, \
    _______,  _______,  _______,  RESET    \
  ),
};

void matrix_init_rgb(void) {
  //TODO: jam in default state of rgb???
  rgblight_enable_noeeprom();
  rgblight_sethsv_noeeprom_cyan();
}

void matrix_scan_user(void) {
  static bool has_ran_yet = false;
  if (!has_ran_yet) {
    has_ran_yet = true;
    matrix_init_rgb();
  }
}

void led_set_user(uint8_t usb_led) {
  // Force-enable Numlock
  if (!(usb_led & (1<<USB_LED_NUM_LOCK))) {
    register_code(KC_NUMLOCK);
    unregister_code(KC_NUMLOCK);
  }
}

uint32_t layer_state_set_user(uint32_t state) {
#ifdef RGBLIGHT_ENABLE
  switch (biton32(state)) {
    case _FUNC:
      rgblight_sethsv_noeeprom_magenta();
      break;
    case _MACRO:
      rgblight_sethsv_noeeprom_purple();
      break;
    case _META:
      rgblight_sethsv_noeeprom_blue();
      break;
    case _NUM:
    default:
      rgblight_sethsv_noeeprom_cyan();
      break;
  }
#endif

  return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch(keycode) {
            case MACRO_01:
            case MACRO_02:
            case MACRO_03:
            case MACRO_04:
            case MACRO_05:
            case MACRO_06:
            case MACRO_07:
            case MACRO_08:
            case MACRO_09:
            case MACRO_10:
            case MACRO_11:
            case MACRO_12:
            case MACRO_13:
            case MACRO_14:
            case MACRO_15:
                SEND_STRING("QMK is the best thing ever!");
                return false;
        }
    }
    return true;
};
