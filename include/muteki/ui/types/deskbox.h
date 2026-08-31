/*
 * Copyright 2024-2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file deskbox.h
 * @brief Deskbox view type definition.
 */

#ifndef __MUTEKI_UI_TYPES_DESKBOX_H__
#define __MUTEKI_UI_TYPES_DESKBOX_H__

#include "../../loader.h"
#include "framework.h"
#include "imageclip.h"
#include "menu_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Deskbox style flags.
 */
enum ui_deskbox_flag_e {
    /**
     * @brief No flag.
     */
    UI_DESKBOX_FLAG_NONE = 0x0000,
    /**
     * @brief Add a close button to the title bar.
     * @note Some systems may suppress this.
     */
    UI_DESKBOX_FLAG_CLOSE_BUTTON = 0x0001,
    /**
     * @brief Add a done button to the title bar.
     */
    UI_DESKBOX_FLAG_DONE_BUTTON = 0x0002,
    /**
     * @brief Add a save button to the title bar.
     */
    UI_DESKBOX_FLAG_SAVE_BUTTON = 0x0004,
    /**
     * @brief Handle help key presses.
     * @note This does not add a button, instead it adds a hook to the deskbox event handler that opens the help menu.
     */
    UI_DESKBOX_FLAG_HELP_BUTTON = 0x0008,
    /**
     * @brief Add a function button to the title bar.
     */
    UI_DESKBOX_FLAG_FUNCTION_MENU_BUTTON = 0x0010,
    /**
     * @brief Normal style (with title bar).
     */
    UI_DESKBOX_FLAG_STYLE_NORMAL = 0x0000,
    /**
     * @brief Do not draw title bar and frame.
     */
    UI_DESKBOX_FLAG_STYLE_NONE = 0x0100,
    /**
     * @brief Draw a simple frame.
     * @details Implies ::UI_DESKBOX_FLAG_STYLE_NONE.
     */
    UI_DESKBOX_FLAG_STYLE_SIMPLE = 0x0200,
    /**
     * @brief Create a floating window deskbox.
     */
    UI_DESKBOX_FLAG_STYLE_FLOAT = 0x0400,
    /**
     * @brief Create a popup deskbox.
     */
    UI_DESKBOX_FLAG_STYLE_POPUP = 0x0800,
    /**
     * @brief Inhibit close button creation.
     * @details This takes priority over ::UI_DESKBOX_FLAG_CLOSE_BUTTON.
     */
    UI_DESKBOX_FLAG_NO_CLOSE_BUTTON = 0x80000,
};

/**
 * @brief Deskbox container type.
 */
typedef struct ui_deskbox_s ui_deskbox_t;

/**
 * @brief Deskbox container struct.
 */
struct ui_deskbox_s {
    /**
     * @brief Parent group struct.
     */
    ui_group_t group;
    /**
     * @brief Foreground color.
     */
    unsigned int surface_color;
    /**
     * @brief Background color.
     */
    unsigned int background_color;
    /**
     * @brief background image wrapper.
     * @details Set via InsertImageClip(). Hooks into the deskbox drawing routines to repaint background pixels when
     * foreground graphics unload.
     */
    ui_imageclip_t *background_image;
    /**
     * @brief IME control struct.
     * @todo Document the format.
     */
    void *ime_control;
    /**
     * @brief Draw title callback.
     */
    void (*on_draw_title)(ui_deskbox_t *self);
    /**
     * @brief Open help content and index files.
     */
    void (*on_open_help_files)(
        ui_deskbox_t *self,
        loader_file_descriptor_t **out_content,
        loader_file_descriptor_t **out_index
    );
    /**
     * @brief System event callback.
     */
    void (*on_system_event)(ui_component_t *self, ui_event_t *event);
    /**
     * @brief Close previously opened help content and index files.
     */
    void (*on_close_help_files)(
        ui_deskbox_t *self,
        loader_file_descriptor_t **content_file,
        loader_file_descriptor_t **index_file
    );
    /**
     * @brief Title/caption text.
     */
    UTF16 *title;
    /**
     * @brief Function menu entry array.
     */
    const ui_menu_entry_t *menu_entries;
    /**
     * @brief Function menu title text.
     */
    const UTF16 *menu_title_text;
    /**
     * @brief @x_term{padding}
     */
    int unk_0x7c;
    /**
     * @brief Style flags.
     */
    unsigned int deskbox_flags;
    /**
     * @brief Disable mask of command menu entries.
     */
    unsigned int cmdmenu_disable_mask;
    /**
     * @brief Help type index for this deskbox.
     */
    unsigned int help_type;
    /**
     * @brief Left boundary of the titlebar content.
     */
    short title_content_x0;
    /**
     * @brief Right boundary of the titlebar content.
     */
    short title_content_x1;
    /**
     * @brief Reserved for subtype use.
     */
    unsigned int user_data;
};


#ifdef __cplusplus
} // extern "C"
#endif

#endif  // __MUTEKI_UI_TYPES_DESKBOX_H__
