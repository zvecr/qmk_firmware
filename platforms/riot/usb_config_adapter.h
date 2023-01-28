// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>

const char* qmk_get_usb_manuf(void);
const char* qmk_get_usb_product(void);
uint16_t    qmk_get_usb_vid(void);
uint16_t    qmk_get_usb_pid(void);
uint16_t    qmk_get_usb_bcd(void);
uint16_t    qmk_get_usb_max_power(void);

// Replace the usual build time config with stub function - These are linked
// in later to avoid RIOT having knowledge of the build system (and the whole config.h situation)
#define CONFIG_USB_MANUF_STR qmk_get_usb_manuf()
#define CONFIG_USB_PRODUCT_STR qmk_get_usb_product()
#define CONFIG_USB_VID qmk_get_usb_vid()
#define CONFIG_USB_PID qmk_get_usb_pid()
#define CONFIG_USB_BCD qmk_get_usb_bcd()
#define CONFIG_USB_MAX_POWER qmk_get_usb_max_power()

// Static config
// #define CONFIG_USB_SPEC_BCDVERSION_1_1
#define CONFIG_USBUS_EP0_SIZE_8
#define CONFIG_USB_REM_WAKEUP 1
