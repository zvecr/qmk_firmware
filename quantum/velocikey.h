#pragma once

#include <stdint.h>
#include <stdbool.h>

void velocikey_init(void);

bool velocikey_enabled(void);
void velocikey_enable(void);
void velocikey_disable(void);
void velocikey_toggle(void);
void velocikey_toggle_noeeprom(void);

void    velocikey_accelerate(void);
void    velocikey_decelerate(void);
uint8_t velocikey_match_speed(uint8_t minValue, uint8_t maxValue);
