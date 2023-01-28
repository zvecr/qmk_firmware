// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "usb_board_reset.h"
#include "periph/pm.h"

// reuse logic from reset.c where possible - might not be implemented
__attribute__((weak)) void usb_board_reset_in_bootloader(void) {}

__attribute__((weak)) void bootloader_jump(void) {
    usb_board_reset_in_bootloader();
}

__attribute__((weak)) void mcu_reset(void) {
    pm_reboot();
}
