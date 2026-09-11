/*
 * Copyright 2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file static.h
 * @brief Static text field (single-line label) widget constructor and methods.
 * @details
 * Static text fields act as single-line labels on a deskbox. The user can customize the look of the label
 * by modifying its parameters. The string rendering is internally delegated to WriteAlignString().
 *
 * A typical declaration is as follows:
 * @code{c}
 * ui_static_t *label = CreateStatic(
 *     0x00,  // Or anything you want.
 *     x, y, x + max_label_width - 1, 0,
 *     _BUL("Hello world!"),
 *     SERIF_CJK,
 *     LCD_RGB_FROM_U8(0x00, 0x00, 0x00),
 *     ui_static_text_options(LCD_PRINT_NONE, 0),
 *     ui_static_style_options(UI_TEXT_LAYOUT_LEFT_CENTER, UI_STATIC_FRAME_KIND_OUTLINE)
 * );
 * @endcode
 *
 * @warning This widget assumes its parent is a deskbox type. Therefore it cannot be used with arbitrary generic
 * group components.
 */

#ifndef __MUTEKI_UI_WIDGETS_STATIC_H__
#define __MUTEKI_UI_WIDGETS_STATIC_H__

#include "../types/static.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a static text field widget.
 * @details
 * If @p y1 is less than @p y0 , it defaults to @p y0 + GetFontHeight( @p font_type ) + 6, with the exception of
 * when @p font_type is `0xffff`, then it will simply be @p y0 + 6. Note the absence of the font width here.
 * @x_syscall_num{0x1014d}
 * @param tag @x_term{tag}
 * @param x0 @x_term{x0}
 * @param y0 @x_term{y0}
 * @param x1 @x_term{x1}
 * @param y1 @x_term{y1}
 * @param text Label text.
 * @param font_type Font type, or `0xffff` to use the default font.
 * @param text_color Text color.
 * @param text_options Text options.
 * @param style_options Widget style options.
 * @return The created static text field widget.
 * @see font_type_e Builtin font types.
 * @see LCD_RGB_FROM_U8 Format RGB color values.
 * @see ui_static_text_options Helper function to format @p text_options .
 * @see ui_static_style_options Helper function to format @p style_options .
 */
extern ui_static_t *CreateStatic(
    unsigned short tag,
    short x0,
    short y0,
    short x1,
    short y1,
    const UTF16 *text,
    unsigned short font_type,
    unsigned int text_color,
    unsigned int text_options,
    unsigned int style_options
);

/**
 * @brief Format the `text_options` parameter of the CreateStatic() syscall.
 * @param text_flags Flags accepted by WriteAlignString().
 * @param left_inset Extra left inset in number of pixels.
 * @return The formatted value.
 */
static inline unsigned int ui_static_text_options(unsigned short text_flags, short left_inset) {
    return text_flags | (left_inset << 16);
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_WIDGETS_STATIC_H__
