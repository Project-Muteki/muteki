/*
 * Copyright 2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file boolfield.h
 * @brief Bool field (unlabeled checkbox) and bool text field (labeled checkbox) widget constructor and methods.
 * @details
 * Bool fields are checkboxes that are capable of setting and clearing bits in a user-owned 16-bit bitfield. Multiple
 * bits can be set/cleared simultaneously using a bit mask, although the bits must be set/cleared at the same time.
 * For example, if the mask is set to `0x0003`, the bit 0 and bit 1 must both be initialized to `0` or `1`, and both
 * bits must stay equal across the entire lifecycle of the widget. Checkboxes have a fixed size of 16x16px.
 *
 * Bool text fields are labeled checkboxes that are based on the bool field, with the possibility to add a text label
 * at the right side of the checkbox widget itself. The parent bool field is available as ui_booltextfield_t::bf.
 */

#ifndef __MUTEKI_UI_WIDGETS_BOOLFIELD_H__
#define __MUTEKI_UI_WIDGETS_BOOLFIELD_H__

#include "../types/boolfield.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a bool field (unlabeled checkbox) widget.
 * @x_syscall_num{0x10132}
 * @param tag @x_term{tag}
 * @param x @x_term{x0}
 * @param y @x_term{y0}
 * @param value Pointer to a 16-bit value bitfield. Must be valid across widget lifecycle.
 * @param mask Bitmask used to toggle the bits in the value bitfield.
 * @return The created bool field widget object.
 */
extern ui_boolfield_t *CreateBoolField(unsigned short tag, short x, short y, unsigned short *value, unsigned short mask);

/**
 * @brief Create a bool text field (labeled checkbox) widget.
 * @x_syscall_num{0x10133}
 * @param tag @x_term{tag}
 * @param x @x_term{x0}
 * @param y @x_term{y0}
 * @param value Pointer to a 16-bit value bitfield. Must be valid across widget lifecycle.
 * @param mask Bitmask used to toggle the bits in the value bitfield.
 * @param label Text label.
 * @return The created bool text field widget object.
 */
extern ui_booltextfield_t *CreateBoolTextField(
    unsigned short tag,
    short x,
    short y,
    unsigned short *value,
    unsigned short mask,
    const UTF16 *label
);

/**
 * @brief The default draw method.
 * @x_syscall_num{0x1013c}
 * @param self The bool field object.
 * @x_void_return
 */
extern void PBOOLFIELD_draw(ui_boolfield_t *self);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_WIDGETS_BOOLFIELD_H__
