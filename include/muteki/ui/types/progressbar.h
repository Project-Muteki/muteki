/*
 * Copyright 2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file progressbar.h
 * @brief Progress bar widget type definition.
 */

#ifndef __MUTEKI_UI_TYPES_PROGRESSBAR_H__
#define __MUTEKI_UI_TYPES_PROGRESSBAR_H__

#include "framework.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Progress bar style flags.
 */
enum ui_progressbar_flag_e {
    /**
     * @brief Default look (flat, no percent label).
     */
    UI_PROGRESSBAR_FLAG_NONE = 0x0000,
    /**
     * @brief Add a percent label in the middle of the progress bar.
     */
    UI_PROGRESSBAR_FLAG_SHOW_PERCENT = 0x0001,
    /**
     * @brief Segmented mode.
     */
    UI_PROGRESSBAR_FLAG_SEGMENTED = 0x0004,
    /**
     * @brief Add a gradient texture (white -> widget fg color) to the progress bar.
     * @note This does nothing if the progress bar is in segmented mode.
     */
    UI_PROGRESSBAR_FLAG_GRADIENT = 0x0010,
};

/**
 * @brief Progress bar widget type.
 */
typedef struct ui_progressbar_s ui_progressbar_t;

/**
 * @brief Progress bar widget struct.
 */
struct ui_progressbar_s {
    /**
     * @brief The inherited component struct.
     */
    ui_component_t component;
    /**
     * @brief Set position callback.
     * @see PPROGRESSetPos
     */
    void (*on_set_position)(ui_progressbar_t *self, unsigned short position);
    /**
     * @brief Set maximum position callback.
     * @see PPROGRESSetRange
     */
    unsigned short (*on_set_maximum)(ui_progressbar_t *self, unsigned short maximum);
    /**
     * @brief Maximum position.
     */
    unsigned short maximum;
    /**
     * @brief Current position.
     */
    unsigned short position;
    /**
     * @brief Segment count if the progress bar is in segmented mode.
     */
    unsigned short segment_count;
    /**
     * @brief Progress bar style flags.
     * @see ui_progressbar_flag_e
     */
    unsigned short progressbar_flags;
};

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // __MUTEKI_UI_TYPES_PROGRESSBAR_H__
