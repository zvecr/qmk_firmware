// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#ifndef __ASSEMBLER__
#    include "periph/flashpage.h"
#endif

// Work out how many bytes per write to internal flash
#ifndef BACKING_STORE_WRITE_SIZE
#    if defined(FLASHPAGE_WRITE_BLOCK_SIZE)
#        define BACKING_STORE_WRITE_SIZE FLASHPAGE_WRITE_BLOCK_SIZE
#    else
#        error "Could not automatically determine BACKING_STORE_WRITE_SIZE"
#    endif
#endif

// 2kB backing space allocated
#ifndef WEAR_LEVELING_BACKING_SIZE
#    define WEAR_LEVELING_BACKING_SIZE 2048
#endif // WEAR_LEVELING_BACKING_SIZE

// 1kB logical EEPROM
#ifndef WEAR_LEVELING_LOGICAL_SIZE
#    define WEAR_LEVELING_LOGICAL_SIZE 1024
#endif // WEAR_LEVELING_LOGICAL_SIZE
