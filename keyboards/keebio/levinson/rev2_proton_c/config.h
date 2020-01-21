/*
Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert
Copyright 2018 Danny Nguyen <danny@keeb.io>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

/* USB Device descriptor parameter */
#define VENDOR_ID       0xCB10
#define PRODUCT_ID      0x1146
#define DEVICE_VER      0x0200
#define MANUFACTURER    Keebio
#define PRODUCT         Levinson
#define DESCRIPTION     Split 40 percent ortholinear keyboard

/* key matrix size */
// Rows are doubled-up
#define MATRIX_ROWS 8
#define MATRIX_COLS 6

// wiring of each half
#define MATRIX_ROW_PINS { B5, B3, B2, B1 }
#define MATRIX_COL_PINS { A0, B8, B13, B14, B15, B9 }

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 5

/* serial.c configuration for split keyboard */
#define SOFT_SERIAL_PIN B6

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/* ws2812 RGB LED */
#define RGB_DI_PIN A9
#define RGBLIGHT_ANIMATIONS
#define RGBLIGHT_LIMIT_VAL 50
#define RGBLED_NUM 32    // Number of LEDs
#define RGBLED_SPLIT { 16, 16 }

#define WS2812_PWM_DRIVER PWMD1
#define WS2812_PWM_CHANNEL 2
#define WS2812_PWM_PAL_MODE 6
#define WS2812_DMA_STREAM STM32_DMA1_STREAM5    // DMA Stream for TIMx_UP
#define WS2812_DMA_CHANNEL 5                    // DMA Channel for TIMx_UP

/* Backlight LEDs */
#define BACKLIGHT_PIN B0
#define BACKLIGHT_LEVELS 7
//#define BACKLIGHT_BREATHING

#define BACKLIGHT_PWM_DRIVER PWMD3
#define BACKLIGHT_PWM_CHANNEL 3
#define BACKLIGHT_PAL_MODE 2

#define NO_USB_STARTUP_CHECK
//#define SELECT_SOFT_SERIAL_SPEED 0
