// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#include <stdbool.h>
#include "periph/flashpage.h"
#include "timer.h"
#include "wear_leveling.h"
#include "wear_leveling_internal.h"

#define WEAR_LEVELING_EFL_TOTAL_PAGES ((WEAR_LEVELING_BACKING_SIZE + (FLASHPAGE_SIZE - 1)) / FLASHPAGE_SIZE)

#ifndef WEAR_LEVELING_EFL_FIRST_PAGE
#    if defined(QMK_MCU_NRF52840) && defined(BOOTLOADER_ADAFRUIT_NRF52)
#        define WEAR_LEVELING_EFL_FIRST_PAGE (flashpage_page((void *)(CPU_FLASH_BASE + 0XED000)))
#    else
#endif

// TODO: assuming last page can overwrite bootloaders...
// #define WEAR_LEVELING_EFL_FIRST_PAGE (FLASHPAGE_NUMOF - WEAR_LEVELING_EFL_TOTAL_PAGES)

#ifndef WEAR_LEVELING_EFL_FIRST_PAGE
#        error WEAR_LEVELING_EFL_FIRST_PAGE was not set.
#    endif
#endif

bool backing_store_init(void) {
    bs_dprintf("Init\n");
    return true;
}

bool backing_store_lock(void) {
    bs_dprintf("Lock  \n");
    return true;
}

bool backing_store_unlock(void) {
    bs_dprintf("Unlock\n");
    return true;
}

bool backing_store_erase(void) {
#ifdef WEAR_LEVELING_DEBUG_OUTPUT
    uint32_t start = timer_read32();
#endif

    for (int i = 0; i < WEAR_LEVELING_EFL_TOTAL_PAGES; i++) {
        flashpage_erase(WEAR_LEVELING_EFL_FIRST_PAGE + i);
    }

    bs_dprintf("Backing store erase took %ldms to complete\n", ((long)(timer_read32() - start)));
    return true;
}

bool backing_store_read(uint32_t address, backing_store_int_t *value) {
    uint32_t offset = (uint32_t)flashpage_addr(WEAR_LEVELING_EFL_FIRST_PAGE) + address;
    memcpy(value, (void *)offset, sizeof(backing_store_int_t));

    bs_dprintf("Read  ");
    wl_dump(offset, value, sizeof(backing_store_int_t));
    return true;
}

bool backing_store_write(uint32_t address, backing_store_int_t value) {
    uint32_t offset = (uint32_t)flashpage_addr(WEAR_LEVELING_EFL_FIRST_PAGE) + address;
    if (offset % FLASHPAGE_WRITE_BLOCK_ALIGNMENT) {
        return false;
    }

    bs_dprintf("Write ");
    wl_dump(offset, &value, sizeof(backing_store_int_t));
    flashpage_write((void *)offset, &value, sizeof(backing_store_int_t));
    return true;
}
