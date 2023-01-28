// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <irq.h>

static __inline__ uint8_t __interrupt_disable__(void) {
    return irq_disable();
}

static __inline__ void __interrupt_enable__(const unsigned *__s) {
    irq_enable();

    __asm__ volatile("" ::: "memory");
    (void)__s;
}

static __inline__ void __interrupt_restore__(const unsigned *__s) {
    irq_restore(*__s);

    __asm__ volatile("" ::: "memory");
}

#define ATOMIC_BLOCK(type) for (type, __ToDo = 1; __ToDo; __ToDo = 0)
#define ATOMIC_FORCEON unsigned status_save __attribute__((__cleanup__(__interrupt_enable__))) = __interrupt_disable__()
#define ATOMIC_RESTORESTATE unsigned status_save __attribute__((__cleanup__(__interrupt_restore__))) = __interrupt_disable__()

#define ATOMIC_BLOCK_RESTORESTATE ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
#define ATOMIC_BLOCK_FORCEON ATOMIC_BLOCK(ATOMIC_FORCEON)
