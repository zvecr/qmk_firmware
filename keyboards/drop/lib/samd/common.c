// Copyright 2023 Massdrop, Inc.
// SPDX-License-Identifier: GPL-2.0-or-later

#include "drivers/usb2422.h"
#include "lib/samd/i2c_multi_bus.h"

//#include "shift_register.h"
void SR_EXP_init(void);
void SR_EXP_USB_enable(void);
void SR_EXP_ISSI_enable(void);

void keyboard_pre_init_vendor(void) {
    i2c_claim_bus(0);

    SR_EXP_init();
    SR_EXP_USB_enable();
    SR_EXP_ISSI_enable();

    USB2422_init();
    USB2422_configure();

    i2c_claim_bus(1);
}
