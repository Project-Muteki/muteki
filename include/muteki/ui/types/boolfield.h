/*
 * Copyright 2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file boolfield.h
 * @brief Bool field (unlabeled checkbox) and bool text field (labeled checkbox) widget type definition.
 */

#ifndef __MUTEKI_UI_TYPES_BOOLFIELD_H__
#define __MUTEKI_UI_TYPES_BOOLFIELD_H__

#include "framework.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Bool field (unlabeled checkbox) widget type.
 */
typedef struct ui_boolfield_s ui_boolfield_t;

/**
 * @brief Bool text field (labeled checkbox) widget type.
 */
typedef struct ui_booltextfield_s ui_booltextfield_t;

/**
 * @brief Bool field (unlabeled checkbox) widget struct.
 */
struct ui_boolfield_s {
    /**
     * @brief @x_term{superc}
     */
    ui_component_t component;
    /**
     * @brief Not used but set to 1. Likely used by the widget theming module.
     * @todo Actually cross-check with the theme modules.
     */
    unsigned short theme_param;
    /**
     * @brief @x_term{unused}
     */
    short unk_0x36;
    /**
     * @brief Bitmask used to toggle the bits in the value bitfield.
     */
    unsigned short mask;
    /**
     * @brief @x_term{unused}
     */
    short unk_0x3a;
    /**
     * @brief Borrowed value bitfield.
     */
    unsigned short *value;
    /**
     * @brief Text label.
     * @details Optional when used as ui_booltextfield_t::bf. Unused and uninitialized when used as ui_boolfield_t.
     */
    const UTF16 *label;
};

/**
 * @brief Bool text field (labeled checkbox) widget struct.
 */
struct ui_booltextfield_s {
    /**
     * @brief The inherited ui_boolfield_t struct.
     */
    ui_boolfield_t bf;
    /**
     * @brief Text label start X coordinate.
     */
    short label_x0;
    /**
     * @brief Text label end X coordinate.
     */
    short label_x1;
    /**
     * @brief Font type.
     * @details Sets to ::SERIF_CJK by default. May be changed by the user.
     * @see font_type_e
     */
    unsigned short font_type;
    /**
     * @brief @x_term{padding}
     */
    short unk_0x4a;
};

#ifdef __cplusplus
} // extern "C"
#endif

#endif  //  __MUTEKI_UI_TYPES_BOOLFIELD_H__
