/*
 * Copyright 2024 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file surface.h
 * @brief Surface type definitions.
 */

#ifndef __MUTEKI_DRAW_TYPES_SURFACE_H__
#define __MUTEKI_DRAW_TYPES_SURFACE_H__

#include "../../common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Process flags for blit operations.
 * @warning These are stand-alone and not stackable with bitwise-OR operations (<tt>'|'</tt>).
 * @todo Need to look into this further. 2 and 16 seem to draw the image verbatim like 0. Nothing is being drawn when
 * setting any other flags. (Could it be related to some other properties on the surface descriptor such as transparent
 * color?)
 */
enum blit_flag_e {
    /**
     * @brief Nothing.
     * @details This just copies the pixels as-is.
     */
    BLIT_NONE = 0,
    /**
     * @brief Perform AND operation between source line and destination line.
     * @details This effectively does a bit-wise color masking of the destination surface.
     */
    BLIT_AND = 0x2,
    /**
     * @brief Perform OR operation between source line and destination line.
     * @details This effectively does a bit-wise painting onto the destination surface.
     * Colors will also be mixed in a bit-wise fashion.
     */
    BLIT_OR = 0x3,
    /**
     * @brief Perform XOR operation between source line and destination line.
     * @details This effectively does a bit-wise color inversion of the destination surface.
     */
    BLIT_XOR = 0x4,
    /**
     * @brief Honor transparent color settings on the current LCD canvas.
     * @details This copies the pixels when it's not exactly the same as the transparent color of the current LCD
     * canvas.
     */
    BLIT_TRANSPARENT = 0xe,
    /**
     * @brief Perform alpha blending (when supported).
     * @details This is done without gamma correction.
     */
    BLIT_ALPHA_BLENDING = 0xf,
    /**
     * @brief Perform alpha blending (when supported).
     * @details This is similar to #BLIT_ALPHA_BLENDING but instead of using alpha values per-pixel, it uses only the
     * alpha value of the first pixel for the entire surface. This makes the process slightly faster if the entire
     * image has the same alpha values for every pixel.
     */
    BLIT_ALPHA_BLENDING_FAST = 0x10,
};

/**
 * @brief Valid pixel formats for LCD surfaces.
 * @details All color values are assumed to be little endian.
 */
enum lcd_surface_pixfmt_e {
    /**
     * @brief 1-bit index color.
     */
    LCD_SURFACE_PIXFMT_L1 = 1,
    /**
     * @brief 4-bit index color.
     */
    LCD_SURFACE_PIXFMT_L4 = 4,
    /**
     * @brief 8-bit index color.
     */
    LCD_SURFACE_PIXFMT_L8 = 8,
    /**
     * @brief 12-bit RGB444.
     */
    LCD_SURFACE_PIXFMT_RGB444 = 12,
    /**
     * @brief 16-bit RGB565.
     */
    LCD_SURFACE_PIXFMT_RGB565 = 16,
    /**
     * @brief 24-bit RGB.
     */
    LCD_SURFACE_PIXFMT_RGB = 24,
    /**
     * @brief 32-bit XRGB.
     */
    LCD_SURFACE_PIXFMT_XRGB = 32,
    /**
     * @brief 32-bit ARGB (alias of #LCD_SURFACE_PIXFMT_XRGB).
     */
    LCD_SURFACE_PIXFMT_ARGB = 32,
};

/**
 * @brief Valid pixel buffer encoding for LCD surfaces.
 */
enum lcd_surface_encoding_e {
    /**
     * @brief Pixel buffer is contained in HCA container.
     */
    LCD_SURFACE_ENCODING_HCA = 1,
    /**
     * @brief Pixel buffer is in raw format.
     */
    LCD_SURFACE_ENCODING_RAW = 2,
};

/**
 * @brief Descriptor of an LCD drawing surface or hardware framebuffer.
 * @details This contains format description of the pixel/framebuffer and a pointer to the actual buffer.
 */
struct lcd_surface_s {
    /**
     * @brief Header magic.
     * @details Always `"PX"` without NUL.
     */
    char magic[2]; // 0:2
    /**
     * @brief Width of the framebuffer.
     */
    short width; // 2:4
    /**
     * @brief Height of the framebuffer.
     */
    short height; // 4:6
    /**
     * @brief Depth of the color in bits.
     * @see lcd_surface_pixfmt_e
     */
    short depth; // 6:8
    /**
     * @brief Pixel row size in bytes (aka. pitch if you know SDL).
     * @details This is usually 4-bytes aligned.
     */
    short xsize; // 8:10
    /**
     * @brief Pixel buffer encoding.
     * @see lcd_surface_encoding_e
     */
    short encoding; // 10:12
    /**
     * @brief Palette used to pack RGBA color into pixels.
     * @details Only make sense when using indexed color. It should be `NULL` otherwise.
     */
    int *palette; // 12:16
    /**
     * @brief The actual framebuffer/surface pixel buffer region.
     * @details The actual format varies and depends on the parameters provided above.
     */
    void *buffer; // 16:20
}; // 20 bytes

typedef struct lcd_surface_s lcd_surface_t;

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_DRAW_TYPES_SURFACE_H__
