// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "util.h"
#include "usb_config_adapter.h"

#ifndef USB_MAX_POWER_CONSUMPTION
#    define USB_MAX_POWER_CONSUMPTION 500
#endif

const char* qmk_get_usb_manuf(void) { return MANUFACTURER; }
const char* qmk_get_usb_product(void) { return PRODUCT; }
uint16_t    qmk_get_usb_vid(void) { return VENDOR_ID; }
uint16_t    qmk_get_usb_pid(void) { return PRODUCT_ID; }
uint16_t    qmk_get_usb_bcd(void) { return DEVICE_VER; }
uint16_t    qmk_get_usb_max_power(void) { return USB_MAX_POWER_CONSUMPTION; }
