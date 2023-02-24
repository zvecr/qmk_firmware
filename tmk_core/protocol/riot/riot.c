// Copyright 2022 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdio.h>
#include <stdlib.h>

#include "wait.h"
#include "gpio.h"

#include "host.h"
#include "report.h"
#include "host_driver.h"
#include "keyboard.h"
#include "matrix.h"
#include "suspend.h"
#include "debug.h"
#include "usb_device_state.h"
#include "usb_driver.h"
#include "usb_descriptor_common.h"

#include "usb/usbus.h"
#include "usb/usbus/hid.h"
#include "usb/usbus/hid_io.h"

#ifdef NKRO_ENABLE
#    include "keycode_config.h"
#endif

#ifdef XAP_ENABLE
#    include "xap.h"
#    include <string.h>
#endif

#ifdef CONSOLE_ENABLE
#    define RBUF_SIZE 512
#    include "ring_buffer.h"

void console_task(void);
#endif

#ifndef USB_POLLING_INTERVAL_MS
#    define USB_POLLING_INTERVAL_MS 1
#endif

/*------------------------------------------------------------------*
 * Descriptors
 *------------------------------------------------------------------*/
#ifdef KEYBOARD_SHARED_EP
static const uint8_t shared_hid_report[] = {
#    define SHARED_REPORT_STARTED
#else
static const uint8_t report_desc_keyboard[] = {
#endif
    0x05, 0x01, // Usage Page (Generic Desktop)
    0x09, 0x06, // Usage (Keyboard)
    0xA1, 0x01, // Collection (Application)
#ifdef KEYBOARD_SHARED_EP
    0x85, REPORT_ID_KEYBOARD, // Report ID
#endif
    // Modifiers (8 bits)
    0x05, 0x07, //   Usage Page (Keyboard/Keypad)
    0x19, 0xE0, //   Usage Minimum (Keyboard Left Control)
    0x29, 0xE7, //   Usage Maximum (Keyboard Right GUI)
    0x15, 0x00, //   Logical Minimum (0)
    0x25, 0x01, //   Logical Maximum (1)
    0x95, 0x08, //   Report Count (8)
    0x75, 0x01, //   Report Size (1)
    0x81, 0x02, //   Input (Data, Variable, Absolute)
    // Reserved (1 byte)
    0x95, 0x01, //   Report Count (1)
    0x75, 0x08, //   Report Size (8)
    0x81, 0x03, //   Input (Constant)
    // Keycodes (6 bytes)
    0x05, 0x07,       //   Usage Page (Keyboard/Keypad)
    0x19, 0x00,       //   Usage Minimum (0)
    0x29, 0xFF,       //   Usage Maximum (255)
    0x15, 0x00,       //   Logical Minimum (0)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x95, 0x06,       //   Report Count (6)
    0x75, 0x08,       //   Report Size (8)
    0x81, 0x00,       //   Input (Data, Array, Absolute)

    // Status LEDs (5 bits)
    0x05, 0x08, //   Usage Page (LED)
    0x19, 0x01, //   Usage Minimum (Num Lock)
    0x29, 0x05, //   Usage Maximum (Kana)
    0x95, 0x05, //   Report Count (5)
    0x75, 0x01, //   Report Size (1)
    0x91, 0x02, //   Output (Data, Variable, Absolute)
    // LED padding (3 bits)
    0x95, 0x01, //   Report Count (1)
    0x75, 0x03, //   Report Size (3)
    0x91, 0x03, //   Output (Constant)
    0xC0,       // End Collection
#ifndef KEYBOARD_SHARED_EP
};
#endif

#ifdef MOUSE_ENABLE
#    ifndef MOUSE_SHARED_EP
static const uint8_t mouse_hid_report[] = {
#    elif !defined(SHARED_REPORT_STARTED)
static const uint8_t shared_hid_report[] = {
#        define SHARED_REPORT_STARTED
#    endif
    // Mouse report descriptor
    0x05, 0x01, // Usage Page (Generic Desktop)
    0x09, 0x02, // Usage (Mouse)
    0xA1, 0x01, // Collection (Application)
#    ifdef MOUSE_SHARED_EP
    0x85, REPORT_ID_MOUSE, //   Report ID
#    endif
    0x09, 0x01, //   Usage (Pointer)
    0xA1, 0x00, //   Collection (Physical)
    // Buttons (8 bits)
    0x05, 0x09, //     Usage Page (Button)
    0x19, 0x01, //     Usage Minimum (Button 1)
    0x29, 0x08, //     Usage Maximum (Button 8)
    0x15, 0x00, //     Logical Minimum (0)
    0x25, 0x01, //     Logical Maximum (1)
    0x95, 0x08, //     Report Count (8)
    0x75, 0x01, //     Report Size (1)
    0x81, 0x02, //     Input (Data, Variable, Absolute)

#    ifdef MOUSE_EXTENDED_REPORT
    // Boot protocol XY ignored in Report protocol
    0x95, 0x02, //     Report Count (2)
    0x75, 0x08, //     Report Size (8)
    0x81, 0x03, //     Input (Constant)
#    endif

    // X/Y position (2 or 4 bytes)
    0x05, 0x01, //     Usage Page (Generic Desktop)
    0x09, 0x30, //     Usage (X)
    0x09, 0x31, //     Usage (Y)
#    ifndef MOUSE_EXTENDED_REPORT
    0x15, 0x81, //     Logical Minimum (-127)
    0x25, 0x7F, //     Logical Maximum (127)
    0x95, 0x02, //     Report Count (2)
    0x75, 0x08, //     Report Size (8)
#    else
    0x16, 0x01, 0x80, // Logical Minimum (-32767)
    0x26, 0xFF, 0x7F, // Logical Maximum (32767)
    0x95, 0x02,       // Report Count (2)
    0x75, 0x10,       // Report Size (16)
#    endif
    0x81, 0x06, //     Input (Data, Variable, Relative)

    // Vertical wheel (1 byte)
    0x09, 0x38, //     Usage (Wheel)
    0x15, 0x81, //     Logical Minimum (-127)
    0x25, 0x7F, //     Logical Maximum (127)
    0x95, 0x01, //     Report Count (1)
    0x75, 0x08, //     Report Size (8)
    0x81, 0x06, //     Input (Data, Variable, Relative)
    // Horizontal wheel (1 byte)
    0x05, 0x0C,       //     Usage Page (Consumer)
    0x0A, 0x38, 0x02, //     Usage (AC Pan)
    0x15, 0x81,       //     Logical Minimum (-127)
    0x25, 0x7F,       //     Logical Maximum (127)
    0x95, 0x01,       //     Report Count (1)
    0x75, 0x08,       //     Report Size (8)
    0x81, 0x06,       //     Input (Data, Variable, Relative)
    0xC0,             //   End Collection
    0xC0,             // End Collection
#    ifndef MOUSE_SHARED_EP
};
#    endif
#endif

#ifdef DIGITIZER_ENABLE
#    ifndef DIGITIZER_SHARED_EP
static const uint8_t digitizer_hid_report[] = {
#    elif !defined(SHARED_REPORT_STARTED)
    static const uint8_t shared_hid_report[] = {
#        define SHARED_REPORT_STARTED
#    endif      // Digitizer report descriptor
    0x05, 0x0D, // Usage Page (Digitizers)
    0x09, 0x01, // Usage (Digitizer)
    0xA1, 0x01, // Collection (Application)
#    ifdef DIGITIZER_SHARED_EP
    0x85, REPORT_ID_DIGITIZER, //   Report ID
#    endif
    0x09, 0x20, //   Usage (Stylus)
    0xA1, 0x00, //   Collection (Physical)
    // In Range, Tip Switch & Barrel Switch (3 bits)
    0x09, 0x32, //     Usage (In Range)
    0x09, 0x42, //     Usage (Tip Switch)
    0x09, 0x44, //     Usage (Barrel Switch)
    0x15, 0x00, //     Logical Minimum
    0x25, 0x01, //     Logical Maximum
    0x95, 0x03, //     Report Count (3)
    0x75, 0x01, //     Report Size (1)
    0x81, 0x02, //     Input (Data, Variable, Absolute)
    // Padding (5 bits)
    0x95, 0x05, //     Report Count (5)
    0x81, 0x03, //     Input (Constant)

    // X/Y Position (4 bytes)
    0x05, 0x01,       //     Usage Page (Generic Desktop)
    0x09, 0x30,       //     Usage (X)
    0x09, 0x31,       //     Usage (Y)
    0x26, 0xFF, 0x7F, //     Logical Maximum (32767)
    0x95, 0x02,       //     Report Count (2)
    0x75, 0x10,       //     Report Size (16)
    0x65, 0x33,       //     Unit (Inch, English Linear)
    0x55, 0x0E,       //     Unit Exponent (-2)
    0x81, 0x02,       //     Input (Data, Variable, Absolute)
    0xC0,             //   End Collection
    0xC0,             // End Collection
#    ifndef DIGITIZER_SHARED_EP
};
#    endif
#endif

#if defined(SHARED_EP_ENABLE) && !defined(SHARED_REPORT_STARTED)
static const uint8_t shared_hid_report[] = {
#endif

#ifdef EXTRAKEY_ENABLE
    // Extrakeys report descriptor
    0x05, 0x01,             // Usage Page (Generic Desktop)
    0x09, 0x80,             // Usage (System Control)
    0xA1, 0x01,             // Collection (Application)
    0x85, REPORT_ID_SYSTEM, //   Report ID
    0x19, 0x01,             //   Usage Minimum (Pointer)
    0x2A, 0xB7, 0x00,       //   Usage Maximum (System Display LCD Autoscale)
    0x15, 0x01,             //   Logical Minimum
    0x26, 0xB7, 0x00,       //   Logical Maximum
    0x95, 0x01,             //   Report Count (1)
    0x75, 0x10,             //   Report Size (16)
    0x81, 0x00,             //   Input (Data, Array, Absolute)
    0xC0,                   // End Collection

    0x05, 0x0C,               // Usage Page (Consumer)
    0x09, 0x01,               // Usage (Consumer Control)
    0xA1, 0x01,               // Collection (Application)
    0x85, REPORT_ID_CONSUMER, //   Report ID
    0x19, 0x01,               //   Usage Minimum (Consumer Control)
    0x2A, 0xA0, 0x02,         //   Usage Maximum (AC Desktop Show All Applications)
    0x15, 0x01,               //   Logical Minimum
    0x26, 0xA0, 0x02,         //   Logical Maximum
    0x95, 0x01,               //   Report Count (1)
    0x75, 0x10,               //   Report Size (16)
    0x81, 0x00,               //   Input (Data, Array, Absolute)
    0xC0,                     // End Collection
#endif

#ifdef PROGRAMMABLE_BUTTON_ENABLE
    // Programmable buttons report descriptor
    0x05, 0x0C,                          // Usage Page (Consumer)
    0x09, 0x01,                          // Usage (Consumer Control)
    0xA1, 0x01,                          // Collection (Application)
    0x85, REPORT_ID_PROGRAMMABLE_BUTTON, //   Report ID
    0x09, 0x03,                          //   Usage (Programmable Buttons)
    0xA1, 0x04,                          //   Collection (Named Array)
    0x05, 0x09,                          //     Usage Page (Button)
    0x19, 0x01,                          //     Usage Minimum (Button 1)
    0x29, 0x20,                          //     Usage Maximum (Button 32)
    0x15, 0x00,                          //     Logical Minimum (0)
    0x25, 0x01,                          //     Logical Maximum (1)
    0x95, 0x20,                          //     Report Count (32)
    0x75, 0x01,                          //     Report Size (1)
    0x81, 0x02,                          //     Input (Data, Variable, Absolute)
    0xC0,                                //   End Collection
    0xC0,                                // End Collection
#endif

#ifdef NKRO_ENABLE
    0x05, 0x01,           // Usage Page (Generic Desktop)
    0x09, 0x06,           // Usage (Keyboard)
    0xA1, 0x01,           // Collection (Application)
    0x85, REPORT_ID_NKRO, //   Report ID
    // Modifiers (8 bits)
    0x05, 0x07, //   Usage Page (Keyboard/Keypad)
    0x19, 0xE0, //   Usage Minimum (Keyboard Left Control)
    0x29, 0xE7, //   Usage Maximum (Keyboard Right GUI)
    0x15, 0x00, //   Logical Minimum (0)
    0x25, 0x01, //   Logical Maximum (1)
    0x95, 0x08, //   Report Count (8)
    0x75, 0x01, //   Report Size (1)
    0x81, 0x02, //   Input (Data, Variable, Absolute)
    // Keycodes
    0x05, 0x07,                           //   Usage Page (Keyboard/Keypad)
    0x19, 0x00,                           //   Usage Minimum (0)
    0x29, (KEYBOARD_REPORT_BITS * 8 - 1), //   Usage Maximum (247)
    0x15, 0x00,                           //   Logical Minimum (0)
    0x25, 0x01,                           //   Logical Maximum (1)
    0x95, (KEYBOARD_REPORT_BITS * 8),     //   Report Count (248)
    0x75, 0x01,                           //   Report Size (1)
    0x81, 0x02,                           //   Input (Data, Variable, Absolute, Bitfield)

    // Status LEDs (5 bits)
    0x05, 0x08, //   Usage Page (LED)
    0x19, 0x01, //   Usage Minimum (Num Lock)
    0x29, 0x05, //   Usage Maximum (Kana)
    0x95, 0x05, //   Report Count (5)
    0x75, 0x01, //   Report Size (1)
    0x91, 0x02, //   Output (Data, Variable, Absolute)
    // LED padding (3 bits)
    0x95, 0x01, //   Report Count (1)
    0x75, 0x03, //   Report Size (3)
    0x91, 0x03, //   Output (Constant)
    0xC0,       // End Collection
#endif

#ifdef SHARED_EP_ENABLE
};
#endif

#ifdef RAW_ENABLE
static const uint8_t raw_hid_report[] = {
    0x06, HID_VALUE_16(RAW_USAGE_PAGE), // Usage Page (Vendor Defined)
    0x09, RAW_USAGE_ID,                 // Usage (Vendor Defined)
    0xA1, 0x01,                         // Collection (Application)
    // Data to host
    0x09, 0x62,       //   Usage (Vendor Defined)
    0x15, 0x00,       //   Logical Minimum (0)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x95, RAW_EPSIZE, //   Report Count
    0x75, 0x08,       //   Report Size (8)
    0x81, 0x02,       //   Input (Data, Variable, Absolute)
    // Data from host
    0x09, 0x63,       //   Usage (Vendor Defined)
    0x15, 0x00,       //   Logical Minimum (0)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x95, RAW_EPSIZE, //   Report Count
    0x75, 0x08,       //   Report Size (8)
    0x91, 0x02,       //   Output (Data, Variable, Absolute)
    0xC0              // End Collection
};
#endif

#ifdef CONSOLE_ENABLE
static const uint8_t console_hid_report[] = {
    0x06, 0x31, 0xFF, // Usage Page (Vendor Defined - PJRC Teensy compatible)
    0x09, 0x74,       // Usage (Vendor Defined - PJRC Teensy compatible)
    0xA1, 0x01,       // Collection (Application)
    // Data to host
    0x09, 0x75,           //   Usage (Vendor Defined)
    0x15, 0x00,           //   Logical Minimum (0x00)
    0x26, 0xFF, 0x00,     //   Logical Maximum (0x00FF)
    0x95, CONSOLE_EPSIZE, //   Report Count
    0x75, 0x08,           //   Report Size (8)
    0x81, 0x02,           //   Input (Data, Variable, Absolute)
    // Data from host
    0x09, 0x76,           //   Usage (Vendor Defined)
    0x15, 0x00,           //   Logical Minimum (0x00)
    0x26, 0xFF, 0x00,     //   Logical Maximum (0x00FF)
    0x95, CONSOLE_EPSIZE, //   Report Count
    0x75, 0x08,           //   Report Size (8)
    0x91, 0x02,           //   Output (Data)
    0xC0                  // End Collection
};
#endif

#ifdef XAP_ENABLE
static const uint8_t xap_hid_report[] = {
    0x06, 0x51, 0xFF, // Usage Page (Vendor Defined)
    0x09, 0x58,       // Usage (Vendor Defined)
    0xA1, 0x01,       // Collection (Application)
    // Data to host
    0x09, 0x62,       //   Usage (Vendor Defined)
    0x15, 0x00,       //   Logical Minimum (0)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x95, XAP_EPSIZE, //   Report Count
    0x75, 0x08,       //   Report Size (8)
    0x81, 0x02,       //   Input (Data, Variable, Absolute)
    // Data from host
    0x09, 0x63,       //   Usage (Vendor Defined)
    0x15, 0x00,       //   Logical Minimum (0)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x95, XAP_EPSIZE, //   Report Count
    0x75, 0x08,       //   Report Size (8)
    0x91, 0x02,       //   Output (Data, Variable, Absolute)
    0xC0              // End Collection
};
#endif

/*------------------------------------------------------------------*
 * Host driver
 *------------------------------------------------------------------*/
extern uint8_t           keyboard_protocol;
extern report_keyboard_t keyboard_report_sent;
extern report_mouse_t    mouse_report_sent;

static uint8_t keyboard_leds(void);
static void    send_keyboard(report_keyboard_t *report);
static void    send_mouse(report_mouse_t *report);
static void    send_extra(report_extra_t *report);

static host_driver_t driver = {keyboard_leds, send_keyboard, send_mouse, send_extra};

/* ---------------------------------------------------------
 *                  Keyboard functions
 * ---------------------------------------------------------
 */

/* LED status */
static uint8_t keyboard_leds(void) {
    return usbdrv_keyboard_leds();
}

void send_report(uint8_t endpoint, void *report, size_t size) {
    if (g_usbus.state != USBUS_STATE_CONFIGURED) {
        return;
    }
    usbdrv_write(endpoint, report, size);
}

/* prepare and start sending a report IN
 * not callable from ISR or locked state */
void send_keyboard(report_keyboard_t *report) {
    uint8_t ep   = KEYBOARD_INTERFACE;
    size_t  size = KEYBOARD_REPORT_SIZE;

    /* If we're in Boot Protocol, don't send any report ID or other funky fields */
    if (!keyboard_protocol) {
        send_report(ep, &report->mods, 8);
    } else {
#ifdef NKRO_ENABLE
        if (keymap_config.nkro) {
            ep   = SHARED_INTERFACE;
            size = sizeof(struct nkro_report);
        }
#endif

        send_report(ep, report, size);
    }

    keyboard_report_sent = *report;
}

/* ---------------------------------------------------------
 *                     Mouse functions
 * ---------------------------------------------------------
 */

void send_mouse(report_mouse_t *report) {
#ifdef MOUSE_ENABLE
    send_report(MOUSE_INTERFACE, report, sizeof(report_mouse_t));
    mouse_report_sent = *report;
#endif
}

/* ---------------------------------------------------------
 *                   Extrakey functions
 * ---------------------------------------------------------
 */

void send_extra(report_extra_t *report) {
#ifdef EXTRAKEY_ENABLE
    send_report(SHARED_INTERFACE, report, sizeof(report_extra_t));
#endif
}

void send_programmable_button(report_programmable_button_t *report) {
#ifdef PROGRAMMABLE_BUTTON_ENABLE
    send_report(SHARED_INTERFACE, report, sizeof(report_programmable_button_t));
#endif
}

void send_joystick(report_joystick_t *report) {
#ifdef JOYSTICK_ENABLE
    send_report(JOYSTICK_INTERFACE, report, sizeof(report_joystick_t));
#endif
}

void send_digitizer(report_digitizer_t *report) {
#ifdef DIGITIZER_ENABLE
    send_report(DIGITIZER_INTERFACE, report, sizeof(report_digitizer_t));
#endif
}

/*------------------------------------------------------------------*
 * RAW HID
 *------------------------------------------------------------------*/
#ifdef RAW_ENABLE

static uint8_t raw_output_buffer[RAW_EPSIZE];
static uint8_t raw_output_received_bytes = 0;

void raw_hid_dump(uint8_t *data, uint8_t length) {
    if (raw_output_received_bytes + length > RAW_EPSIZE) {
        return;
    }

    memcpy(&raw_output_buffer[raw_output_received_bytes], data, length);
    raw_output_received_bytes += length;
}

void raw_hid_send(uint8_t *data, uint8_t length) {
    if (length != RAW_EPSIZE) {
        return;
    }

    if (g_usbus.state != USBUS_STATE_CONFIGURED) {
        return;
    }

    usbdrv_write(RAW_INTERFACE, data, length);
    usbdrv_write(RAW_INTERFACE, 0, 0);
}

__attribute__((weak)) void raw_hid_receive(uint8_t *data, uint8_t length) {
    // Users should #include "raw_hid.h" in their own code
    // and implement this function there. Leave this as weak linkage
    // so users can opt to not handle data coming in.
}

void raw_hid_task(void) {
    if (raw_output_received_bytes == RAW_EPSIZE) {
        raw_hid_receive(raw_output_buffer, RAW_EPSIZE);
        raw_output_received_bytes = 0;
    }
}
#endif

/*------------------------------------------------------------------*
 * XAP
 *------------------------------------------------------------------*/
#ifdef XAP_ENABLE

static uint8_t xap_output_buffer[XAP_EPSIZE];
static uint8_t xap_output_received_bytes = 0;

extern void xap_receive(xap_token_t token, const uint8_t *data, size_t length);

void xap_dump(uint8_t *data, uint8_t length) {
    if (xap_output_received_bytes + length > XAP_EPSIZE) {
        return;
    }

    memcpy(&xap_output_buffer[xap_output_received_bytes], data, length);
    xap_output_received_bytes += length;
}

void xap_send_base(uint8_t *data, uint8_t length) {
    if (length != XAP_EPSIZE) {
        return;
    }

    if (g_usbus.state != USBUS_STATE_CONFIGURED) {
        return;
    }

    static bool timed_out = false;

    const uint32_t timeout = timed_out ? 100 : 5000;
    usbdrv_write_timeout(XAP_INTERFACE, data, length, timeout);
    usbdrv_write_timeout(XAP_INTERFACE, 0, 0, timeout);
}

void xap_send(xap_token_t token, xap_response_flags_t response_flags, const void *data, size_t length) {
    uint8_t                rdata[XAP_EPSIZE] = {0};
    xap_response_header_t *header            = (xap_response_header_t *)&rdata[0];
    header->token                            = token;

    if (length > (XAP_EPSIZE - sizeof(xap_response_header_t))) response_flags &= ~(XAP_RESPONSE_FLAG_SUCCESS);
    header->flags = response_flags;

    if (response_flags & (XAP_RESPONSE_FLAG_SUCCESS)) {
        header->length = (uint8_t)length;
        if (data != NULL) {
            memcpy(&rdata[sizeof(xap_response_header_t)], data, length);
        }
    }
    xap_send_base(rdata, sizeof(rdata));
}

void xap_broadcast(uint8_t type, const void *data, size_t length) {
    uint8_t                 rdata[XAP_EPSIZE] = {0};
    xap_broadcast_header_t *header            = (xap_broadcast_header_t *)&rdata[0];
    header->token                             = XAP_BROADCAST_TOKEN;
    header->type                              = type;

    if (length > (XAP_EPSIZE - sizeof(xap_broadcast_header_t))) return;

    header->length = (uint8_t)length;
    if (data != NULL) {
        memcpy(&rdata[sizeof(xap_broadcast_header_t)], data, length);
    }
    xap_send_base(rdata, sizeof(rdata));
}

void xap_receive_base(const void *data) {
    const uint8_t *       u8data = (const uint8_t *)data;
    xap_request_header_t *header = (xap_request_header_t *)&u8data[0];
    if (header->length <= (XAP_EPSIZE - sizeof(xap_request_header_t))) {
        xap_receive(header->token, &u8data[sizeof(xap_request_header_t)], header->length);
    }
}

void xap_task(void) {
    if (xap_output_received_bytes == XAP_EPSIZE) {
        xap_receive_base(xap_output_buffer);
        xap_output_received_bytes = 0;
    }
}
#endif

/*------------------------------------------------------------------*
 * Console
 *------------------------------------------------------------------*/
#ifdef CONSOLE_ENABLE

int8_t sendchar(uint8_t c) {
    rbuf_enqueue(c);
    return 0;
}

void console_task(void) {
    if (!rbuf_has_data()) {
        return;
    }

    char    send_buf[CONSOLE_EPSIZE] = {0};
    uint8_t send_buf_count           = 0;
    while (rbuf_has_data() && send_buf_count < CONSOLE_EPSIZE) {
        send_buf[send_buf_count++] = rbuf_dequeue();
    }

    if (g_usbus.state != USBUS_STATE_CONFIGURED) {
        return;
    }

    static bool timed_out = false;

    const uint32_t timeout = timed_out ? 100 : 5000;

    timed_out = usbdrv_write_timeout(CONSOLE_INTERFACE, send_buf, sizeof(send_buf), timeout) == 0;
    usbdrv_write_timeout(CONSOLE_INTERFACE, 0, 0, timeout);
}
#endif

/*------------------------------------------------------------------*
 * Protocol bindings
 *------------------------------------------------------------------*/
void protocol_setup(void) {
    usb_device_state_init();
}

void protocol_pre_init(void) {
    // clang-format off
    comp_hid_device_conf_t config[TOTAL_INTERFACES] = {
#ifndef KEYBOARD_SHARED_EP
        {
            .id               = KEYBOARD_INTERFACE,
            .subclass         = USB_HID_SUBCLASS_BOOT,
            .protocol         = USB_HID_PROTOCOL_KEYBOARD,
            .report_desc      = report_desc_keyboard,
            .report_desc_size = sizeof(report_desc_keyboard),
            .ep_size          = KEYBOARD_EPSIZE,
            .interval         = USB_POLLING_INTERVAL_MS
        },
#else
        {
            .id               = SHARED_INTERFACE,
            .subclass         = USB_HID_SUBCLASS_BOOT,
            .protocol         = USB_HID_PROTOCOL_KEYBOARD,
            .report_desc      = shared_hid_report,
            .report_desc_size = sizeof(shared_hid_report),
            .ep_size          = SHARED_EPSIZE,
            .interval         = USB_POLLING_INTERVAL_MS
        },
#endif
#ifdef RAW_ENABLE
        {
            .id               = RAW_INTERFACE,
            .subclass         = USB_HID_SUBCLASS_NONE,
            .protocol         = USB_HID_PROTOCOL_NONE,
            .report_desc      = raw_hid_report,
            .report_desc_size = sizeof(raw_hid_report),
            .ep_size          = RAW_EPSIZE,
            .interval         = 1
        },
#endif
#if defined(MOUSE_ENABLE) && !defined(MOUSE_SHARED_EP)
        {
            .id               = MOUSE_INTERFACE,
            .subclass         = USB_HID_SUBCLASS_BOOT,
            .protocol         = USB_HID_PROTOCOL_MOUSE,
            .report_desc      = mouse_hid_report,
            .report_desc_size = sizeof(mouse_hid_report),
            .ep_size          = MOUSE_EPSIZE,
            .interval         = USB_POLLING_INTERVAL_MS
        },
#endif
#if defined(SHARED_EP_ENABLE) && !defined(KEYBOARD_SHARED_EP)
        {
            .id               = SHARED_INTERFACE,
            .subclass         = USB_HID_SUBCLASS_NONE,
            .protocol         = USB_HID_PROTOCOL_NONE,
            .report_desc      = shared_hid_report,
            .report_desc_size = sizeof(shared_hid_report),
            .ep_size          = SHARED_EPSIZE,
            .interval         = USB_POLLING_INTERVAL_MS
        },
#endif
#ifdef CONSOLE_ENABLE
        {
            .id               = CONSOLE_INTERFACE,
            .subclass         = USB_HID_SUBCLASS_NONE,
            .protocol         = USB_HID_PROTOCOL_NONE,
            .report_desc      = console_hid_report,
            .report_desc_size = sizeof(console_hid_report),
            .ep_size          = CONSOLE_EPSIZE,
            .interval         = 1
        },
#endif
#if defined(DIGITIZER_ENABLE) && !defined(DIGITIZER_SHARED_EP)
        {
            .id               = DIGITIZER_INTERFACE,
            .subclass         = USB_HID_SUBCLASS_NONE,
            .protocol         = USB_HID_PROTOCOL_NONE,
            .report_desc      = digitizer_hid_report,
            .report_desc_size = sizeof(digitizer_hid_report),
            .ep_size          = DIGITIZER_EPSIZE,
            .interval         = USB_POLLING_INTERVAL_MS
        },
#endif
#ifdef XAP_ENABLE
        {
            .id               = XAP_INTERFACE,
            .subclass         = USB_HID_SUBCLASS_NONE,
            .protocol         = USB_HID_PROTOCOL_NONE,
            .report_desc      = xap_hid_report,
            .report_desc_size = sizeof(xap_hid_report),
            .ep_size          = XAP_EPSIZE,
            .interval         = 1
        },
#endif
    };
    // clang-format on
    usbdrv_init(config, TOTAL_INTERFACES);
}

void protocol_post_init(void) {
    host_set_driver(&driver);
    wait_ms(50);
}

void protocol_pre_task(void) {
    if (g_usbus.state == USBUS_STATE_SUSPEND) {
        while (g_usbus.state == USBUS_STATE_SUSPEND) {
            suspend_power_down();
            /* Remote wakeup */
            if (suspend_wakeup_condition()) {
                // TODO: handle remote wakeup
                usbdrv_wake();

#if USB_SUSPEND_WAKEUP_DELAY > 0
                // Some hubs, kvm switches, and monitors do
                // weird things, with USB device state bouncing
                // around wildly on wakeup, yielding race
                // conditions that can corrupt the keyboard state.
                //
                // Pause for a while to let things settle...
                wait_ms(USB_SUSPEND_WAKEUP_DELAY);
#endif
            }
        }
        suspend_wakeup_init();
    }
}

void protocol_post_task(void) {
#ifdef RAW_ENABLE
    raw_hid_task();
#endif
#ifdef XAP_ENABLE
    xap_task();
#endif
#ifdef CONSOLE_ENABLE
    console_task();
#endif
}
