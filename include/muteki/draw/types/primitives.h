/*
 * Copyright 2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file primitives.h
 * @brief Drawing routine primitive types.
 */


#ifndef __MUTEKI_DRAW_TYPES_PRIMITIVES_H__
#define __MUTEKI_DRAW_TYPES_PRIMITIVES_H__

#include "../../common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Rectangle used to represent usable drawing area in an LCD descriptor.
 * @details The area is double-inclusive (i.e. `{0, 0, 479, 271}` represents an area of 480x272 px).
 */
struct rect_s {
    /** @brief @x_term x0 */
    short x0; // (lcd_t[0x6c:0x6e])
    /** @brief @x_term y0 */
    short y0; // (lcd_t[0x6e:0x70])
    /** @brief @x_term x1 */
    short x1; // (lcd_t[0x70:0x72])
    /** @brief @x_term y1 */
    short y1; // (lcd_t[0x72:0x74])
}; // 0x8 bytes

typedef struct rect_s rect_t;

/**
 * @brief Convert separate RGB values to integer RGB representation
 * @param r Red value.
 * @param g Green value.
 * @param b Blue value.
 */
#define RGB_FROM_U8(r, g, b) ((r & 0xff) << 16 | ((g & 0xff) << 8) | (b & 0xff))

#ifdef __cplusplus
} // extern "C"
#endif

#endif  // __MUTEKI_DRAW_TYPES_PRIMITIVES_H__
