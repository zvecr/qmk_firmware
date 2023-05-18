// Copyright 2023 Massdrop, Inc.
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>

/* Data structure to define Shift Register output expander hardware */
/* This structure gets shifted into registers LSB first */
typedef union {
    struct {
        uint8_t RSVD4 : 1;       /*!< bit:      0                                               */
        uint8_t RSVD3 : 1;       /*!< bit:      1                                               */
        uint8_t RSVD2 : 1;       /*!< bit:      2                                               */
        uint8_t RSVD1 : 1;       /*!< bit:      3                                               */
        uint8_t SDB_N : 1;       /*!< bit:      4  SHUTDOWN THE CHIP WHEN 0, RUN WHEN 1         */
        uint8_t IRST : 1;        /*!< bit:      5  RESET THE IS3733 I2C WHEN 1, RUN WHEN 0      */
        uint8_t SRC_2 : 1;       /*!< bit:      6  ADVERTISE A SOURCE TO USBC-2 CC              */
        uint8_t SRC_1 : 1;       /*!< bit:      7  ADVERTISE A SOURCE TO USBC-1 CC              */
        uint8_t E_VBUS_2 : 1;    /*!< bit:      8  ENABLE 5V OUT TO USBC-2 WHEN 1               */
        uint8_t E_VBUS_1 : 1;    /*!< bit:      9  ENABLE 5V OUT TO USBC-1 WHEN 1               */
        uint8_t E_DN1_N : 1;     /*!< bit:     10  ENABLE DN1 1:2 MUX WHEN 0                    */
        uint8_t S_DN1 : 1;       /*!< bit:     11  SELECT DN1 PATH 0:USBC-1, 1:USBC-2           */
        uint8_t E_UP_N : 1;      /*!< bit:     12  ENABLE SUP 1:2 MUX WHEN 0                    */
        uint8_t S_UP : 1;        /*!< bit:     13  SELECT UP PATH 0:USBC-1, 1:USBC-2            */
        uint8_t HUB_RESET_N : 1; /*!< bit:     14  RESET USB HUB WHEN 0, RUN WHEN 1             */
        uint8_t HUB_CONNECT : 1; /*!< bit:     15  SIGNAL VBUS CONNECT TO USB HUB WHEN 1        */
    } bit;                       /*!< Structure used for bit access                             */
    uint16_t reg;                /*!< Type      used for register access                        */
} sr_exp_t;

extern sr_exp_t sr_exp_data;

void SR_EXP_init(void);
void SR_EXP_flush(void);

void SR_EXP_USB_enable(void);
void SR_EXP_ISSI_enable(void);
