/* Copyright 2021 zvecr<git@zvecr.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <hal.h>
#include "eeprom_driver.h"

// TODO: migrate to battery backed sram? (4k instead of 64-128)

#if HAL_USE_RTC == FALSE
#    error("RTC NOT ENABLED")
#endif
#if RTC_HAS_STORAGE == FALSE
#    error("RTC STORAGE NOT SUPPORTED")
#endif
#if EECONFIG_SIZE > STM32_RTC_STORAGE_SIZE
#    pragma message("RTC STORAGE too small for eeconfig")
#endif

void eeprom_driver_init(void) {}

void eeprom_driver_erase(void) {
    uint8_t buf[STM32_RTC_STORAGE_SIZE] = {0};
    psRead(&RTCD1, 0, STM32_RTC_STORAGE_SIZE, buf);
}

void eeprom_read_block(void *buf, const void *addr, size_t len) {
    uint32_t offset = (uint32_t)addr;

    if (offset + len >= STM32_RTC_STORAGE_SIZE) return;

    psRead(&RTCD1, offset, len, buf);
}

void eeprom_write_block(const void *buf, void *addr, size_t len) {
    uint32_t offset = (uint32_t)addr;

    if (offset + len >= STM32_RTC_STORAGE_SIZE) return;

    psWrite(&RTCD1, offset, len, buf);
}
