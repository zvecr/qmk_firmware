#include QMK_KEYBOARD_H

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

// Bodge keycodes
#define KC_CAD LALT(LCTL(KC_DEL))
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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Esc  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Tab  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |  Up  |Enter |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Ctrl | GUI  | Alt  | Meta |Lower |Space |Space |Raise |   /  | Left | Down |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY] = LAYOUT_ortho_4x12( \
  KC_GESC, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC, \
  KC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_UP,   KC_ENT,  \
  KC_LCTL, KC_LGUI, KC_LALT, META,    LOWER,   RAI_SPC, LOW_SPC, RAISE,   KC_SLSH, KC_LEFT, KC_DOWN, KC_RGHT \
),

/* Lower - symbols
 * ,-----------------------------------------------------------------------------------.
 * |   ~  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  | Del  |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |   _  |   +  |   {  |   }  |  |   |      |   -  |   =  |   [  |   ]  |  |   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |   -  |   =  |   [  |   ]  |  \   |      |ISO ~ |ISO | |ISO # | PgUp |      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |ISO \ | Home | PgDn | End  |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT_ortho_4x12( \
  KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_DEL,  \
  _______, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, _______, KC_MINS, KC_EQL, KC_LBRC, KC_RBRC, KC_PIPE, \
  _______, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS, _______, KC_ISOT, KC_ISOP, KC_NUHS, KC_PGUP, _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, KC_NUBS, KC_HOME, KC_PGDN, KC_END   \
),

/* Raise - numbers
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Del  |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |      |      |      |      |      | Bksp |   4  |   5  |   6  |  +   |  .   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |      |      |      |      |      |  .   |   1  |   2  |   3  |      |      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |ISO \ |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_ortho_4x12( \
  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,  \
  _______, _______, _______, _______, _______, _______, KC_BSPC, KC_4,    KC_5,    KC_6,    KC_PLUS, KC_DOT,  \
  _______, _______, _______, _______, _______, _______, KC_DOT,  KC_1,    KC_2,    KC_3,    _______, _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, KC_NUBS, _______, _______, _______  \
),

/* Meta - funcs + macros
 * ,-----------------------------------------------------------------------------------.
 * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |  F13 |  F14 |  F15 |  F16 |  F17 |  F18 |  F19 |  F20 |  F21 |  F22 |  F23 |  F24 |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |MACRO1|MACRO2|MACRO3|MACRO4|MACRO5| INT1 | INT2 | INT5 |      |      |      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_META] =  LAYOUT_ortho_4x12( \
  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  \
  KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F20,  KC_F21,  KC_F22,  KC_F23,  KC_F24,  \
  _______, MACRO_1, MACRO_2, MACRO_3, MACRO_4, MACRO_5, KC_INT1, KC_INT2, KC_INT5, _______, _______, _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______  \
),

/* Adjust (meta && lower) - dump
 * ,-----------------------------------------------------------------------------------.
 * | Play | Next | Vol- | Vol+ | Mute |      |R Toggl|R Mode|R Rev|R Grad| PrtSc| CADel|
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |      |      |      |      |      |R  HUI|R  SAI|R  VAI|      |      |      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |rShift|      |      |      |      |      |R  HUD|R  SAD|R  VAD|      |      |      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |rCtrl |      | rAlt |      |      |      |      |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] =  LAYOUT_ortho_4x12( \
  KC_MPLY, KC_MNXT, KC_VOLD, KC_VOLU, KC_MUTE, _______, RGB_TOG, RGB_MOD, RGB_RMOD,RGB_M_G, KC_PSCR, KC_CAD,  \
  _______, _______, _______, _______, _______, _______, RGB_HUI, RGB_SAI, RGB_VAI, _______, _______, _______, \
  KC_RSFT, _______, _______, _______, _______, _______, RGB_HUD, RGB_SAD, RGB_VAD, _______, _______, _______, \
  KC_RCTRL, _______,KC_RALT, _______, _______, _______, _______, _______, _______, _______, _______, _______  \
)

};

uint32_t layer_state_set_user(uint32_t state) {
  state = update_tri_layer_state(state, _LOWER, _META, _ADJUST);
  #ifdef RGBLIGHT_ENABLE
  rgblight_config_t rgblight_config;
  switch (biton32(state)) {
    case _LOWER:
      rgblight_sethsv_noeeprom_cyan();
      rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
      break;
    case _RAISE:
      rgblight_sethsv_noeeprom_magenta();
      rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
      break;
    case _META:
      rgblight_sethsv_noeeprom_goldenrod();
      rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
      break;
    case _ADJUST:
      rgblight_sethsv_noeeprom_springgreen();
      rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
      break;
    default:
    // set back to current eeprom values controled via RGB_TOG,RGB_MOD,etc
    rgblight_config.raw = eeconfig_read_rgblight();
    if (rgblight_config.mode) {
      rgblight_sethsv_noeeprom(rgblight_config.hue, rgblight_config.sat, rgblight_config.val);
      rgblight_mode_noeeprom(rgblight_config.mode);
    }
  }
  #endif

  return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch(keycode) {
            case MACRO_1:
            case MACRO_2:
            case MACRO_3:
            case MACRO_4:
            case MACRO_5:
                SEND_STRING("QMK is the best thing ever!");
                return false;
        }
    }
    return true;
};
