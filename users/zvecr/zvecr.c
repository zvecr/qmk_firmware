#include "zvecr.h"
#include <sendstring_uk.h>

uint32_t layer_state_set_user(uint32_t state) {
  //state = update_tri_layer_state(state, _LOWER, _META, _ADJUST);
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

uint16_t get_tapping_term(uint16_t keycode) {
  switch (keycode) {
    case RAI_SPC:
    case LOW_SPC:
    case LOW_BSPC:
    case LOW_ESC:
    case ADJ_MENU:
      return TAPPING_TERM_HOLD;
    default:
      return TAPPING_TERM;
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch(keycode) {
            case MACRO_1:
                SEND_STRING("z.zvecr@gmail.com");
                break;
            case MACRO_2:
                SEND_STRING("info@zvecr.com");
                break;
            case MACRO_3:
                SEND_STRING("zvecr@hotmail.co.uk");
                break;
            case MACRO_4:
                SEND_STRING("asdf");
                break;
            case MACRO_5:
                SEND_STRING("password");
                break;
        }
    }
    return true;
};
