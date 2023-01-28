// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "periph/wdt.h"
#include "periph/pm.h"

__attribute__((weak)) void bootloader_jump(void) {
    wdt_setup_reboot(0, 5);
    wdt_start();

    while (1)
        ;  // Won't get here
}

__attribute__((weak)) void mcu_reset(void) {
    pm_reboot();
}
