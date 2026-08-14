/*
 * Copyright 2024-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file deskbox.h
 * @brief Public method syscall proxies and helper functions related to the deskbox (applet window pane) view.
 */

#ifndef __MUTEKI_UI_VIEWS_DESKBOX_H__
#define __MUTEKI_UI_VIEWS_DESKBOX_H__

#include <muteki/ui/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Assign a background image to the deskbox.
 * @x_syscall_num `0x1014f`
 * @param parent Parent deskbox object to attach a image to.
 * @param tag @x_term tag
 * @param x0 @x_term x0
 * @param y0 @x_term y0
 * @param width Width of the image
 * @param height Height of the image.
 * @param image_x X offset of the image to blit.
 * @param image_y Y offset of the image to blit.
 * @param source_image Surface that contains the image.
 * @param free_source_after_copy Free up the surface after copy. Note that this uses the _lfree() syscall so **do not**
 * use this if you allocated the surface using muteki newlib.
 * @param blit_flag Blit flags.
 * @return The image clip instance created and attached to the deskbox.
 */
ui_imageclip_t *InsertImageClip(
    ui_deskbox_t *parent,
    unsigned short tag,
    short x0,
    short y0,
    short width,
    short height,
    short image_x,
    short image_y,
    lcd_surface_t *source_image,
    bool free_source_after_copy,
    unsigned short blit_flag
);

/**
 * @brief Create a deskbox object.
 * @x_syscall_num `0x10172`
 * @param tag @x_term tag
 * @param x0 @x_term x0
 * @param y0 @x_term y0
 * @param x1 @x_term x1
 * @param y1 @x_term y1
 * @param title Title of the deskbox.
 * @param cmdmenu Array of the command menu entries.
 * @param cmdmenu_title Title text of the command menu.
 * @param flags Style flags specific to deskbox class.
 * @return The allocated deskbox object.
 * @see ui_deskbox_flag_e
 */
extern ui_deskbox_t *CreateDeskBox(
    unsigned short tag,
    short x0,
    short y0,
    short x1,
    short y1,
    UTF16 *title,
    const ui_menu_entry_t *cmdmenu,
    UTF16 *cmdmenu_title,
    unsigned int flags
);

/**
 * @brief Draw the deskbox.
 * @x_syscall_num `0x10173`
 * @param self The deskbox object.
 * @x_void_return
 */
extern void PDESKBOX_draw(ui_deskbox_t *self);

/**
 * @brief Draw the title of the deskbox.
 * @x_syscall_num `0x10174`
 * @param self The deskbox object.
 * @x_void_return
 */
extern void PDESKBOX_drawTitle(ui_deskbox_t *self);

/**
 * @brief Default event handler of the deskbox.
 * @x_syscall_num `0x10175`
 * @param self The deskbox object.
 * @param event The event.
 * @x_void_return
 */
extern void PDESKBOX_handleEvent(ui_deskbox_t *self, ui_event_t *event);

/**
 * @brief Redraw the deskbox.
 * @x_syscall_num `0x10176`
 * @param self The deskbox object.
 * @x_void_return
 */
extern void PDESKBOX_redraw(ui_deskbox_t *self);

/**
 * @brief Populate a deskbox object in-place.
 * @details This is used internally inside CreateDeskBox() and is normally not needed.
 * @x_syscall_num `0x10178`
 * @param self Unpopulated deskbox object.
 * @return The same object.
 */
extern ui_deskbox_t *DeskBox_construct(ui_deskbox_t *self);

/**
 * @brief Helper to render the borders of a deskbox.
 * @details This is used internally inside PDESKBOX_draw() and is normally not needed.
 * @x_syscall_num `0x10179`
 * @param x0 @x_term x0
 * @param y0 @x_term y0
 * @param x1 @x_term x1
 * @param y1 @x_term y1
 * @param deskbox_flags 
 * @x_void_return
 * @see ui_deskbox_flag_e
 */
extern void DrawDeskBoxBound(
    short x0,
    short y0,
    short x1,
    short y1,
    unsigned int deskbox_flags
);

/**
 * @brief Update the command menu on a deskbox.
 * @x_syscall_num `0x1017a`
 * @param deskbox The parent deskbox to attach the menu to.
 * @param cmdmenu Array of the new command menu entries.
 * @x_void_return
 */
extern void ChangeCommandMenu(ui_deskbox_t *deskbox, const ui_menu_entry_t *cmdmenu);

/**
 * @brief Convenience method to add the function button to the title bar of a deskbox.
 * @x_syscall_num `0x10181`
 * @param self The parent deskbox to attach the button to.
 * @param tag @x_term tag
 * @param label Text label on the button.
 * @param event Event value assigned to the button.
 * @return The resulting button object.
 */
extern ui_button_t *InsertTitleBarButton(ui_deskbox_t *self, unsigned short tag, UTF16 *label, unsigned int event);

/**
 * @brief Get the user-drawable area on a deskbox.
 * @x_syscall_num `0x10192`
 * @param[out] rect Result rectangle.
 * @param[in] from The deskbox object.
 * @x_void_return
 */
extern void GetDeskClientRect(ui_rect_t *rect, ui_deskbox_t *from);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_VIEWS_DESKBOX_H__
