// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "hardware_id.h"
#include "periph/cpuid.h"

hardware_id_t get_hardware_id(void) {
    hardware_id_t id = {0};
#if CPUID_LEN
    static_assert(sizeof(id) >= CPUID_LEN, "CPUID oversized for hardware_id_t");
    cpuid_get(&id);
#endif
    return id;
}
