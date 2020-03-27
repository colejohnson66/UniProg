/* ============================================================================
 * File:   upro.cpp
 * Author: Cole Johnson
 * ============================================================================
 * Copyright (c) 2020 Cole Johnson
 * 
 * This file is part of upro.
 * 
 * upro is free software: you can redistribute it and/or modify it under the
 *   terms of the GNU General Public License as published by the Free Software
 *   Foundation, either version 3 of the License, or (at your option) any later
 *   version.
 * 
 * upro is distributed in the hope that it will be useful, but WITHOUT ANY
 *   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *   FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 *   details.
 * 
 * You should have received a copy of the GNU General Public License along with
 *   upro. If not, see <http://www.gnu.org/licenses/>.
 * ============================================================================
 */
#pragma once

#include "configure.h"
#include <cstddef>
#include <cstdint>

// NOTE: will fail if these types (int8_t...) aren't present
// NOTE: will fail if these types (i8...) are already defined

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef intptr_t iptr;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uintptr_t uptr;

typedef float f32;
typedef double f64;

#define HEX86_INLINE inline
