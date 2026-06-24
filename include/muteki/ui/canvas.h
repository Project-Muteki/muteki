/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file canvas.h
 * @brief Display canvas (LCD) API.
 * @details Note that the term "display canvas", "canvas" and "LCD" may be used interchangeably below. This may get
 * rectified in a future release.
 */

#ifndef __MUTEKI_UI_CANVAS_H__
#define __MUTEKI_UI_CANVAS_H__

#include <muteki/ui/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get the current font type.
 * @x_syscall_num `0x1004f`
 * @x_void_param
 * @return The current font type.
 * @see font_type_e Valid values for `font_type`.
 */
extern short GetFontType(void);

/**
 * @brief Set the current font type to `font_type`.
 * @x_syscall_num `0x10051`
 * @param font_type The font type.
 * @x_void_return
 * @see font_type_e Valid values for `font_type`.
 */
extern void SetFontType(short font_type);

/**
 * @brief Draw a string `s` with specified alignment at `(x, y)` px.
 * @details
 * The anchor point will be located at the top of the string to be drawn. The `align` parameter then controls whether
 * the anchor point will be at the left, center or right of the string.
 *
 * If the string is too long, it will be truncated to `max_length`. After truncation, the last few characters may get
 * replaced with `...` depending on the font selection.
 *
 * @x_syscall_num `0x10052`
 * @param x X coordinate of the anchor point.
 * @param y Y coordinate of the anchor point.
 * @param s String to be drawn. Exact encoding depends on the process flags being used.
 * @param max_length The maximum length of the string in pixels.
 * @param align String alignment.
 * @param flags String processing flags
 * @see print_flag_e Valid process flags.
 * @see print_str_flag_e Encoding conversion flags.
 */
extern void WriteAlignString(short x, short y, const void *s, short max_length, int align, unsigned int flags);

/**
 * @brief Draw a UTF-16 character `c` aligned to the top left corner at `(x, y)` px.
 * @x_syscall_num `0x10053`
 * @param x X coordinate of the corner.
 * @param y Y coordinate of the corner.
 * @param c UTF-16 codepoint of the character.
 * @param flags Process flags.
 * @x_void_return
 * @see print_flag_e Valid process flags.
 */
extern void WriteChar(short x, short y, UTF16 c, unsigned int flags);

/**
 * @brief Draw a string `s` aligned to the top left corner at `(x, y)` px.
 * @x_syscall_num `0x10054`
 * @param x X coordinate of the corner.
 * @param y Y coordinate of the corner.
 * @param[in] s String to be drawn. Exact encoding depends on the process flags being used.
 * @param flags Process flags.
 * @x_void_return
 * @see WriteChar Similar function that displays single characters instead.
 * @see print_flag_e Valid process flags.
 * @see print_str_flag_e Encoding conversion flags.
 */
extern void WriteString(short x, short y, const void *s, unsigned int flags);

/**
 * @brief Format and draw a string.
 * @details Handles line wraps and screen scrolling automatically.
 * @x_syscall_num `0x10057`
 * @param format The format string.
 * @param ... Any subsequent values.
 * @x_void_return
 */
extern void Printf(const char *format, ...) SYS_IS_PRINTF(1, 2);

/**
 * @brief Format and draw a string aligned to the top left corner at `(x, y)` px.
 * @x_syscall_num `0x10058`
 * @param x X coordinate of the corner.
 * @param y Y coordinate of the corner.
 * @param format The format string passed to the built-in sprintf().
 * @param ... Any subsequent values passed to the built-in sprintf().
 * @x_void_return
 */
extern void PrintfXY(short x, short y, const char *format, ...) SYS_IS_PRINTF(3, 4);

/**
 * @brief Draw a surface onto current active LCD.
 * @details If `surface` is set to `NULL` this will fail gracefully.
 * @x_syscall_num `0x10059`
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param surface The surface descriptor.
 * @param flags Processing flags.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 * @see blit_flag_e Accepted processing flags.
 * @see PutImage A simplified version of this function.
 */
extern int ShowGraphic(short x, short y, lcd_surface_t *surface, unsigned short flags);

/**
 * @brief Set the size of the cursor on the current canvas.
 * @x_syscall_num `0x1005d`
 * @param new_size The new cursor size.
 * @return The previous cursor size.
 */
extern unsigned int SetCursorSize(unsigned int new_size);

/**
 * @brief Get the size of the cursor on the current canvas.
 * @x_syscall_num `0x1005e`
 * @x_void_param
 * @return The cursor size.
 */
extern unsigned int GetCursorSize(void);

/**
 * @brief Move the cursor position on the current canvas to the specified coordinate.
 * @x_syscall_num `0x1005f`
 * @param x The new x coordinate of the cursor.
 * @param y The new y coordinate of the cursor.
 * @x_void_return
 */
extern void SetCursorPosition(short x, short y);

/**
 * @brief Get the cursor position on the current canvas.
 * @x_syscall_num `0x10060`
 * @param[out] x The x coordinate of the cursor.
 * @param[out] y The y coordinate of the cursor.
 * @x_void_return
 */
extern void GetCursorPosition(short *x, short *y);

/**
 * @brief Set the type of the cursor on the current canvas.
 * @todo Document the actual type as an enum.
 * @x_syscall_num `0x10061`
 * @param new_type The new cursor type.
 * @return The previous cursor type.
 */
extern short SetCursorType(short new_type);

/**
 * @brief Get the type of the cursor on the current canvas.
 * @todo Document the actual type as an enum.
 * @x_syscall_num `0x10062`
 * @x_void_param
 * @return The cursor type.
 */
extern short GetCursorType(void);

/**
 * @brief Lock the cursor on the current canvas.
 * @x_syscall_num `0x10063`
 * @x_void
 */
extern void CursorLock(void);

/**
 * @brief Unlock the cursor on the current canvas.
 * @x_syscall_num `0x10064`
 * @x_void
 */
extern void CursorUnlock(void);

/**
 * @brief Set display canvas transparent color.
 * @details This color will be removed from the image and replaced with transparency effect.
 * @x_syscall_num `0x10065`
 * @param color The integer RGB value representing a new color.
 * @return The integer RGB value representing the old color.
 */
extern int SetTransparentColor(int color);

/**
 * @brief Set display canvas background fill color.
 * @x_syscall_num `0x10067`
 * @param color The integer RGB value representing a new color.
 * @return The integer RGB value representing the old color.
 */
extern int rgbSetBkColor(int color);

/**
 * @brief Set display canvas foreground fill color.
 * @x_syscall_num `0x10068`
 * @param color The integer RGB value representing a new color.
 * @return The integer RGB value representing the old color.
 */
extern int rgbSetColor(int color);

/**
 * @brief Set the stroke dash pattern.
 * @details The dash bit patterns are simple 32-bit bitfields (in **big endian**). Each bit `1` represents a foreground
 * colored pixel and bit `0` represents a background pixel, from left to right, MSB to LSB. For example,
 * stroke-dasharray `1 7 4 4 2 6 4 4` can be encoded as `10000000111100001100000011110000`. With endian swapped, the final
 * `dash_pattern` would be `0xf0c0f080`.
 * @note Stroke dash pattern set via this function does not apply to curves.
 * @x_syscall_num `0x1006b`
 * @param dash New predefined dash pattern. Set to ::DASH_CUSTOM to specify a custom pattern.
 * @param[in, out] dash_pattern New dash bit pattern. Will be set to the previous dash bit pattern once this function
 * returns.
 * @return Previous predefined dash pattern.
 */
short SetPenStyle(short dash, unsigned int *dash_pattern);

/**
 * @brief Get the stroke dash pattern.
 * @x_syscall_num `0x1006c`
 * @param[out] dash_pattern If not `NULL`, the current dash bit pattern.
 * @return The current predefined dash pattern.
 */
short GetPenStyle(unsigned int *dash_pattern);

/**
 * @brief Get the stroke width.
 * @x_syscall_num `0x1006d`
 * @x_void_param
 * @return Current stroke width in pixels.
 */
short GetPenSize(void);

/**
 * @brief Set the stroke width.
 * @x_syscall_num `0x1006e`
 * @param size New stroke width in pixels.
 * @return Previous stroke width in pixels.
 */
short SetPenSize(short size);

/**
 * @brief Get the RGB value of a pixel on the current active surface.
 * @warning This does not check for out-of-bound access.
 * @x_syscall_num `0x1006f`
 * @param x X coordinate.
 * @param y Y coordinate.
 * @return The RGB color value
 */
int GetPixel(short x, short y);

/**
 * @brief Set the pixel color on the current active surface with the given pixel.
 * @details When the surface uses indexed color, the color that is the closest to the palette will be picked.
 * @warning This does not check for out-of-bound access.
 * @x_syscall_num `0x10070`
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param color The RGB color value.
 * @return The actual RGB color value being used.
 */
int SetPixel(short x, short y, int color);

/**
 * @brief Copy a rectangle of pixels from current active LCD and save it to the specified buffer.
 * @details This is similar to using _BitBlt() on the active LCD surface but with a seemingly different implementation
 * and a different function signature. Surface initialization is done implicitly by this function although calling
 * GetImageSize() in user code is still required in order to allocate a buffer of appropriate size.
 * @x_syscall_num `0x10071`
 * @param x0 @x_term x0
 * @param y0 @x_term y0
 * @param x1 @x_term x1
 * @param y1 @x_term y1
 * @param surface Target buffer that will hold the surface.
 * @return Size of the surface.
 */
extern size_t GetImage(short x0, short y0, short x1, short y1, lcd_surface_t *surface);

/**
 * @brief Copy pixels from a surface to current active LCD.
 * @details This is very similar to ShowGraphic() except that it does not check whether or not `surface` is `NULL` and
 * there is no indication of whether or not the process was completed successfully.
 * @x_syscall_num `0x10072`
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param surface The surface descriptor.
 * @param flags Processing flags.
 * @x_void_return
 * @see blit_flag_e Accepted processing flags.
 * @see ShowGraphic A function that pretty much does the same thing but with `NULL` check and a return value.
 */
extern void PutImage(short x, short y, lcd_surface_t *surface, unsigned short flags);

/**
 * @brief Set the drawing area of the current active LCD.
 * @details The new values will be rejected if they are out of bounds (specifically when `x0` or `x1` are negative or
 * `x1` or `y1` are less than `x0` and `y0` respectively), although `x1` and `y1` will be automatically capped at the
 * *physical* display size if they are larger than that instead of failing outright.
 * @x_syscall_num `0x10073`
 * @param x0 @x_term x0
 * @param y0 @x_term y0
 * @param x1 @x_term x1
 * @param y1 @x_term y1
 * @x_void_return
 */
extern void SetDrawArea(short x0, short y0, short x1, short y1);

/**
 * @brief Get the drawing area of the current active LCD.
 * @x_syscall_num `0x10074`
 * @param[out] x0 @x_term x0
 * @param[out] y0 @x_term y0
 * @param[out] x1 @x_term x1
 * @param[out] y1 @x_term y1
 * @x_void_return
 */
extern void GetDrawArea(short *x0, short *y0, short *x1, short *y1);

/**
 * @brief Stroke a line on the current active LCD.
 * @x_syscall_num `0x10075`
 * @param x0 X coordinate of the first point.
 * @param y0 Y coordinate of the first point.
 * @param x1 X coordinate of the second point.
 * @param y1 Y coordinate of the second point.
 * @param flags Blit processing flags.
 * @x_void_return
 */
extern void DrawLine(short x0, short y0, short x1, short y1, unsigned int flags);

/**
 * @brief Stroke a rectangle on the current active LCD.
 * @x_syscall_num `0x10076`
 * @param x0 @x_term x0
 * @param y0 @x_term y0
 * @param x1 @x_term x1
 * @param y1 @x_term y1
 * @param flags Blit processing flags.
 * @x_void_return
 */
extern void DrawRect(short x0, short y0, short x1, short y1, unsigned int flags);

/**
 * @brief Fill a rectangle on the current active LCD.
 * @x_syscall_num `0x10077`
 * @param x0 @x_term x0
 * @param y0 @x_term y0
 * @param x1 @x_term x1
 * @param y1 @x_term y1
 * @param flags Blit processing flags.
 * @x_void_return
 */
extern void FillRect(short x0, short y0, short x1, short y1, unsigned int flags);

/**
 * @brief Stroke a rounded corner rectangle on the current active LCD.
 * @x_syscall_num `0x10078`
 * @param x0 @x_term x0
 * @param y0 @x_term y0
 * @param x1 @x_term x1
 * @param y1 @x_term y1
 * @param rx Horizontal radius in pixels.
 * @param ry Vertical radius in pixels.
 * @param flags Blit processing flags.
 * @x_void_return
 */
extern void DrawRoundRect(short x0, short y0, short x1, short y1, short rx, short ry, unsigned int flags);

/**
 * @brief Stroke a circle on the current active LCD.
 * @x_syscall_num `0x10079`
 * @param x X coordinate of the center.
 * @param y Y coordinate of the center.
 * @param r Radius of the circle.
 * @param flags Blit processing flags.
 * @x_void_return
 */
extern void DrawCircle(short x, short y, short r, unsigned int flags);

/**
 * @brief Fill a circle on the current active LCD.
 * @warning This does not seem to actually fill the circle. Use FillEllipse() instead.
 * @x_syscall_num `0x1007a`
 * @param x X coordinate of the center.
 * @param y Y coordinate of the center.
 * @param r Radius of the circle.
 * @param flags Blit processing flags.
 * @x_void_return
 */
extern void FillCircle(short x, short y, short r, unsigned int flags);

/**
 * @brief Stroke an ellipse on the current active LCD.
 * @x_syscall_num `0x1007b`
 * @param x X coordinate of the center.
 * @param y Y coordinate of the center.
 * @param rx Horizontal radius of the ellipse.
 * @param ry Vertical radius of the ellipse.
 * @param flags Blit processing flags.
 * @x_void_return
 */
extern void DrawEllipse(short x, short y, short rx, short ry, unsigned int flags);

/**
 * @brief Fill an ellipse on the current active LCD.
 * @x_syscall_num `0x1007c`
 * @param x X coordinate of the center.
 * @param y Y coordinate of the center.
 * @param rx Horizontal radius of the ellipse.
 * @param ry Vertical radius of the ellipse.
 * @param flags Blit processing flags.
 * @x_void_return
 */
extern void FillEllipse(short x, short y, short rx, short ry, unsigned int flags);

/**
 * @brief Invert color within a rectangle.
 * @x_syscall_num `0x1007d`
 * @param x0 @x_term x0
 * @param y0 @x_term y0
 * @param x1 @x_term x1
 * @param y1 @x_term y1
 * @x_void_return
 */
extern void InverseSetArea(short x0, short y0, short x1, short y1);

/**
 * @brief Fill the current display canvas with background color, effectively clearing it.
 * @x_syscall_num `0x1007e`
 * @param fill_with_fg Fill with foreground color instead of background color.
 * @x_void_return
 */
extern void ClearScreen(bool fill_with_fg);

/**
 * @brief Move a rectangle down by `amount` pixels.
 * @x_syscall_num `0x10080`
 * @param x0 @x_term x0
 * @param y0 @x_term y0
 * @param x1 @x_term x1
 * @param y1 @x_term y1
 * @param amount Amount of pixels to move.
 * @x_void_return
 */
extern void ScrollDown(short x0, short y0, short x1, short y1, short amount);

/**
 * @brief Move a rectangle left by `amount` pixels.
 * @x_syscall_num `0x10081`
 * @param x0 @x_term x0
 * @param y0 @x_term y0
 * @param x1 @x_term x1
 * @param y1 @x_term y1
 * @param amount Amount of pixels to move.
 * @x_void_return
 */
extern void ScrollLeft(short x0, short y0, short x1, short y1, short amount);

/**
 * @brief Move a rectangle right by `amount` pixels.
 * @x_syscall_num `0x10082`
 * @param x0 @x_term x0
 * @param y0 @x_term y0
 * @param x1 @x_term x1
 * @param y1 @x_term y1
 * @param amount Amount of pixels to move.
 * @x_void_return
 */
extern void ScrollRight(short x0, short y0, short x1, short y1, short amount);

/**
 * @brief Move a rectangle up by `amount` pixels.
 * @x_syscall_num `0x10083`
 * @param x0 @x_term x0
 * @param y0 @x_term y0
 * @param x1 @x_term x1
 * @param y1 @x_term y1
 * @param amount Amount of pixels to move.
 * @x_void_return
 */
extern void ScrollUp(short x0, short y0, short x1, short y1, short amount);

/**
 * @brief Create a virtual LCD descriptor.
 * @details Virtual LCDs allow the program to draw using the LCD/canvas API without committing the pixels to the screen
 * immediately.
 * @x_syscall_num `0x10087`
 * @param width Width of the virtual LCD.
 * @param height Height of the virtual LCD.
 * @param width_bytes If set to a value larger than the value calculated from `width`, the pixel buffer will be
 * allocated according to this value instead (i.e. `width_bytes * height`).
 * @return The virtual LCD descriptor.
 */
extern lcd_t *CreateVirtualLCD(short width, short height, short width_bytes);

/**
 * @brief Dispose a previously created virtual LCD descriptor.
 * @x_syscall_num `0x10088`
 * @param lcd Pointer to a virtual LCD descriptor.
 * @x_void_return
 */
extern void DeleteVirtualLCD(lcd_t *lcd);

/**
 * @brief Set an LCD descriptor as active.
 * @x_syscall_num `0x1008b`
 * @param new_lcd Pointer to the new LCD descriptor. If `NULL`, only return the current active LCD descriptor.
 * @return The active LCD descriptor before `new_lcd` replaced it.
 */
extern lcd_t *SetActiveLCD(lcd_t *new_lcd);

/**
 * @brief Get the current active LCD descriptor.
 * @x_syscall_num `0x1008d`
 * @x_void_param
 * @return The current active LCD descriptor.
 */
extern lcd_t *GetActiveLCD(void);

/**
 * @brief Copy an LCD descriptor (excluding surface).
 * @details This function allocates a new LCD descriptor and copies everything from the source descriptor to the new
 * one. The new descriptor will not be linked to the source descriptor's lcd_t::surface, and lcd_t::saved_cursor
 * will be updated to the latest cursor states assigned to the current active LCD descriptor. If the target platform
 * supports LCD descriptor locking, a new critical section object will be created for the new descriptor.
 * @warning The new descriptor will not have any surface linked to it by default. It is required to manually link a
 * surface to it with e.g. SetDCObject() before using it to draw or things will break!
 * @x_syscall_num `0x1008e`
 * @param source The source LCD descriptor.
 * @return The new LCD descriptor copied from the source descriptor.
 */
extern lcd_t *CreateCompatibleLCD(lcd_t *source);

/**
 * @brief Link a surface (device context) to an LCD descriptor.
 * @details
 * The drawing area lcd_t::drawing_area will also be reset to cover the entire surface.
 *
 * If the display is portrait as indicated by the lcd_t::rotation value, the LCD descriptor's width/height fields will
 * be updated to reflect the dimension shown to the user (i.e. swap width and height).
 *
 * @x_syscall_num `0x10093`
 * @param lcd The LCD descriptor.
 * @param new_surface The descriptor of the new surface to be linked to `lcd`.
 * @return The descriptor of the surface previously linked to `lcd`.
 */
extern lcd_surface_t *SetDCObject(lcd_t *lcd, lcd_surface_t *new_surface);

/**
 * @brief Get X coordinate of the rightmost visible pixels on the current canvas.
 * @details This is usually tied to the physical resolution of the display. Therefore it can be used to determine the
 * display size. The display size can be calculated by adding 1 to the return values of both GetMaxScr* calls.
 * @x_syscall_num `0x10190`
 * @x_void_param
 * @return The X coordinate of the rightmost visible pixels.
 * @see GetMaxScrY Returns the Y boundary instead.
 */
extern short GetMaxScrX(void);

/**
 * @brief Get Y coordinate of the bottom-most visible pixels on the current canvas.
 * @details This is usually tied to the physical resolution of the display. Therefore it can be used to determine the
 * display size. The display size can be calculated by adding 1 to the return values of both `GetMaxScr*` calls.
 * @x_syscall_num `0x10191`
 * @x_void_param
 * @return The Y coordinate of the bottom-most visible pixels.
 * @see GetMaxScrX Returns the X boundary instead.
 */
extern short GetMaxScrY(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_CANVAS_H__
