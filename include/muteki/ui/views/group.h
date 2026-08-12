/*
 * Copyright 2024 dogtopus
 * SPDX-License-Identifier: MIT
 */

 /**
  * @file group.h
  * @brief Component group/container methods.
  * @details Methods of a ::ui_group_t but as syscalls.
  */

#ifndef __MUTEKI_UI_VIEWS_GROUP_H__
#define __MUTEKI_UI_VIEWS_GROUP_H__

#include <muteki/ui/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Draw the group and its children.
 * @x_syscall_num `0x1016a`
 * @param self The group object.
 * @x_void_return
 */
extern void PGROUP_draw(ui_group_t *self);

/**
 * @brief Handle events.
 * @x_syscall_num `0x1016b`
 * @param self The group object.
 * @param event The incoming event object.
 * @x_void_return
 */
extern void PGROUP_handleEvent(ui_group_t *self, ui_event_t *event);

/**
 * @brief Insert a component to the component ring.
 * @details This method inserts the component to the tail of the component ring relative to ui_group_t::current.
 * @x_syscall_num `0x1016c`
 * @param self The group object.
 * @param child The child component.
 * @x_void_return
 */
extern void PGROUP_insert(ui_group_t *self, ui_component_t *child);

/**
 * @brief Redraw the child components.
 * @x_syscall_num `0x1016d`
 * @param self The group object.
 * @x_void_return
 */
extern void PGROUP_redraw(ui_group_t *self);

/**
 * @brief Set the current component ring head to `child`.
 * @details `child` must already form a ring with other components.
 * @x_syscall_num `0x1016e`
 * @param self The group object.
 * @param child The child component that will become the component ring head.
 */
extern void PGROUP_setCurrent(ui_group_t *self, ui_component_t *child);

/**
 * @brief Operate on the component state bitfield.
 * @details This calls PVIEW_setState() and also calls the virtual method ui_group_t::on_set_current with
 * ui_group_t::current.
 * @x_syscall_num `0x1016f`
 * @param self The group object.
 * @param mask Bitmask that corresponds bits in the component state.
 * @param set Any state bits that has a corresponding 1 bit in the `bitmask` will be set when this is `true`. If this
 * is `false`, they will be cleared instead.
 * @x_void_return
 */
extern void PGROUP_setState(ui_group_t *self, unsigned short mask, bool set);

/**
 * @brief Execute a group container.
 * @details
 * This starts a local event loop that synchronously checks for new events using GetEvent(), dispatches any received
 * events to its children, and checks the ui_group_t::execution_result for any non-zero return values.
 *
 * When the container allows exit word processing and the exit word has been set to a non-zero value, the execution may
 * terminate early with an execution result of 0.
 * @x_syscall_num `0x10170`
 * @param self The group object.
 * @return Return value of the execution.
 */
extern int PGROUP_execute(ui_group_t *self);

/**
 * @brief Find and return the component that has the ui_group_t::current component as its next component.
 * @x_syscall_num `0x10171`
 * @param self The group object.
 * @return The previous component of the ring head aka. the tail of the ring relative to ui_group_t::current.
 */
extern ui_component_t *PGROUP_preView(ui_group_t *self);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_VIEWS_GROUP_H__
