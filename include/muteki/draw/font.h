/*
 * Copyright 2024 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file font.h
 * @brief Font-related API.
 */

#ifndef __MUTEKI_DRAW_FONT_H__
#define __MUTEKI_DRAW_FONT_H__

#include "types/font.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get the width of the character `c` used in `font_type`.
 * @x_syscall_num `0x1004d`
 * @param c UTF-16 codepoint of the character.
 * @param font_type The font type.
 * @return The width in pixels of that character.
 * @see font_type_e List of known font types.
 */
extern unsigned short GetCharWidth(UTF16 c, unsigned short font_type);

/**
 * @brief Get the height of the font `font_type`.
 * @x_syscall_num `0x1004e`
 * @param font_type The font type.
 * @return The height in pixels of that font.
 * @see font_type_e List of known font types.
 */
extern unsigned short GetFontHeight(unsigned short font_type);

/**
 * @brief Calculate the width of a glyph.
 * @x_syscall_num `0x102bf`
 * @param c Unicode codepoint of the character. Going above U+FFFF is possible.
 * @param[in, out] font The font info record.
 * @param[out] out_glyph_extent Actual horizontal extent of the glyph in pixels.
 * @return Simple width of the glyph in pixels, without accounting for features such as italic overhangs.
 */
extern unsigned short GetFontWidth(unsigned int c, font_info_t *font, unsigned short *out_glyph_extent);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_DRAW_FONT_H__
