/*
 * Copyright 2024-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file component.h
 * @brief Component (VIEW) methods.
 * @details Methods of a vanilla ::ui_component_t but as syscalls.
 */

#ifndef __MUTEKI_UI_VIEWS_COMPONENT_H__
#define __MUTEKI_UI_VIEWS_COMPONENT_H__

#include <muteki/ui/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Draw a component.
 * @x_syscall_num `0x10166`
 * @param self The component object.
 * @x_void_return
 */
void PVIEW_draw(ui_component_t *self);

/**
 * @brief Signal the parent container to restore pixels that were obstructed by this component.
 * @x_syscall_num `0x10167`
 * @param self The component object.
 * @x_void_return
 */
void PVIEW_EraseBackGround(ui_component_t *self);

/**
 * @brief Handle incoming events.
 * @x_syscall_num `0x10168`
 * @param self The component object.
 * @param e Event.
 * @x_void_return
 */
void PVIEW_handleEvent(ui_component_t *self, ui_event_t *e);

/**
 * @brief Change the object `state` bitmask.
 * @x_syscall_num `0x10169`
 * @param self The component object.
 * @param mask Bits to set/clear.
 * @param set `true` if setting bits, `false` if clearing bits.
 */
void PVIEW_setState(ui_component_t *self, unsigned short mask, bool set);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_VIEWS_COMPONENT_H__
