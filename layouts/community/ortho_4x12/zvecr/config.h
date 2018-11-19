#pragma once

// split handling
#if defined(KEYBOARD_lets_split_rev2)
  #define USE_SERIAL
  #define MASTER_LEFT
#endif

// rgb underglow
#define RGBLIGHT_SLEEP
#define RGBLIGHT_ANIMATIONS
#if defined(KEYBOARD_lets_split_rev2)
  #undef RGBLED_NUM
  #define RGBLED_NUM 10
#endif
