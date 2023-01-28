// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "bootloader.h"

__attribute__((weak)) void bootloader_jump(void) {}
__attribute__((weak)) void mcu_reset(void) {}
