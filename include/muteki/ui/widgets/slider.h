/*
 * Copyright 2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file slider.h
 * @brief Slider widget constructor and methods.
 */

#ifndef __MUTEKI_UI_WIDGETS_SLIDER_H__
#define __MUTEKI_UI_WIDGETS_SLIDER_H__

#include "../types/slider.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a slider widget.
 * @x_syscall_num{0x10157}
 * @param tag @x_term{tag}
 * @param x0 @x_term{x0}
 * @param y0 @x_term{y0}
 * @param x1 @x_term{x1}
 * @param y1 @x_term{y1}
 * @param minimum Minimum position number of the slider.
 * @param maximum Maximum position number of the slider.
 * @param step Number of positions to advance on key presses.
 * @param[in, out] position Current position number (borrowed reference).
 * @param slider_flags Style flags of the slider.
 * @return The created slider widget object.
 * @see ui_slider_flag_e
 */
extern ui_slider_t *CreateSlider(
    unsigned short tag,
    short x0,
    short y0,
    short x1,
    short y1,
    unsigned short minimum,
    unsigned short maximum,
    unsigned short step,
    unsigned short *position,
    unsigned short slider_flags
);

/**
 * @brief Replace the position number variable used by the slider.
 * @x_syscall_num{0x1015c}
 * @param self The slider widget object.
 * @param[in, out] position Pointer to the new position number variable.
 * @x_void_return
 */
extern void PSLIDER_SetPos(ui_slider_t *self, unsigned short *position);

/**
 * @brief Set the maximum value of the slider.
 * @x_syscall_num{0x1015d}
 * @param self The slider widget object.
 * @param maximum The new maximum value.
 * @return The old maximum value.
 */
unsigned short PSLIDER_SetRange(ui_slider_t *self,unsigned short maximum);

/**
 * @brief Slider event handler callback.
 * @x_syscall_num{0x1015e}
 * @param self The slider widget object.
 * @param event The incoming event.
 * @x_void_return
 */
extern void PSLIDER_handleEvent(ui_slider_t *self, ui_event_t *event);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // __MUTEKI_UI_WIDGETS_SLIDER_H__
