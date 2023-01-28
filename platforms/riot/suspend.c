// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "platform_deps.h"

#include "suspend.h"
#include "action.h"
#include "wait.h"

/** \brief suspend power down
 *
 * FIXME: needs doc
 */
void suspend_power_down(void) {
    suspend_power_down_quantum();
    // on AVR, this enables the watchdog for 15ms (max), and goes to
    // SLEEP_MODE_PWR_DOWN

    wait_ms(17);
}

/** \brief suspend wakeup condition
 *
 * run immediately after wakeup
 * FIXME: needs doc
 */
void suspend_wakeup_init(void) {
    // TODO: clear keyboard state
    // clear_keyboard();

    suspend_wakeup_init_quantum();
}
