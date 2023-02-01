// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#include "usb_driver.h"
#include "usb_util.h"

void usb_disconnect(void) {
    // RIOT currently does not have this functionality
}

bool usb_connected_state(void) {
    return usbdrv_is_connected();
}
