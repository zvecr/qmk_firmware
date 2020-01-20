#pragma once

// ortho_4x12 overrides

// split handling
#if defined(KEYBOARD_lets_split_rev2)
  #define MASTER_LEFT
#endif

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
  #define RGBLIGHT_LIMIT_VAL 100
#endif

#ifndef RGBLED_NUM
  #define RGBLED_NUM 1
#endif
#ifndef RGB_DI_PIN
    #define RGB_DI_PIN D1
#endif
