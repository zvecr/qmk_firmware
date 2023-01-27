// Copyright 2022 zvecr <git@zvecr.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "shift_register.h"
#include "lib/shifter.h"
#include "analog.h"
#include "wait.h"

sr_exp_t sr_exp_data = {
    .bit.HUB_CONNECT = 0,
    .bit.HUB_RESET_N = 0,
    .bit.S_A_UP      = 0,  // 0 = DN1 to A and UP to B, 1 = DN1 to B and UP to A
    .bit.S_A_EXT     = 1,  // 0 = Onboard, 1 = Offboard
    .bit.S_B_EXT     = 0,  // 1 = Onboard, 0 = Offboard
    .bit.ENB         = 0,  // Disable CON1
    .bit.ENA         = 0,  // Disable CON2
    .bit.E_VBUS_1    = 0,
    .bit.E_VBUS_2    = 0,
    .bit.SRC_1_A5    = 1,  // Set CON1 CC A5 as Rd 5.1k
    .bit.SRC_1_B5    = 1,  // Set CON1 CC B5 as Rd 5.1k
    .bit.SRC_2_A5    = 1,  // Set CON2 CC A5 as Rd 5.1k
    .bit.SRC_2_B5    = 1,  // Set CON2 CC B5 as Rd 5.1k
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
    uint16_t v_con_3 = analogReadPin(B4);
    uint16_t v_con_4 = analogReadPin(B5);

    // TODO: dynamic port port configure logic?
    if ((v_con_1 + v_con_3) > (v_con_2 + v_con_4)) {
        sr_exp_data.bit.ENB      = 1;  // Enable CON1
        sr_exp_data.bit.ENA      = 1;  // Enable CON2
        sr_exp_data.bit.SRC_1_A5 = 1;  // HOST on USBC-1
        sr_exp_data.bit.SRC_1_B5 = 1;
        sr_exp_data.bit.SRC_2_A5 = 0;  // EXTRA available on USBC-2
        sr_exp_data.bit.SRC_2_B5 = 0;
        sr_exp_data.bit.S_A_UP   = 1;  // 0 = DN1 to A and UP to B, 1 = DN1 to B and UP to A
        sr_exp_data.bit.S_A_EXT  = 1;  // 0 = Onboard, 1 = Offboard
        sr_exp_data.bit.S_B_EXT  = 0;  // 1 = Onboard, 0 = Offboard
        sr_exp_data.bit.E_VBUS_1 = 1;  // USBC-1 enable full power I/O
        sr_exp_data.bit.E_VBUS_2 = 1;  // USBC-2 disable full power I/O
    } else {
        sr_exp_data.bit.ENB      = 1;  // Enable CON1
        sr_exp_data.bit.ENA      = 1;  // Enable CON2
        sr_exp_data.bit.SRC_1_A5 = 0;  // HOST on USBC-2
        sr_exp_data.bit.SRC_1_B5 = 0;
        sr_exp_data.bit.SRC_2_A5 = 1;  // EXTRA available on USBC-1
        sr_exp_data.bit.SRC_2_B5 = 1;
        sr_exp_data.bit.S_A_UP   = 0;  // 0 = DN1 to A and UP to B, 1 = DN1 to B and UP to A
        sr_exp_data.bit.S_A_EXT  = 1;  // 0 = Onboard, 1 = Offboard
        sr_exp_data.bit.S_B_EXT  = 0;  // 1 = Onboard, 0 = Offboard
        sr_exp_data.bit.E_VBUS_1 = 1;  // USBC-1 enable full power I/O
        sr_exp_data.bit.E_VBUS_2 = 1;  // USBC-2 disable full power I/O
    }

    SR_EXP_flush();
}

void SR_EXP_ISSI_enable(void) {
    sr_exp_data.bit.IRST = 0;
    sr_exp_data.bit.SDB_N = 1;

    SR_EXP_flush();
}
