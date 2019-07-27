#pragma once

#define TAPPING_TERM 200
#define TAPPING_TERM_HOLD 750

// split handling
#if defined(KEYBOARD_lets_split_rev2)
  #define MASTER_LEFT
#endif

// rgb underglow
#define RGBLIGHT_SLEEP
#define RGBLIGHT_ANIMATIONS

#if defined(KEYBOARD_lets_split_rev2)
  #undef RGBLED_NUM
  #define RGBLED_NUM 10
  #if defined(USE_I2C)
    #define RGBLED_SPLIT {5, 5}
  #endif
#elif defined(KEYBOARD_keebio_levinson_rev2)
  #undef RGBLED_NUM
  #define RGBLED_NUM 32
  #define RGBLED_SPLIT {16, 16}
  #define RGBLIGHT_LIMIT_VAL 160
#endif

#ifndef RGBLED_NUM
  #define RGBLED_NUM 1
#endif
#ifndef RGB_DI_PIN
    #define RGB_DI_PIN D1
#endif
