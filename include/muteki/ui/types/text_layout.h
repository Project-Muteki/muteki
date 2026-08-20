/*
 * Copyright 2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file text_layout.h
 * @brief Text layout common constants.
 */

#ifndef __MUTEKI_UI_TYPES_TEXT_LAYOUT_H__
#define __MUTEKI_UI_TYPES_TEXT_LAYOUT_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Text layout types.
 * @details Used by several widgets to indicate the text layout.
 */
enum ui_text_layout_e {
    /**
     * @brief Render the text at the center of the widget.
     */
    UI_TEXT_LAYOUT_CENTER_CENTER = 0,
    /**
     * @brief Render the text at the top-left of the widget.
     */
    UI_TEXT_LAYOUT_LEFT_TOP,
    /**
     * @brief Render the text at the bottom-left of the widget.
     */
    UI_TEXT_LAYOUT_LEFT_BOTTOM,
    /**
     * @brief Render the text at the left of the widget.
     */
    UI_TEXT_LAYOUT_LEFT_CENTER,
    /**
     * @brief Render the text at the top-right of the widget.
     */
    UI_TEXT_LAYOUT_RIGHT_TOP,
    /**
     * @brief Render the text at the bottom-left of the widget.
     */
    UI_TEXT_LAYOUT_RIGHT_BOTTOM,
    /**
     * @brief Render the text at the right of the widget.
     */
    UI_TEXT_LAYOUT_RIGHT_CENTER,
    /**
     * @brief Render the text at the top of the widget.
     */
    UI_TEXT_LAYOUT_CENTER_TOP,
    /**
     * @brief Render the text at the bottom of the widget.
     */
    UI_TEXT_LAYOUT_CENTER_BOTTOM,
};

#ifdef __cplusplus
} // extern "C"
#endif

#endif  //  __MUTEKI_UI_TYPES_TEXT_LAYOUT_H__
