/*
Copyright 2011 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

// convert to L string
#define LSTR(s) XLSTR(s)
#define XLSTR(s) L## #s
// convert to string
#define STR(s) XSTR(s)
#define XSTR(s) #s

#ifdef __cplusplus
extern "C" {
#endif

// uint8_t bitpop(uint8_t bits);
// uint8_t bitpop16(uint16_t bits);
// uint8_t bitpop32(uint32_t bits);

#define bitpop(bits) (uint8_t)__builtin_popcount(bits)
#define bitpop16(bits) (uint8_t)__builtin_popcount(bits)
#define bitpop32(bits) (uint8_t)__builtin_popcountl(bits)

// uint8_t biton(uint8_t bits);
// uint8_t biton16(uint16_t bits);
// uint8_t biton32(uint32_t bits);

#define biton(bits) (bits ? (uint8_t)__builtin_clz(bits) : 0)
#define biton16(bits) (bits ? (uint8_t)__builtin_clz(bits) : 0)
#define biton32(bits) (bits ? (uint8_t)__builtin_clzl(bits) : 0)

//uint8_t  bitrev(uint8_t bits);
// uint16_t bitrev16(uint16_t bits);
// uint32_t bitrev32(uint32_t bits);
#define bitrev8(bits) (uint8_t)(__builtin_bswap16(bits) >> 8)
#define bitrev16(bits) (uint16_t)__builtin_bswap16(bits)
#define bitrev32(bits) (uint32_t)__builtin_bswap32(bits)

#ifdef __cplusplus
}
#endif

#endif
