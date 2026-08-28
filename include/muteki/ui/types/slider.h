/*
 * Copyright 2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file slider.h
 * @brief Slider widget type definition.
 */

#ifndef __MUTEKI_UI_TYPES_SLIDER_H__
#define __MUTEKI_UI_TYPES_SLIDER_H__

#include "framework.h"
#include "../../draw/types/surface.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Style flags for the slider widget.
 * @details The default slider styles.
 * @note These documented behaviors are only true on an unthemed slider widget. Theme modules could ignore or
 * drastically alter the behavior of the flags.
 */
enum ui_slider_flag_e {
    /**
     * @brief Track style: Simple.
     * @details This draws a thin bar filled with foreground color.
     */
    UI_SLIDER_TRACK_SIMPLE = 0x0000,
    /**
     * @brief Track style: Progress-bar-like.
     * @details This draws a thick bar with left portion up to the thumb filled with foreground color, and the right
     * portion filled with background color, like a typical progress bar.
     */
    UI_SLIDER_TRACK_BAR = 0x0001,

    /**
     * @brief Thumb style: Hidden.
     */
    UI_SLIDER_THUMB_NONE = 0x0000,
    /**
     * @brief Thumb style: Bitmap.
     */
    UI_SLIDER_THUMB_BITMAP = 0x0002,
    /**
     * @brief Thumb style: Bitmap (alternate).
     * @details Seems to be exactly the same as ::UI_SLIDER_THUMB_BITMAP, although an external theme could make use of
     * it.
     */
    UI_SLIDER_THUMB_BITMAP_ALT = 0x0004,
    /**
     * @brief Thumb style: Outline.
     * @details Draw a black box over the thumb when it is not in focus. No idea why it is here.
     */
    UI_SLIDER_THUMB_OUTLINE = 0x0006,

    /**
     * @brief Frame style: None.
     * @details Do not draw a frame around the slider.
     */
    UI_SLIDER_FRAME_NONE = 0x0000,
    /**
     * @brief Frame style: Raised.
     * @details Draw a raised 3D-looking box frame around the slider.
     */
    UI_SLIDER_FRAME_RAISED = 0x0020,
    /**
     * @brief Frame style: Sunken.
     * @details Draw a sunken 3D-looking box frame around the slider.
     */
    UI_SLIDER_FRAME_SUNKEN = 0x0040,
};

/**
 * @brief The slider widget type.
 */
typedef struct ui_slider_s ui_slider_t;

/**
 * @brief The slider widget struct.
 */
struct ui_slider_s {
    /**
     * @brief The inherited component struct.
     */
    ui_component_t component;
    /**
     * @brief Set position callback.
     * @see PSLIDER_SetPos
     */
    void (*on_set_position)(ui_slider_t *self, unsigned short position);
    /**
     * @brief Set maximum callback.
     * @see PSLIDER_SetRange
     */
    unsigned short (*on_set_maximum)(ui_slider_t *self, unsigned short maximum);
    /**
     * @brief Callback to convert slider position value to thumb sprite coordinate.
     */
    void (*on_value_to_point)(ui_slider_t *self, unsigned short value, short out_point[2]);
    /**
     * @brief Cached fraction value of (sub)pixels per slider step.
     */
    double pixels_per_step;
    /**
     * @brief Cached thumb background image.
     */
    lcd_surface_t *thumb_bg;
    /**
     * @brief Current position number (borrowed reference).
     */
    unsigned short *position;
    /**
     * @brief Thumb position.
     */
    short last_thumb_center_x;
    /**
     * @brief X coordinate of the start point of a thumb drag.
     */
    unsigned short touch_begin_x;
    /**
     * @brief Unknown. Set to 0.
     */
    int unk_0x54;
    /**
     * @brief Maximum position number of the slider.
     */
    unsigned short maximum;
    /**
     * @brief Minimum position number of the slider.
     */
    unsigned short minimum;
    /**
     * @brief Number of positions to advance on key presses.
     */
    unsigned short step;
    /**
     * @brief Thumb pressing state.
     */
    unsigned short thumb_pressed;
    /**
     * @brief Style flags.
     * @see ui_slider_flag_e
     */
    unsigned short slider_flags;
    /**
     * @brief Unknown. Set to 0.
     */
    unsigned short unk_0x62[3];
};

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // __MUTEKI_UI_TYPES_SLIDER_H__
