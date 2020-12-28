/* Copyright 2020 zvecr <git@zvecr.com>
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

#include "quantum.h"
#define ____ KC_NO

/* This a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 */
#define LAYOUT( \
    K000, K001, K002, K003, K004, K005, K006, K007, K008, K009, K010, K011, K012, K013, K015, \
    K100, K101, K102, K103, K104, K105, K106, K107, K108, K109, K110, K111, K112, K113, K115, \
    K200, K201, K202, K203, K204, K205, K206, K207, K208, K209, K210, K211, K212, K215, \
    K300, K301, K302, K303, K304, K305, K306, K307, K308, K309, K310, K311, K313, K315, \
    K400, K401,             K404,                         K409,       K411, K413, K415  \
) \
{ \
    { K000, K001, K002, K003, K004, K005, K006, K007, K008, K009, K010, K011, K012, K013, ____, K015}, \
    { K100, K101, K102, K103, K104, K105, K106, K107, K108, K109, K110, K111, K112, K113, ____, K115}, \
    { K200, K201, K202, K203, K204, K205, K206, K207, K208, K209, K210, K211, K212, ____, ____, K215}, \
    { K300, K301, K302, K303, K304, K305, K306, K307, K308, K309, K310, K311, ____, K313, ____, K315}, \
    { K400, K401, ____, ____, K404, ____, ____, ____, ____, K409, ____, K411, ____, K413, ____, K415}, \
}

#define LAYOUT_split_bs( \
    K000, K001, K002, K003, K004, K005, K006, K007, K008, K009, K010, K011, K012, K013, K014, K015, \
    K100, K101, K102, K103, K104, K105, K106, K107, K108, K109, K110, K111, K112, K113, K115, \
    K200, K201, K202, K203, K204, K205, K206, K207, K208, K209, K210, K211, K212, K215, \
    K300, K301, K302, K303, K304, K305, K306, K307, K308, K309, K310, K311, K313, K315, \
    K400, K401,             K404,                         K409,       K411, K413, K415  \
) \
{ \
    { K000, K001, K002, K003, K004, K005, K006, K007, K008, K009, K010, K011, K012, K013, K014, K015}, \
    { K100, K101, K102, K103, K104, K105, K106, K107, K108, K109, K110, K111, K112, K113, ____, K115}, \
    { K200, K201, K202, K203, K204, K205, K206, K207, K208, K209, K210, K211, K212, ____, ____, K215}, \
    { K300, K301, K302, K303, K304, K305, K306, K307, K308, K309, K310, K311, ____, K313, ____, K315}, \
    { K400, K401, ____, ____, K404, ____, ____, ____, ____, K409, ____, K411, ____, K413, ____, K415}, \
}
