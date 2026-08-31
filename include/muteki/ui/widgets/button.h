/*
 * Copyright 2024-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file button.h
 * @brief Button widget constructor and methods.
 */

#ifndef __MUTEKI_UI_WIDGETS_BUTTON_H__
#define __MUTEKI_UI_WIDGETS_BUTTON_H__

#include "../types/button.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Bind a button to a keyboard key.
 * @x_syscall_num{0x10147}
 * @param button The button object.
 * @param keycode Keycode.
 * @x_void_return
 * @see ui_keycode_e
 */
extern void RelatedKeyButton(ui_button_t *button, int keycode);

/**
 * @brief Bind a button to a keyboard key and attach the prompt string for that key.
 * @details The default draw callback will draw the key prompt string before the label string (e.g. `F1 Press Me`)
 * @x_syscall_num{0x10148}
 * @param button The button object.
 * @param keycode Keycode.
 * @x_void_return
 * @see ui_keycode_e
 * @see ui_button_s::key_binding_str
 */
extern void RelatedKeyButtonEx(ui_button_t *button, int keycode);

/**
 * @brief Remove existing key binding on a button.
 * @x_syscall_num{0x10149}
 * @param button The button object.
 * @x_void_return
 */
extern void UnRelatedKeyButton(ui_button_t *button);

/**
 * @brief Create a button widget object.
 * @todo Test the event behavior. This needs more reverse engineering on the view event handling process.
 * @todo Figure out the flags.
 * @x_syscall_num{0x1014a}
 * @param tag @x_term{tag}
 * @param x0 @x_term{x0}
 * @param y0 @x_term{y0}
 * @param x1 @x_term{x1-auto}
 * @param y1 @x_term{y1-auto}
 * @param label UTF-16 encoded string to be used as a label.
 * @param event Custom event to be sent on button press.
 * @param flags Flags. Valid options seem to be 2, 4, 0x40.
 * @return The allocated object.
 */
extern ui_button_t *CreateButton(
    unsigned short tag,
    short x0,
    short y0,
    short x1,
    short y1,
    const UTF16 *label,
    unsigned int event,
    unsigned short flags
);

/**
 * @brief Modify and redraw the button.
 * @x_syscall_num{0x1014b}
 * @param button The button object.
 * @param label New UTF-16 encoded string to be used as a label. Will be skipped if set to `NULL`.
 * @param event New custom event to be sent on button press. Will be skipped if set to `0`.
 * @x_void_return
 */
extern void ChangeButton(ui_button_t *button, const UTF16 *label, unsigned int event);

/**
 * @brief Default draw callback of the button widget.
 * @x_syscall_num{0x1014e}
 * @param self 
 */
extern void PBUTTON_draw(ui_button_t *self);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_WIDGETS_BUTTON_H__
