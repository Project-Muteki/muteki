/*
 * Copyright 2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file menufield.h
 * @brief Menu field widget API.
 */

#ifndef __MUTEKI_UI_WIDGETS_MENUFIELD_H__
#define __MUTEKI_UI_WIDGETS_MENUFIELD_H__

#include <muteki/ui/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a menu field widget.
 * @x_syscall_num `0x10154`
 * @param tag @x_term tag
 * @param x @x_term x0
 * @param y @x_term y0
 * @param[in, out] index_p Borrowed reference to a 16-bit integer that holds the index of the currently selected item.
 * Must be valid across the entire lifespan of the widget.
 * @param entries Menu entry array.
 * @param flags Menu field flags.
 * @return The created menu field widget.
 * @see ui_menufield_flag_e
 */
ui_menufield_t *CreateMenuField(
    ushort tag,
    short x,
    short y,
    unsigned short *index_p,
    const ui_menu_entry_t *entries,
    unsigned short flags
);

/**
 * @brief Default draw callback of menu field.
 * @x_syscal_num `0x10162`
 * @param self The menu field object.
 * @x_void_return
 */
void PMENUFIELD_draw(ui_menufield_t *self);

/**
 * @brief Default event handler of menu field.
 * @x_syscal_num `0x10163`
 * @param self The menu field object.
 * @param event The incoming event.
 * @x_void_return
 */
void PMENUFIELD_handleEvent(ui_menufield_t *self, ui_event_t *event);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_WIDGETS_MENUFIELD_H__
