// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Left side (front)
#define D3 GPIO_PIN(0, 6)
#define D2 GPIO_PIN(0, 8)
//      GND
//      GND
#define D1 GPIO_PIN(0, 17)
#define D0 GPIO_PIN(0, 20)
#define D4 GPIO_PIN(0, 22)
#define C6 GPIO_PIN(0, 24)
#define D7 GPIO_PIN(1, 0)
#define E6 GPIO_PIN(0, 11)
#define B4 GPIO_PIN(1, 4)
#define B5 GPIO_PIN(1, 6)

// Right side (front)
//      RAW
//      GND
//      RESET
//      VCC
#define F4 GPIO_PIN(0, 31)
#define F5 GPIO_PIN(0, 29)
#define F6 GPIO_PIN(0, 2)
#define F7 GPIO_PIN(1, 15)
#define B1 GPIO_PIN(1, 13)
#define B3 GPIO_PIN(1, 11)
#define B2 GPIO_PIN(0, 10)
#define B6 GPIO_PIN(0, 9)

// LEDs (Mapped to unused pins to avoid collisions)
#define D5 GPIO_PIN(1, 1)
#define B0 GPIO_PIN(1, 2)
