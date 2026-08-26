/*
 * Copyright 2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file controlmenu.h
 * @brief Control menu widget type definition.
 */

#ifndef __MUTEKI_UI_TYPES_CONTROLMENU_H__
#define __MUTEKI_UI_TYPES_CONTROLMENU_H__

#include "framework.h"
#include "menu_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Control menu layout type.
 */
enum ui_controlmenu_layout_e {
    /**
     * @brief Absolute X coordinates and widget height.
     * @details Skip X value overrides. The y1 bound will also not be automatically derived from the font height.
     */
    UI_CONTROLMENU_LAYOUT_ABS = 0,

    /**
     * @brief Draw the control menu at the left half of the screen.
     * @details Override x0 as 1 pixel right to the left border of the screen, and x1 as the midpoint of the
     * screen ((GetMaxScrX() + 1) / 2).
     */
    UI_CONTROLMENU_LAYOUT_LEFT_HALF,

    /**
     * @brief Draw the control menu at the right half of the screen with a 2-pixel separation.
     * @details Override x0 as 2 pixels right to the midpoint of the screen, and x1 as 1 pixel left to the right
     * border of the screen.
     */
    UI_CONTROLMENU_LAYOUT_RIGHT_HALF_WITH_SEP,

    /**
     * @brief Fill the screen horizontally.
     * @details Override x0 as 1 pixel right to the left border of the screen, and x1 as 1 pixel left to the right
     * border of the screen.
     */
    UI_CONTROLMENU_LAYOUT_SCREEN_WIDTH,

    /**
     * @brief Draw the control menu at the right half of the screen.
     * @details Override x0 as the midpoint of the screen, and x1 as 1 pixel left to the right border of the screen.
     */
    UI_CONTROLMENU_LAYOUT_RIGHT_HALF,
};

/**
 * @brief Control menu type.
 */
typedef struct ui_controlmenu_s ui_controlmenu_t;

/**
 * @brief Control menu struct.
 */
struct ui_controlmenu_s {
    /**
     * @brief The inherited component struct.
     */
    ui_component_t component;
    /**
     * @brief Layout mode.
     * @see ui_controlmenu_layout_e
     */
    unsigned short layout_mode;
    /**
     * @brief Index of currently selected item.
     */
    unsigned short current_index;
    /**
     * @brief Menu entry array.
     */
    const ui_menu_entry_t *menu_entries;
    /**
     * @brief Title text of the pop-up menu.
     */
    const UTF16 *menu_title_text;
    /**
     * @brief Selection policy.
     * @details Set this to 1 to allow reselection of the currently selected item. Values other than 1 will cause the
     * event handler to filter out the selection event when the currently selected item is reselected.
     */
    unsigned short selection_policy;
    /**
     * @brief Unknown. Probably padding bytes.
     */
    unsigned short unk_0x42;
};

#ifdef __cplusplus
} // extern "C"
#endif

#endif  //  __MUTEKI_UI_TYPES_BUTTON_H__
