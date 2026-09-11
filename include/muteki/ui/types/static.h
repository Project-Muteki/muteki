/*
 * Copyright 2024-2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file static.h
 * @brief Static text field (single-line label) widget type definition.
 */

#ifndef __MUTEKI_UI_TYPES_STATIC_H__
#define __MUTEKI_UI_TYPES_STATIC_H__

#include "framework.h"
#include "text_layout.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Kinds of outer frame style.
 */
enum ui_static_frame_type_e {
    /**
     * @brief No frame.
     */
    UI_STATIC_FRAME_KIND_NONE,
    /**
     * @brief Draw the outline with the default outline color.
     * @todo Link to the system color struct when that is done.
     */
    UI_STATIC_FRAME_KIND_OUTLINE,
    /**
     * @brief Sunken look.
     */
    UI_STATIC_FRAME_KIND_SUNKEN,
    /**
     * @brief Raised look.
     */
    UI_STATIC_FRAME_KIND_RAISED,
};

/**
 * @brief Static text field widget struct.
 */
typedef struct ui_static_s {
    /**
     * @brief @x_term{superc}
     */
    ui_component_t component;
    /**
     * @brief @x_term{sbz}
     */
    unsigned int unk_0x34;
    /**
     * @brief UTF-16-encoded text.
     */
    const UTF16 *text;
    /**
     * @brief Style options value.
     * @details Contains packed layout and frame type control values.
     */
    unsigned int style_options;
    /**
     * @brief Text color.
     * @see LCD_RGB_FROM_U8
     */
    unsigned int text_color;
    /**
     * @brief Font type.
     * @details Defaults to ::SERIF_SMALL_CJK if unset by the user (i.e. passing `0xffff` as the `font_type` to the constructor).
     * @see font_type_e
     */
    unsigned short font_type;
    /**
     * @brief Text print control flags.
     * @see lcd_print_flag_e
     */
    unsigned short text_flags;
    /**
     * @brief Text left inset value.
     */
    short left_inset;
    /**
     * @brief @x_term{padding}
     */
    short unk_0x4a;
} ui_static_t;

/**
 * @brief Format the ui_static_t::style_options value.
 * 
 * @param layout The layout type.
 * @param frame_type The frame type.
 * @return The formatted value.
 */
static inline unsigned int ui_static_style_options(
    enum ui_text_layout_e layout,
    enum ui_static_frame_type_e frame_type
) {
    return ((layout & 0xf) << 22) | ((frame_type & 0xf) << 26);
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif  //  __MUTEKI_UI_TYPES_STATIC_H__
