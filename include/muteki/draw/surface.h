/*
 * Copyright 2024 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file surface.h
 * @brief Surface API.
 */

#ifndef __MUTEKI_DRAW_SURFACE_H__
#define __MUTEKI_DRAW_SURFACE_H__

#include "types/surface.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculate the total on-memory size of a surface (including descriptor).
 * @x_syscall_num `0x1005a`
 * @param surface The surface descriptor.
 * @return The total on-memory size of this surface in bytes.
 */
extern size_t SizeofGraphic(lcd_surface_t *surface);

/**
 * @brief Initialize a buffer as an all-in-one surface.
 * @details If a palette is required (`depth` is 8-bit or less), the palette is assumed to follow the descriptor
 * immediately, then the pixel buffer is assumed to start immediately after the palette. Otherwise the pixel buffer is
 * assumed to follow the descriptor immediately (i.e. palette size is 0) and lcd_surface_t::palette will be set to
 * NULL.
 * @note This function does not allocate any memory. The caller is responsible for allocating a buffer that is large
 * enough to hold the descriptor, the palette (if required) and the pixel buffer.
 * @x_syscall_num `0x1005b`
 * @param surface A buffer that will hold the data.
 * @param width Width of the surface in pixels.
 * @param height Height of the surface in pixels.
 * @param depth The pixel bit depth.
 * @return The same buffer.
 * @see GetImageSizeExt A function that can be used to estimate the required size of the `surface` buffer, based on the
 * same parameters that will be passed to this function.
 */
extern lcd_surface_t *InitGraphic(lcd_surface_t *surface, short width, short height, short depth);

/**
 * @brief Perform blit operation from `src` surface to `dst` surface.
 * @details This copies a rectangle of size `(xsize, ysize)` px on the `src` surface, with its the top left corner
 * located at `(xsrcoffset, ysrcoffset)` px, to the `dst` surface. The top left corner of that rectangle will be
 * aligned to a point on the `dst` surface at `(xdstoffset, ydstoffset)` px. Optionally one can specify one or more
 * processing `flags` to let the blitter perform certain pixel operations on-the-fly.
 * @x_syscall_num `0x10089`
 * @param dst Destination surface.
 * @param xdstoffset X coordinate of the destination surface that align with the top left corner of the blitted image,
 * in pixels.
 * @param ydstoffset Y coordinate of the destination surface that align with the top left corner of the blitted image,
 * in pixels.
 * @param xsize Number of pixel columns to copy.
 * @param ysize Number of pixel rows to copy.
 * @param src Source surface.
 * @param xsrcoffset X coordinate of the source surface that align with the top left corner of the blitted image,
 * in pixels.
 * @param ysrcoffset Y coordinate of the source surface that align with the top left corner of the blitted image,
 * in pixels.
 * @param flags Process flags.
 * @x_void_return
 * @see blit_flag_e Accepted processing flags.
 * @see [BitBlt](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-bitblt) in WinGDI, which this
 * function has been loosely based on.
 */
extern void _BitBlt(
    lcd_surface_t *dst,
    short xdstoffset,
    short ydstoffset,
    short xsize,
    short ysize,
    lcd_surface_t *src,
    int xsrcoffset,
    int ysrcoffset,
    unsigned short flags
);

/**
 * @brief Calculate the buffer size required for an all-in-one surface of a specific size.
 * @details This assumes the surface will be in the same format as the surface linked to the current active LCD.
 * @x_syscall_num `0x10095`
 * @param width The width of the desired surface.
 * @param height The height of the desired surface.
 * @return The buffer size required to contain the entire surface.
 */
extern size_t GetImageSize(short width, short height);

/**
 * @brief Calculate the buffer size required for an all-in-one surface of a specific pixel format and size.
 * @details This is similar to GetImageSize() but it allows arbitrary pixel bit depth.
 * @x_syscall_num `0x10096`
 * @param width The width of the desired surface.
 * @param height The height of the desired surface.
 * @param depth The pixel bit depth of the desired surface.
 * @return The buffer size required to contain the entire surface.
 * @see InitGraphic For some assumptions made for an all-in-one surface format.
 */
extern size_t GetImageSizeExt(short width, short height, short depth);

/**
 * @brief Get the palette and bitmap buffer for an all-in-one surface.
 * @warning The behavior of calling this on a hardware framebuffer backed surface is undefined as those surfaces may
 * allocate the palette and/or the pixel buffer not immediately after the descriptor. Use ::lcd_surface_t::palette
 * or ::lcd_surface_t::buffer instead for those surfaces.
 * @x_syscall_num `0x10097`
 * @param surface The surface descriptor.
 * @return Pointer to surface palette buffer when a palette is available, or otherwise pointer to the pixel buffer.
 */
extern void *ImageData(lcd_surface_t *surface);

/**
 * @brief Get the total on-memory size of a surface, in bytes.
 * @details This is equivalent to
 * @code{.c}
 * size_t SizeofImage(lcd_surface_t *surface) {
 *     return GetImageSize(surface->width, surface->height);
 * }
 * @endcode
 * @x_syscall_num `0x10098`
 * @param surface Pointer to the surface descriptor.
 * @return The on-memory size of the surface.
 */
extern size_t SizeofImage(lcd_surface_t *surface);

/**
 * @brief Dispose an all-in-one surface.
 * @details This is simply a wrapper around _lfree().
 * @warning Behavior is undefined when not called on a dymically allocated (via lmalloc(), lcalloc() or lrealloc())
 * all-in-one surface.
 * @x_syscall_num `0x10099`
 * @param surface The surface descriptor.
 * @x_void_return
 */
extern void FreeImage(lcd_surface_t *surface);
#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_DRAW_SURFACE_H__
