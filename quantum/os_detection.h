/* Copyright 2022 Ruslan Sayfutdinov (@KapJI)
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

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "usb_device_state.h"

typedef enum {
    OS_UNSURE,
    OS_LINUX,
    OS_WINDOWS,
    OS_MACOS,
    OS_IOS,
} os_variant_t;

void         process_wlength(const uint16_t w_length);
os_variant_t detected_host_os(void);
void         erase_wlength_data(void);
void         os_detection_notify_usb_device_state_change(struct usb_device_state usb_device_state);

void os_detection_task(void);

bool process_detected_host_os_kb(os_variant_t os);
bool process_detected_host_os_user(os_variant_t os);

#if defined(SPLIT_KEYBOARD) && defined(SPLIT_DETECTED_OS_ENABLE)
void slave_update_detected_host_os(os_variant_t os);
#endif

#ifdef OS_DETECTION_DEBUG_ENABLE
#    if defined(DYNAMIC_KEYMAP_ENABLE) || defined(VIA_ENABLE)
#        error Cannot enable OS Detection debug mode simultaneously with DYNAMIC_KEYMAP or VIA
#    endif
void print_stored_setups(void);
void store_setups_in_eeprom(void);
#endif
