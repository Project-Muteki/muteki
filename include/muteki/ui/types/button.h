/*
 * Copyright 2024-2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file button.h
 * @brief Button widget type definition.
 */

#ifndef __MUTEKI_UI_TYPES_BUTTON_H__
#define __MUTEKI_UI_TYPES_BUTTON_H__

#include "framework.h"
#include "text_layout.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Valid button visual states.
 */
enum ui_button_visual_state_e {
    /**
     * @brief Redraw the button as released/not pressed down.
     */
    UI_BUTTON_VS_RELEASED = 0,
    /**
     * @brief Redraw the button as pressed down.
     */
    UI_BUTTON_VS_PRESSED = 1,
    /**
     * @brief Do nothing and keep the current on-screen state.
     */
    UI_BUTTON_VS_UNCHANGED = 10,
};

/**
 * @brief Convert a ::ui_text_layout_e member to a ui_button_t::content_layout value.
 * @param text_layout A valid ::ui_text_layout_e member.
 * @return A value for ui_button_t::content_layout.
 */
static inline unsigned int ui_button_layout_from_text_layout(enum ui_text_layout_e text_layout) {
    return ((unsigned int) text_layout) << 22;
}

/**
 * @brief Button widget type.
 */
typedef struct ui_button_s ui_button_t;

/**
 * @brief Button widget struct.
 */
struct ui_button_s {
    /**
     * @brief @x_term{superc}
     */
    ui_component_t component; // 0x0:0x34
    /**
     * @brief Draw the button content.
     */
    void (*on_draw_content)(ui_button_t *self); // 0x34:0x38
    /**
     * @brief Draw the button border.
     */
    void (*on_draw_border)(
        ui_button_t *self,
        short x0,
        short y0,
        short x1,
        short y1,
        unsigned short button_flags,
        unsigned short state
    ); // 0x38:0x3c
    /**
     * @brief Key binding.
     * @details Setting this to 0 disables key binding.
     * @see ui_keycode_e
     */
    int key_binding; // 0x3c:0x40
    /**
     * @brief Custom event to be sent on button press.
     */
    unsigned int event; // 0x40:0x44
    /**
     * @brief Button label text.
     */
    const UTF16 *label; // 0x44:0x48
    /**
     * @brief Human-readable label of key binding (can be `NULL`).
     */
    const char *key_binding_str; // 0x48:0x4c
    /**
     * @brief @x_term{padding}
     */
    int unk_0x4c; // 0x4c:0x50
    /**
     * @brief Visual state.
     * @see ui_button_visual_state_e
     */
    unsigned int next_visual_state; // 0x50:0x54
    /**
     * @brief Text layout of the button label.
     * @details This is most likely not intended to be set by the end-user, as the resulting layouts enabled by values
     * other than the default look broken on the default button style, and both the default constructor and all public
     * methods never allowed the user to set it.
     * @see ui_text_layout_e Raw values for the text layout types.
     * @see ui_button_layout_from_text_layout Convert ::ui_text_layout_e to accepted format.
     */
    unsigned int content_layout; // 0x54:0x58
    /**
     * @brief Number of horizontal padding pixels.
     */
    short horizontal_padding; // 0x58:0x5a
    /**
     * @brief Style flags.
     * @see ui_button_flag_e
     */
    unsigned short button_flags; // 0x5a:0x5c
    /**
     * @brief Reserved for subtype use.
     */
    unsigned int user_data; // 0x5c:0x60
}; // 0x60 bytes

#ifdef __cplusplus
} // extern "C"
#endif

#endif  //  __MUTEKI_UI_TYPES_BUTTON_H__
