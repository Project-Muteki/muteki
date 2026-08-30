/*
 * Copyright 2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file progressbar.h
 * @brief Progress bar widget constructor and methods.
 */

#ifndef __MUTEKI_UI_WIDGETS_PROGRESSBAR_H__
#define __MUTEKI_UI_WIDGETS_PROGRESSBAR_H__

#include "../types/progressbar.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a progress bar widget.
 * @x_syscall_num{0x10158}
 * @param tag @x_term{tag}
 * @param x @x_term{x0}
 * @param y @x_term{y0}
 * @param width Width of the progress bar.
 * @param height Height of the progress bar.
 * @param maximum Maximum position value of the progress bar.
 * @param segment_count Segment count if the progress bar is in segmented mode.
 * @param flags Style flags.
 * @return The created progress bar widget object.
 * @see ui_progressbar_flags_e
 */
extern ui_progressbar_t *CreateProgress(
    unsigned short tag,
    short x,
    short y,
    short width,
    short height,
    unsigned short maximum,
    unsigned short segment_count,
    unsigned short flags
);

/**
 * @brief Set the progress bar position.
 * @note New values above maximum are rejected rather than clamped.
 * @x_syscall_num{0x1015a}
 * @param self The progress bar object.
 * @param position New position value.
 * @x_void_return
 */
extern void PPROGRESSetPos(ui_progressbar_t *self, unsigned short position);

/**
 * @brief Change the maximum position of the progress bar and refreshes the view.
 * @x_syscall_num{0x1015b}
 * @param self The progress bar object.
 * @param maximum The new maximum position value.
 * @return The old maximum position value.
 */
extern unsigned short PPROGRESSetRange(ui_progressbar_t *self, unsigned short maximum);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // __MUTEKI_UI_WIDGETS_PROGRESSBAR_H__
