// Copyright 2022 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "usb/usbus.h"

enum usb_interfaces {
#ifndef KEYBOARD_SHARED_EP
    KEYBOARD_INTERFACE,
#else
    SHARED_INTERFACE,
#    define KEYBOARD_INTERFACE SHARED_INTERFACE
#endif

// It is important that the Raw HID interface is at a constant
// interface number, to support Linux/OSX platforms and chrome.hid
// If Raw HID is enabled, let it be always 1.
#ifdef RAW_ENABLE
    RAW_INTERFACE,
#endif

#if defined(MOUSE_ENABLE) && !defined(MOUSE_SHARED_EP)
    MOUSE_INTERFACE,
#else
#    define MOUSE_INTERFACE SHARED_INTERFACE
#endif

#if defined(SHARED_EP_ENABLE) && !defined(KEYBOARD_SHARED_EP)
    SHARED_INTERFACE,
#endif

#ifdef CONSOLE_ENABLE
    CONSOLE_INTERFACE,
#endif

#if defined(DIGITIZER_ENABLE) && !defined(DIGITIZER_SHARED_EP)
    DIGITIZER_INTERFACE,
#else
#    define DIGITIZER_INTERFACE SHARED_INTERFACE
#endif

#ifdef XAP_ENABLE
    XAP_INTERFACE,
#endif

    TOTAL_INTERFACES
};

#define KEYBOARD_EPSIZE 8
#define SHARED_EPSIZE 32
#define MOUSE_EPSIZE 8
#define RAW_EPSIZE 32
#define CONSOLE_EPSIZE 32
#define DIGITIZER_EPSIZE 8
#define XAP_EPSIZE 64

extern usbus_t g_usbus;

typedef struct comp_hid_device_conf {
    uint8_t        id;
    uint32_t       subclass;
    uint32_t       protocol;
    const uint8_t *report_desc;      /**< report descriptor reference */
    size_t         report_desc_size; /**< report descriptor size */
    size_t         ep_size;
    uint8_t        interval;
} comp_hid_device_conf_t;

void usbdrv_init(comp_hid_device_conf_t *config, size_t len);
void usbdrv_write(uint8_t index, const void *buffer, size_t len);

size_t usbdrv_write_timeout(uint8_t id, const void *buffer, size_t len, uint32_t timeout);

uint8_t usbdrv_keyboard_leds(void);

void usbdrv_wake(void);

static inline bool usbdrv_is_connected(void) {
    return g_usbus.state != USBUS_STATE_CONFIGURED;
}
