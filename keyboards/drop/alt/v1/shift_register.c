// Copyright 2023 Massdrop, Inc.
// SPDX-License-Identifier: GPL-2.0-or-later

#include "shift_register.h"
#include "lib/samd/shift_out.h"
#include "analog.h"
#include "wait.h"

sr_exp_t sr_exp_data = {
    .bit.HUB_CONNECT = 0,
    .bit.HUB_RESET_N = 0,
    .bit.S_UP        = 0,
    .bit.E_UP_N      = 1,
    .bit.S_DN1       = 1,
    .bit.E_DN1_N     = 1,
    .bit.E_VBUS_1    = 0,
    .bit.E_VBUS_2    = 0,
    .bit.SRC_1       = 1,
    .bit.SRC_2       = 1,
    .bit.IRST        = 1,
    .bit.SDB_N       = 0,
};


void SR_EXP_init(void) {
    shift_init();

    SR_EXP_flush();

    shift_enable();
}

void SR_EXP_flush(void) {
    //
    shift_out((const uint8_t *)&sr_exp_data, sizeof(sr_exp_data));
}

void SR_EXP_USB_enable(void) {
    sr_exp_data.bit.HUB_CONNECT = 1;  // connect signal
    sr_exp_data.bit.HUB_RESET_N = 1;  // reset high
    SR_EXP_flush();

    wait_ms(250);  // Allow power dissapation time on CC lines

    uint16_t v_con_1 = analogReadPin(B2);
    uint16_t v_con_2 = analogReadPin(B1);

    // TODO: dynamic port port configure logic?
    sr_exp_data.bit.E_UP_N   = 0;  // HOST enable
    sr_exp_data.bit.E_DN1_N  = 0;  // EXTRA enable
    sr_exp_data.bit.E_VBUS_1 = 1;  // USBC-1 enable full power I/O
    sr_exp_data.bit.E_VBUS_2 = 1;  // USBC-2 enable full power I/O

    if (v_con_1 > v_con_2) {
        sr_exp_data.bit.S_UP  = 0;  // HOST to USBC-1
        sr_exp_data.bit.S_DN1 = 1;  // EXTRA to USBC-2
        sr_exp_data.bit.SRC_1 = 1;  // HOST on USBC-1
        sr_exp_data.bit.SRC_2 = 0;  // EXTRA available on USBC-2
    } else {
        sr_exp_data.bit.S_UP  = 1;  // EXTRA to USBC-1
        sr_exp_data.bit.S_DN1 = 0;  // HOST to USBC-2
        sr_exp_data.bit.SRC_1 = 0;  // EXTRA available on USBC-1
        sr_exp_data.bit.SRC_2 = 1;  // HOST on USBC-2
    }

    SR_EXP_flush();
}

void SR_EXP_ISSI_enable(void) {
    sr_exp_data.bit.IRST = 0;
    sr_exp_data.bit.SDB_N = 1;

    SR_EXP_flush();
}
