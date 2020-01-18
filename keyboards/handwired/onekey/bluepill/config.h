/* Copyright 2019
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "config_common.h"

#define MATRIX_COL_PINS { B0 }
#define MATRIX_ROW_PINS { A7 }
#define UNUSED_PINS

#define BACKLIGHT_PIN           A0
#define BACKLIGHT_PWM_DRIVER    PWMD2
#define BACKLIGHT_PWM_CHANNEL   1

#define RGBLIGHT_LIMIT_VAL 50
#define RGB_DI_PIN A1
#define WS2812_PWM_DRIVER PWMD2
#define WS2812_PWM_CHANNEL 2
//#define WS2812_PWM_PAL_MODE 2
#define WS2812_DMA_STREAM STM32_DMA1_STREAM2    // DMA Stream for TIMx_UP
#define WS2812_DMA_CHANNEL 2                    // DMA Channel for TIMx_UP
