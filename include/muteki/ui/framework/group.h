/*
 * Copyright 2024 dogtopus
 * SPDX-License-Identifier: MIT
 */

 /**
  * @file group.h
  * @brief Component group/container methods.
  * @details Methods of a ::ui_group_t but as syscalls.
  */

#ifndef __MUTEKI_UI_FRAMEWORK_GROUP_H__
#define __MUTEKI_UI_FRAMEWORK_GROUP_H__

#include "../types/framework.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Draw the group and its children.
 * @x_syscall_num{0x1016a}
 * @param self The group object.
 * @x_void_return
 */
extern void PGROUP_draw(ui_group_t *self);

/**
 * @brief Handle events.
 * @x_syscall_num{0x1016b}
 * @param self The group object.
 * @param event The incoming event object.
 * @x_void_return
 */
extern void PGROUP_handleEvent(ui_group_t *self, ui_event_t *event);

/**
 * @brief Insert a component to the component ring.
 * @details This method inserts the component to the tail of the component ring relative to ui_group_t::current.
 * @x_syscall_num{0x1016c}
 * @param self The group object.
 * @param child The child component.
 * @x_void_return
 */
extern void PGROUP_insert(ui_group_t *self, ui_component_t *child);

/**
 * @brief Redraw the child components.
 * @x_syscall_num{0x1016d}
 * @param self The group object.
 * @x_void_return
 */
extern void PGROUP_redraw(ui_group_t *self);

/**
 * @brief Set the current component ring head to `child`.
 * @details `child` must already form a ring with other components.
 * @x_syscall_num{0x1016e}
 * @param self The group object.
 * @param child The child component that will become the component ring head.
 */
extern void PGROUP_setCurrent(ui_group_t *self, ui_component_t *child);

/**
 * @brief Operate on the component state bitfield.
 * @details This calls PVIEW_setState() and also calls the virtual method ui_group_t::on_set_current with
 * ui_group_t::current.
 * @x_syscall_num{0x1016f}
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
 * @x_syscall_num{0x10170}
 * @param self The group object.
 * @return Return value of the execution.
 */
extern int PGROUP_execute(ui_group_t *self);

/**
 * @brief Find and return the component that has the ui_group_t::current component as its next component.
 * @x_syscall_num{0x10171}
 * @param self The group object.
 * @return The previous component of the ring head aka. the tail of the ring relative to ui_group_t::current.
 */
extern ui_component_t *PGROUP_preView(ui_group_t *self);

/**
 * @brief Dispose a group component and free up its memory.
 * @details This function sets the exit word to 0, sends itself a destroy event, restores the exit value, and frees up
 * the memory occupied by this component.
 * @x_syscall_num{0x10177}
 * @param group The group object to be freed.
 * @x_void_return
 */
extern void Destroy(ui_group_t *group);

/**
 * @brief Set the group execution result.
 * @x_syscall_num{0x1017b}
 * @param self The group object.
 * @param execution_result The new execution result.
 */
extern void SetDeskBoxReturn(ui_group_t *self, int execution_result);

/**
 * @brief Activates a group.
 * @details This manages the global navigation state and calls the ui_group_t::on_exec callback, which starts a local
 * event loop that routes all events through the group. Returns ui_group_t::execution_result when it is set to a
 * non-zero value or, in case when exit work is permitted, when exit word is set to a non-zero value.
 * @x_syscall_num{0x1017c}
 * @param group The group object.
 * @return The group-defined execution result.
 */
extern int ExecView(ui_group_t *group);

/**
 * @brief Set the exit word.
 * @details Setting this to a non-zero value (usually 1) causes eligible components to exit early after an event
 * handling callback.
 * @x_syscall_num{0x1017d}
 * @param val The new value.
 * @return The old value.
 */
extern int SetExitWordVal(int val);

/**
 * @brief Get the current exit word.
 * @x_syscall_num{0x1017e}
 * @x_void_param
 * @return The exit word value.
 */
extern int GetExitWordVal(void);

/**
 * @brief Remove a child from group.
 * @details Finds and unlinks the specified child component from the group's component ring.
 * @warning This does not free the component.
 * @x_syscall_num{0x10182}
 * @param group The group object.
 * @param child Target child to remove.
 */
extern void Delete(ui_group_t *group, ui_component_t *child);
#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_FRAMEWORK_GROUP_H__
