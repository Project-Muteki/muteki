/*
 * Copyright 2024-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file button.h
 * @brief Button widget API.
 */

#ifndef __MUTEKI_UI_WIDGETS_BUTTON_H__
#define __MUTEKI_UI_WIDGETS_BUTTON_H__

#include "../types/button.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a button widget object.
 * @todo Test the event behavior. This needs more reverse engineering on the view event handling process.
 * @todo Figure out the flags.
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
ui_button_t *CreateButton(
    unsigned short tag,
    short x0,
    short y0,
    short x1,
    short y1,
    UTF16 *label,
    unsigned int event,
    unsigned short flags
);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_WIDGETS_BUTTON_H__
