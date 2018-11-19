#pragma once

// split handling
#if defined(KEYBOARD_lets_split_rev2)
  #define USE_SERIAL
  #define MASTER_LEFT
#endif

// rgb underglow
#define RGBLIGHT_SLEEP
#define RGBLIGHT_ANIMATIONS
#undef RGBLED_NUM
#if defined(KEYBOARD_lets_split_rev2)
  #define RGBLED_NUM 10
#else
  #define RGBLED_NUM 1
  #ifndef RGB_DI_PIN
     #define RGB_DI_PIN D1
  #endif
#endif
