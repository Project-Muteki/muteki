/*
 * Copyright 2024-2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file menu_common.h
 * @brief Menu type definitions that are shared among several menu implementations.
 */


#ifndef __MUTEKI_UI_TYPES_MENU_COMMON_H__
#define __MUTEKI_UI_TYPES_MENU_COMMON_H__

#include "../../common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Menu entry attribute flags.
 */
enum ui_menu_entry_attribute_e {
    /**
     * @brief Put separator after current item.
     */
    UI_MENU_ENTRY_ATTR_SEPARATOR_AFTER = 0x0001,
    /**
     * @brief Render the widget label using bold font.
     * @details Use ::SERIF_BOLD_CJK in place of the usual ::SERIF_CJK font when rendering the label.
     * @note This attribute only takes effect when the user of the entry item is a ::ui_controlmenu_t. It will be
     * ignored otherwise.
     */
    UI_CONTROLMENU_ENTRY_ATTR_BOLD_FONT = 0x0002,
    /**
     * @brief Put separator before current item.
     */
    UI_MENU_ENTRY_ATTR_SEPARATOR_BEFORE = 0x0004,
    /**
     * @brief Item can be selected as a control menu value.
     * @details Only items with this flag can be selected as a value. Otherwise selecting the item will only emit
     * an event.
     * @note This attribute only takes effect when the user of the entry item is a ::ui_controlmenu_t. It will be
     * ignored otherwise.
     */
    UI_CONTROLMENU_ENTRY_ATTR_VALUE_ITEM = 0x0080,
    /**
     * @brief Label uses 8-bit encoding with codepage CP950.
     */
    UI_MENU_ENTRY_ATTR_ENCODING_CP950 = 0x0100,
    /**
     * @brief Label uses 8-bit encoding with codepage CP936.
     */
    UI_MENU_ENTRY_ATTR_ENCODING_CP936 = 0x0200,
    /**
     * @brief Label uses 8-bit encoding with codepage CP949.
     */
    UI_MENU_ENTRY_ATTR_ENCODING_CP949 = 0x0400,
    /**
     * @brief Default selection.
     * @details Make this item (or the last item that had this bit set) the default selection.
     * Prioritized over the default item selected by PopUpList().
     */
    UI_MENU_ENTRY_ATTR_DEFAULT = 0x1000,
};

/**
 * @brief End of menu entry record.
 */
#define UI_MENU_ENTRY_END { .label_wide = _BUL(""), .event = 0, .attributes = 0 }

/**
 * @brief Menu entry struct used by various menu widgets.
 * @details
 * Normally should be used in an empty-record-terminated array. For example:
 *
 * @code{.c}
 * const ui_menu_entry_t MENU[] = {
 *     { .label_wide = _BUL("Item"), .event = 0x10000, .attributes = 0 },
 *     { .label_wide = _BUL(""), .event = 0, .attributes = 0 },  // or UI_MENU_ENTRY_END
 * };
 * @endcode
 *
 * ui_menu_entry_s::attributes defines extra attributes that apply to the menu entries. Some attributes are exclusive
 * to certain subtype of menu widgets.
 */
typedef struct ui_menu_entry_s {
    union {
        /**
         * @brief UTF-16-encoded label text.
         */
        const UTF16 label_wide[24];
        /**
         * @brief 8-bit-encoded label text.
         */
        const char label_narrow[48];
    };
    /**
     * @brief Event value emitted when selected.
     */
    unsigned int event;
    /**
     * @brief Attributes.
     * @see ui_menu_entry_attribute_e
     */
    unsigned short attributes;
    /**
     * @brief Unknown. Probably padding bytes.
     */
    short unk_0x36;
} ui_menu_entry_t;

#ifdef __cplusplus
} // extern "C"
#endif

#endif  // __MUTEKI_UI_TYPES_MENU_COMMON_H__
