/*
 * Copyright 2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file controlmenu.h
 * @brief Control menu widget constructor and methods.
 */

#ifndef __MUTEKI_UI_WIDGETS_CONTROLMENU_H__
#define __MUTEKI_UI_WIDGETS_CONTROLMENU_H__

#include "../types/controlmenu.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a control menu widget.
 * @x_syscall_num{0x10153}
 * @param tag @x_term{tag}
 * @param x0 @x_term{x0}
 * @param y0 @x_term{y0}
 * @param x1 @x_term{x1}
 * @param y1 @x_term{y1}
 * @param entries Menu entry array.
 * @param title Title of the pop-up menu.
 * @param default_index Default selected item index.
 * @param layout_mode Layout mode.
 * @param selection_policy Selection policy.
 * @return The created control menu widget.
 */
ui_controlmenu_t *CreateControlMenu(
    unsigned short tag,
    short x0,
    short y0,
    short x1,
    short y1,
    const ui_menu_entry_t *entries,
    const UTF16 *title,
    unsigned short default_index,
    int layout_mode,
    unsigned short selection_policy
);

/**
 * @brief Default draw callback of control menu.
 * @x_syscall_num{0x10164}
 * @param self The control menu object.
 * @x_void_return
 */
void PCONTROLMENU_draw(ui_controlmenu_t *self);

/**
 * @brief Default event handler of control menu.
 * @x_syscall_num{0x10165}
 * @param self The control menu object.
 * @param event The incoming event.
 * @x_void_return
 */
void PCONTROLMENU_handleEvent(ui_controlmenu_t *self,ui_event_t *event);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_WIDGETS_CONTROLMENU_H__
