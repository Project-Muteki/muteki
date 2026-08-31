/*
 * Copyright 2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file appview.h
 * @brief App view/icon grid widget type definition.
 */

#ifndef __MUTEKI_UI_TYPES_APPVIEW_H__
#define __MUTEKI_UI_TYPES_APPVIEW_H__

#include "framework.h"
#include "../../draw/types/surface.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief App view flags.
 * @note LAYOUT flags are mutually exclusively and cannot be stacked.
 */
enum ui_appview_flag_e {
    /**
     * @brief Use the 1x12 list layout.
     */
    UI_APPVIEW_LAYOUT_DEFAULT = 0x0000,
    /**
     * @brief Use the 4x3 tile grid layout (home-menu-like).
     */
    UI_APPVIEW_LAYOUT_TILE_GRID = 0x0001,
    /**
     * @brief Use compact 2x6 column-major list layout.
     */
    UI_APPVIEW_LAYOUT_COMPACT_COLUMNS = 0x0002,
    /**
     * @brief Use 2x6 column-major list layout with icons.
     */
    UI_APPVIEW_LAYOUT_ICON_COLUMNS = 0x0008,
    /**
     * @brief Do not draw the shortcut labels.
     */
    UI_APPVIEW_MOD_HIDE_SHORTCUT_LABELS = 0x2000,
    /**
     * @brief Draw grid lines.
     */
    UI_APPVIEW_MOD_DRAW_GRID_LINES = 0x4000,
    /**
     * @brief Emit event to the parent deskbox instead of exiting the parent deskbox.
     */
    UI_APPVIEW_EMIT_EVENT_ON_SELECT = 0x8000,
};

/**
 * @brief App view item entry type.
 */
typedef struct ui_appview_item_s {
    /**
     * @brief Item ID.
     * @details This value will either be returned as the exit value of the parent deskbox, or as the value of an
     * event, depending on the app view flags.
     */
    int item_id;
    /**
     * @brief Optional icon.
     */
    lcd_surface_t *icon;
    /**
     * @brief @x_term{unused}
     */
    int unk_0x08;
    /**
     * @brief UTF-16 encoded text label of the icon.
     */
    const UTF16 *label;
    /**
     * @brief @x_term{unused}
     */
    char unk_0x10[0x10];
} ui_appview_item_t;

/**
 * @brief App view type.
 */
typedef struct ui_appview_s ui_appview_t;

/**
 * @brief App view struct.
 */
struct ui_appview_s {
    /**
     * @brief @x_term{superc}
     */
    ui_component_t component;
    /**
     * @brief Total number of entries. Used for paging.
     */
    unsigned short item_count;
    /**
     * @brief Absolute index of the first entry on the current page.
     */
    unsigned short first_visible_item;
    /**
     * @brief Current selection on the page.
     * @details Set to `-1` when nothing is selected.
     */
    short selected_page_slot;
    /**
     * @brief @x_term{unused}
     */
    short unk_0x3a;
    /**
     * @brief Computed width of the item (including the label and the icon).
     */
    short item_width;
    /**
     * @brief Computed height of the item (including the label and the icon).
     */
    short item_height;
    /**
     * @brief Computed width of specifically the icon.
     */
    unsigned short icon_width;
    /**
     * @brief Computed height of specifically the icon.
     */
    unsigned short icon_height;
    /**
     * @brief Computed horizontal gap size.
     */
    short horizontal_gap;
    /**
     * @brief Computed vertical gap size.
     */
    short vertical_gap;
    /**
     * @brief Computed number of columns.
     */
    unsigned short column_count;
    /**
     * @brief Computed page capacity (max. 12).
     */
    unsigned short page_capacity;
    /**
     * @brief Pointer to array of page items.
     */
    ui_appview_item_t *page_items;   /* +0x4c: owned page_capacity-entry cache */
    /**
     * @brief Whether to refresh the page before draw/input.
     */
    unsigned short reload_pending;
    /**
     * @brief Flags.
     * @see ui_appview_flag_e
     */
    unsigned short appview_flags;
    /**
     * @brief Font type.
     * @see font_type_e
     */
    unsigned short font_type;
    /**
     * @brief @x_term{unused}
     */
    short unk_0x56;
    /**
     * @brief Relayout callback.
     * @x_void_return
     */
    void (*on_relayout)(ui_appview_t *self);
    /**
     * @brief Load page callback.
     * @details Override this to populate the page.
     * @x_void_return
     */
    void (*on_load_page)(ui_appview_t *self);
    /**
     * @brief Draw item callback.
     * @param x @x_term{x0}
     * @param y @x_term{y0}
     * @param page_slot Relative item number on the page.
     * @x_void_return
     */
    void (*on_draw_item)(ui_appview_t *self, short x, short y, unsigned short page_slot);
    /**
     * @brief Draw icon callback.
     * @param x0 @x_term{x0}
     * @param y0 @x_term{y0}
     * @param x1 @x_term{x1}
     * @param y1 @x_term{y1}
     * @param page_slot Relative item number on the page.
     */
    void (*on_draw_icon)(
        ui_appview_t *self,
        short x0,
        short y0,
        short x1,
        short y1,
        unsigned short page_slot
    );
    /**
     * @brief Draw label callback.
     * @param x0 @x_term{x0}
     * @param y0 @x_term{y0}
     * @param x1 @x_term{x1}
     * @param y1 @x_term{y1}
     * @param page_slot Relative item number on the page.
     */
    void (*on_draw_label)(
        ui_appview_t *self,
        short x0,
        short y0,
        short x1,
        short y1,
        unsigned short page_slot
    );
};

#ifdef __cplusplus
} // extern "C"
#endif

#endif  //  __MUTEKI_UI_TYPES_APPVIEW_H__
