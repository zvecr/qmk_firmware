#pragma once

// # minimise size to enable as many features as possible
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION
#define DISABLE_LEADER

// rgb underglow
#define RGBLIGHT_SLEEP
#define RGBLIGHT_ANIMATIONS
#undef RGBLED_NUM
#if defined(KEYBOARD_40percentclub_nori)
  #define RGBLED_NUM 4
#else
  #define RGBLED_NUM 1
  #ifndef RGB_DI_PIN
     #define RGB_DI_PIN D1
  #endif
#endif
