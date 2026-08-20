/*
 * Copyright 2024-2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file imageclip.h
 * @brief Image clip subwidget type definition.
 */

#ifndef __MUTEKI_UI_TYPES_IMAGECLIP_H__
#define __MUTEKI_UI_TYPES_IMAGECLIP_H__

#include "framework.h"
#include "../../draw/types/surface.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ui_imageclip_s;
typedef struct ui_imageclip_s ui_imageclip_t;

/**
 * @brief Image clip subwidget struct.
 * @details This does not exist independently, rather it is optionally instantiated by the deskbox as a part of it.
 */
struct ui_imageclip_s {
    /**
     * @brief The inherited component struct.
     */
    ui_component_t component;
    /**
     * @brief Replace image callback.
     */
    bool (*on_replace_image)(
        ui_imageclip_t *self,
        unsigned int x,
        unsigned int y,
        lcd_surface_t *surface,
        int free_surface,
        int notify_redraw
    );
    /**
     * @brief Allocated surface that contains the background image.
     */
    lcd_surface_t *surface;
    /**
     * @brief Background image ID if it is a built-in image. Otherwise it should be `-1`.
     */
    int builtin_background_id;
    /**
     * @brief Unknown. Probably padding bytes.
     */
    int unk_0x40;
    /**
     * @brief X offset within the background image.
     */
    short image_offset_x;
    /**
     * @brief Y offset within the background image.
     */
    short image_offset_y;
    /**
     * @brief Non-zero if the image surface is allocated by us.
     */
    unsigned short image_allocated;
    /**
     * @brief Blit mode.
     * @see lcd_surface_blit_flag_e
     */
    unsigned short blit_mode;
    /**
     * @brief Unknown. Seems unused.
     */
    int unk_0x4c[5];
};

#ifdef __cplusplus
} // extern "C"
#endif

#endif  // __MUTEKI_UI_TYPES_IMAGECLIP_H__
