// Copyright 2023 Andrew Kannan
// SPDX-License-Identifier: GPL-2.0-or-later

#include "satisfaction_core.h"
#include "eeprom.h"
#ifdef BACKLIGHT_ENABLE
#    include "backlight.h"
#endif

void pre_encoder_mode_change(void){
  if(encoder_mode == ENC_MODE_CLOCK_SET){
    RTCDateTime timespec;
    timespec.year = year_config;
    timespec.month = month_config;
    timespec.day = day_config;
    // timespec.dayofweek = last_timespec.dayofweek;
    // timespec.dstflag = last_timespec.dstflag;
    timespec.millisecond = (hour_config * 60 + minute_config) * 60 * 1000;
    rtcSetTime(&RTCD1, &timespec);
  }
}

void post_encoder_mode_change(void){
  if(encoder_mode == ENC_MODE_CLOCK_SET){
    hour_config = (last_minute / 60);
    minute_config = last_minute % 60;
    year_config = last_timespec.year;
    month_config = last_timespec.month;
    day_config = last_timespec.day;
    time_config_idx = 0;
  }
}

void change_encoder_mode(bool negative){
  pre_encoder_mode_change();
  if(enabled_encoder_modes == 0){
    enabled_encoder_modes = 0x1F;
  }
  do {
    if(negative){
      if (encoder_mode == 0){
        encoder_mode = _NUM_ENCODER_MODES - 1;
      } else{
        encoder_mode = encoder_mode - 1;
      }
    } else {
        encoder_mode = (encoder_mode + 1) % _NUM_ENCODER_MODES;
    }
  } while(((1 << encoder_mode) & enabled_encoder_modes) == 0);
  post_encoder_mode_change();
}

void update_time_config(int8_t increment){
  uint8_t day_limit = 31;
  uint16_t adjusted_year = 1980 + year_config;
  switch(time_config_idx){
    case 0: // hour
    default:
      hour_config = (hour_config + increment) % 24;
      if (hour_config < 0){
        hour_config += 24;
      }
      break;
    case 1: // minute
      minute_config = (minute_config + increment) % 60;
      if (minute_config < 0){
        minute_config += 60;
      }
      break;
    case 2: // year
      year_config += increment;
      break;
    case 3: // month
      month_config = (month_config % 12) + increment;
      if (month_config <= 0){
        month_config += 12;
      }
      break;
    case 4: //day
      if (month_config == 9 || month_config == 4 || month_config == 6 || month_config == 11){
        day_limit = 30;
      } else if(month_config == 2){
        day_limit = adjusted_year % 4 == 0 && !(adjusted_year % 100 == 0 && adjusted_year % 400 != 0) ? 29 : 28;
      }
      day_config = (day_config % day_limit) + increment;
      if(day_config <= 0){
        day_config += day_limit;
      }
      break;
  }
}

uint16_t handle_encoder_clockwise(void){
  uint16_t mapped_code = 0;
  switch(encoder_mode){
    default:
    case ENC_MODE_VOLUME:
      mapped_code = KC_VOLU;
      break;
    case ENC_MODE_MEDIA:
      mapped_code = KC_MEDIA_NEXT_TRACK;
      break;
    case ENC_MODE_SCROLL:
      mapped_code = QK_MOUSE_WHEEL_DOWN;
      break;
#ifdef BACKLIGHT_ENABLE
    case ENC_MODE_BACKLIGHT:
      backlight_increase();
      if(get_backlight_level() != 0){
        backlight_enable();
      }
      break;
#endif
    case ENC_MODE_BRIGHTNESS:
      mapped_code = KC_BRIGHTNESS_UP;
      break;
#ifdef DYNAMIC_KEYMAP_ENABLE
    case ENC_MODE_CUSTOM0:
      mapped_code = retrieve_custom_encoder_config(0, ENC_CUSTOM_CW);
      break;
    case ENC_MODE_CUSTOM1:
      mapped_code = retrieve_custom_encoder_config(1, ENC_CUSTOM_CW);
      break;
    case ENC_MODE_CUSTOM2:
      mapped_code = retrieve_custom_encoder_config(2, ENC_CUSTOM_CW);
      break;
#endif
    case ENC_MODE_CLOCK_SET:
      update_time_config(1);
      break;
  }
  return mapped_code;
}

uint16_t handle_encoder_ccw(void){
  uint16_t mapped_code = 0;
  switch(encoder_mode){
    default:
    case ENC_MODE_VOLUME:
      mapped_code = KC_VOLD;
      break;
    case ENC_MODE_MEDIA:
      mapped_code = KC_MEDIA_PREV_TRACK;
      break;
    case ENC_MODE_SCROLL:
      mapped_code = QK_MOUSE_WHEEL_UP;
      break;
#ifdef BACKLIGHT_ENABLE
    case ENC_MODE_BACKLIGHT:
      backlight_decrease();
      if(get_backlight_level() == 0){
        backlight_disable();
      }
      break;
#endif
    case ENC_MODE_BRIGHTNESS:
      mapped_code = KC_BRIGHTNESS_DOWN;
      break;
#ifdef DYNAMIC_KEYMAP_ENABLE
    case ENC_MODE_CUSTOM0:
      mapped_code = retrieve_custom_encoder_config(0, ENC_CUSTOM_CCW);
      break;
    case ENC_MODE_CUSTOM1:
      mapped_code = retrieve_custom_encoder_config(1, ENC_CUSTOM_CCW);
      break;
    case ENC_MODE_CUSTOM2:
      mapped_code = retrieve_custom_encoder_config(2, ENC_CUSTOM_CCW);
      break;
#endif

    case ENC_MODE_CLOCK_SET:
      update_time_config(-1);
      break;
  }
  return mapped_code;
}

uint16_t handle_encoder_press(void){
  uint16_t mapped_code = 0;
  switch(encoder_mode){
    case ENC_MODE_VOLUME:
      mapped_code = KC_MUTE;
      break;
    default:
    case ENC_MODE_MEDIA:
      mapped_code = KC_MEDIA_PLAY_PAUSE;
      break;
    case ENC_MODE_SCROLL:
      mapped_code = QK_MOUSE_BUTTON_3;
      break;
#ifdef BACKLIGHT_ENABLE
    case ENC_MODE_BACKLIGHT:
      breathing_toggle();
      break;
#endif
#ifdef DYNAMIC_KEYMAP_ENABLE
    case ENC_MODE_CUSTOM0:
      mapped_code = retrieve_custom_encoder_config(0, ENC_CUSTOM_PRESS);
      break;
    case ENC_MODE_CUSTOM1:
      mapped_code = retrieve_custom_encoder_config(1, ENC_CUSTOM_PRESS);
      break;
    case ENC_MODE_CUSTOM2:
      mapped_code = retrieve_custom_encoder_config(2, ENC_CUSTOM_PRESS);
      break;
#endif
    case ENC_MODE_CLOCK_SET:
      time_config_idx = (time_config_idx + 1) % 5;
    case ENC_MODE_BRIGHTNESS:
      break;
  }
  return mapped_code;
}


uint16_t retrieve_custom_encoder_config(uint8_t encoder_idx, uint8_t behavior){
#ifdef DYNAMIC_KEYMAP_ENABLE
    uint32_t offset = EEPROM_CUSTOM_ENCODER_OFFSET + (encoder_idx * 6) + (behavior * 2);
    //big endian
    uint8_t hi, lo;
    read_custom_config(&hi, offset+0, 1);
    read_custom_config(&lo, offset+1, 1);
    uint16_t keycode = hi << 8;
    keycode |= lo;
    return keycode;
#else
    return 0;
#endif
}

void set_custom_encoder_config(uint8_t encoder_idx, uint8_t behavior, uint16_t new_code){
#ifdef DYNAMIC_KEYMAP_ENABLE
    uint32_t offset = EEPROM_CUSTOM_ENCODER_OFFSET + (encoder_idx * 6) + (behavior * 2);
    uint8_t hi = new_code >> 8;
    uint8_t lo = new_code & 0xFF;
    write_custom_config(&hi, offset+0, 1);
    write_custom_config(&lo, offset+1, 1);
#endif
}
