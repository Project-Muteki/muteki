/*
 * Copyright 2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file appview.h
 * @brief App view/icon grid widget constructor and methods.
 */

#ifndef __MUTEKI_UI_WIDGETS_APPVIEW_H__
#define __MUTEKI_UI_WIDGETS_APPVIEW_H__

#include "../types/appview.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create an app view/icon grid widget.
 * @x_syscall_num{0x1011e}
 * @param x0 @x_term{x0}
 * @param y0 @x_term{y0}
 * @param x1 @x_term{x1}
 * @param y1 @x_term{y1}
 * @param item_count Total number of entries.
 * @param flags Style flags.
 * @return The created app view object.
 * @see ui_appview_flag_e
 */
extern ui_appview_t *CreateAppView(
    short x0,
    short y0,
    short x1,
    short y1,
    unsigned short item_count,
    unsigned short flags
);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_WIDGETS_APPVIEW_H__
