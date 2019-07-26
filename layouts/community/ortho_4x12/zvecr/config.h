#pragma once

// # minimise size to enable as many features as possible
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION
#define DISABLE_LEADER

#define TAPPING_TERM 150

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
  #if defined(USE_SERIAL)
    #define RGBLED_NUM 5*2
  #else
    #define RGBLED_NUM 5
  #endif
#elif defined(KEYBOARD_keebio_levinson_rev2)
  #undef RGBLED_NUM
  #define RGBLED_NUM 32
  #define RGBLED_SPLIT {16, 16}
  #define RGBLIGHT_LIMIT_VAL 160
#else
  #define RGBLED_NUM 1
  #ifndef RGB_DI_PIN
     #define RGB_DI_PIN D1
  #endif
#endif
