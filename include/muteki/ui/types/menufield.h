/*
 * Copyright 2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file menufield.h
 * @brief Menu field widget type definition.
 */

#ifndef __MUTEKI_UI_TYPES_MENUFIELD_H__
#define __MUTEKI_UI_TYPES_MENUFIELD_H__

#include "framework.h"
#include "menu_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Menu field flags.
 */
enum ui_menufield_flag_e {
    /**
     * @brief Notify others of a new selection.
     * @details When this flag is set, produce an event with the type ::UI_EVENT_TYPE_COMPONENT_CMD, the value
     * set to the value of the selected ui_menu_entry_t::event, and user data set to the index of current selection
     * (as ui_event_t::user_data_scalar).
     */
    UI_MENUFIELD_FLAG_NOTIFY = 0x0001,
    /**
     * @brief Set by the file picker user. Purpose unclear.
     */
    UI_MENUFIELD_FILEPICKER = 0x0020,
};

/**
 * @brief Menu field type.
 */
typedef struct ui_menufield_s ui_menufield_t;

/**
 * @brief Menu field struct.
 */
struct ui_menufield_s {
    /**
     * @brief The inherited component struct.
     */
    ui_component_t component;
    /**
     * @brief Not used. Likely used by the widget theming module.
     */
    unsigned short theme_param;
    /**
     * @brief Unknown. Probably padding bytes.
     */
    short unk_0x36;
    /**
     * @brief Post update callback.
     * @details Called whenever there is a new valid menu item selection.
     */
    void (*post_update)(ui_menufield_t *self);
    /**
     * @brief Menu entry array.
     */
    const ui_menu_entry_t *menu_entries;
    /**
     * @brief Borrowed reference to the memory storing the current index.
     * @warning This must be a valid pointer to a 16-bit integer at all times the widget is alive.
     */
    unsigned short *current_index_p;
    /**
     * @brief Maximum text area x1 coordinate.
     * @details Set by the constructor. Normally should not be overridden by the user.
     */
    short max_x1;
    /**
     * @brief Flags.
     * @see ui_menufield_flag_e
     */
    unsigned short menufield_flags;
    /**
     * @brief Unknown.
     */
    int unk_0x48;
};

#ifdef __cplusplus
} // extern "C"
#endif

#endif  //  __MUTEKI_UI_TYPES_MENUFIELD_H__
