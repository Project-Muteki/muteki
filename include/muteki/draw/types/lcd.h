/*
 * Copyright 2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file lcd.h
 * @brief Display canvas (LCD) type definitions.
 */

#ifndef __MUTEKI_DRAW_TYPES_LCD_H__
#define __MUTEKI_DRAW_TYPES_LCD_H__

#include "../../threading.h"
#include "font.h"
#include "surface.h"
#include "primitives.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Process flags for string and character printing.
 * @todo A lot of the bits seem to just print nothing. We need to formally look at the disassembly for this (huge!).
 */
enum print_flag_e {
    /**
     * @brief No extra process.
     */
    PRINT_NONE = 0,
    /**
     * @brief Invert colors.
     * @details Within the bounding box of the glyphs to be rendered, the RGB pixel value is inverted.
     */
    PRINT_INVERT = 0x1,
    /**
     * @brief Seems to be an alternative version of `PRINT_INVERT`.
     */
    PRINT_INVERT_ALT = 0x4,
    /**
     * @brief Draw underscores only.
     * @todo Speculation based on behavior. Seems to be very broken with >16px fonts.
     */
    PRINT_UNDERSCORE_ONLY = 0x40,
};

/**
 * @brief Process flags for string printing only.
 */
enum print_str_flag_e {
    /**
     * @brief Assume text encoding to be BIG5 instead of UTF-16.
     * @todo Seems to be some unknown extension? Limited mojibake test doesn't match iconv result.
     */
    PRINT_STR_ENCODING_BIG5 = 0x100,
    /**
     * @brief Assume text encoding to be GB18030 instead of UTF-16.
     */
    PRINT_STR_ENCODING_GB18030 = 0x200,
    /**
     * @brief Assume text encoding to be TIS-620 instead of UTF-16.
     */
    PRINT_STR_ENCODING_TIS_620 = 0x400,
};

/**
 * @brief String alignment types.
 */
enum str_align_e {
    /**
     * @brief Align to top-left.
     */
    STR_ALIGN_LEFT = 0,
    /**
     * @brief Align to top-right.
     */
    STR_ALIGN_RIGHT,
    /**
     * @brief Align to top-center.
     */
    STR_ALIGN_CENTER,
};

/**
 * @brief Rotation values used by lcd_rotate_callback_t.
 */
enum rotation_value_e {
    /** @brief Set the rotation so the top side of the canvas is facing up. */
    ROTATION_TOP_SIDE_FACING_UP = 0,
    /** @brief Set the rotation so the top side of the canvas is facing left. */
    ROTATION_TOP_SIDE_FACING_LEFT,
    /** @brief Set the rotation so the top side of the canvas is facing down. */
    ROTATION_TOP_SIDE_FACING_DOWN,
    /** @brief Set the rotation so the top side of the canvas is facing right. */
    ROTATION_TOP_SIDE_FACING_RIGHT,
    /** @brief Query the current rotation. Will not change the current location. */
    ROTATION_QUERY = 0xfe,
    /** @brief Actually rotate the buffer. Used internally. */
    ROTATION_APPLY = 0xff,
};

/**
 * @brief Predefined dash patterns.
 * @details The numbers here follows the SVG
 * [stroke-dasharray](https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/stroke-dasharray)
 * property format.
 * @todo Add graphic examples.
 */
enum stroke_predefined_dash_e {
    /**
     * @brief Solid line.
     */
    DASH_NONE = 0,
    DASH_8_8_16_0,
    DASH_4,
    DASH_0_4_16_4_4_4,
    DASH_12_4_4_4_4_4,
    DASH_1,
    DASH_2,
    DASH_16,
    DASH_0_1_8_1_8_1_8_1_4_0 = 12,
    DASH_4_1_8_1_7_1_9_1,
    DASH_3_1_7_1_7_1_4_8,
    DASH_3_1_7_1_7_1_7_1_4_0,
    DASH_0_1_6_1_1_1_4_1_3_1_2_1_5_2_3_0,
    DASH_3_2_5_1_2_1_3_1_4_1_1_1_6_1,
    DASH_7_1_6_1_6_1_6_1_3_0,
    DASH_3_1_6_1_6_1_6_1_7_0,
    DASH_24_8 = 22,
    /**
     * @brief Custom dash pattern.
     */
    DASH_CUSTOM = 0x100,
};

struct lcd_cursor_s;
struct lcd_draw_s;
struct lcd_lock_s;
struct lcd_base_s;
struct lcd_thread_safe_s;

typedef struct lcd_cursor_s lcd_cursor_t;
typedef struct lcd_draw_s lcd_draw_t;
typedef struct lcd_lock_s lcd_lock_t;
typedef struct lcd_base_s lcd_base_t;
typedef struct lcd_thread_safe_s lcd_thread_safe_t;

#if defined(MUTEKI_HAS_THREAD_SAFE_LCD) && MUTEKI_HAS_THREAD_SAFE_LCD == 1
#define lcd_s lcd_thread_safe_s
typedef struct lcd_thread_safe_s lcd_t;
#else
#define lcd_s lcd_base_s
typedef struct lcd_base_s lcd_t;
#endif

/**
 * @brief Cursor states usually linked to an LCD descriptor.
 */
struct lcd_cursor_s {
    /** @brief X coordinate of the cursor. */
    short x; // 0x0:0x2 (lcd_t[0x5c:0x5e])
    /** @brief Y coordinate of the cursor. */
    short y; // 0x2:0x4 (lcd_t[0x5e:0x60])
    /** @brief Size of the cursor. */
    unsigned int size; // 0x4:0x8 (lcd_t[0x60:0x64])
    /**
     * @brief Type of the cursor.
     * @todo Document the actual meaning of this.
     */
    short type; // 0x8:0xa (lcd_t[0x64:0x66])
    /** @brief Unknown. */
    short unk_0xa; // 0xa:0xc (lcd_t[0x66:0x68])
    /** @brief Number of times this cursor has been locked/grabbed by other code. */
    short grab_count; // 0xc:0xe (lcd_t[0x68:0x6a])
    /** @brief Unknown. */
    short unk_0xe; // 0xe:0x10 (lcd_t[0x6a:0x6c])
}; // 0x10 bytes

/**
 * @brief Drawing routine common states.
 */
struct lcd_draw_s {
    /** @brief Unknown. */
    int unk_0x0; // 0x0:0x4 (lcd_t[0x14:0x18])
    /** @brief Current foreground color. */
    int fg_color; // 0x4:0x8 (lcd_t[0x18:0x1c])
    /** @brief Dash bit pattern. */
    unsigned int stroke_dash_pattern; // 0x8:0xc (lcd_t[0x1c:0x20])
    /** @brief Size of stroke. */
    short stroke_size; // 0xc:0xe (lcd_t[0x20:0x22])
    /**
     * @brief Predefined dash pattern.
     * @see stroke_predefined_dash_e
     */
    short stroke_dash; // 0xe:0x10 (lcd_t[0x22:0x24])
    /** @brief Unknown. */
    int unk_0x10; // 0x10:0x14 (lcd_t[0x24:0x28])
    /** @brief Unknown. */
    int unk_0x14; // 0x14:0x18 (lcd_t[0x28:0x2c])
    /** @brief Unknown. */
    int unk_0x18; // 0x18:0x1c (lcd_t[0x2c:0x30])
    /** @brief Unknown. */
    int unk_0x1c; // 0x1c:0x20 (lcd_t[0x30:0x34])
}; // 0x20 bytes

/**
 * @brief An extended part of the LCD descriptor that implements descriptor locking.
 * @details This may not be present on all versions of Besta RTOS. It's confirmed to exist on BA742 but not BA110.
 * @todo Get a better picture of which systems have this and which do not.
 */
struct lcd_lock_s {
    /** @brief Unknown. */
    int unk_0x0; // 0x0:0x4 (lcd_thread_safe_t[0x94:0x98])
    /** @brief A critical section descriptor. It's unclear where it is used. */
    critical_section_t *cs; // 0x4:0x8 (lcd_thread_safe_t[0x98:0x9c])
    /** @brief Shortcut to lock the descriptor. */
    void (*lock)(void); // 0x8:0xc (lcd_thread_safe_t[0x9c:0xa0])
     /** @brief Shortcut to unlock the descriptor. */
    void (*unlock)(void); // 0xc:0x10 (lcd_thread_safe_t[0xa0:0xa4])
    /** @brief Unknown. */
    int unk_0xa4[23]; // 0x10:0x6c (lcd_thread_safe_t[0xa4:0x100])
}; // 0x6c bytes

/**
 * @brief Callback type for handling canvas rotation.
 * @details When `rotation` is set to ::ROTATION_QUERY, the current rotation value will be returned
 * with no side effect. When `rotation` is set to ::ROTATION_APPLY, the canvas will rotate based on
 * current value of lcd_t::rotation.
 * @param self The LCD descriptor this was called from.
 * @param rotation New rotation. This value will be written to lcd_t::rotation and this callback will be called
 * recursively with ::ROTATION_APPLY as the rotation value to actually apply the change.
 * @return Current rotation value in effect. This will be the same as `rotation` when no magic value documented above
 * was used.
 * @see lcd_t::rotation Where the location value is stored in the LCD descriptor.
 * @see rotation_value_e Possible rotation values.
 */
typedef int (*lcd_rotate_callback_t)(lcd_t *self, int rotation);

/**
 * @brief The LCD descriptor.
 * @see lcd_thread_safe_t Thread-safe variant of this struct that is used on some Besta RTOS devices.
 */
struct lcd_base_s {
    /** @brief Surface linked to the LCD. */
    lcd_surface_t *surface; // 0x0:0x4
    /** @brief End address of the pixel/framebuffer. */
    void *pixel_end; // 0x4:0x8
    /** @brief Total size of the pixel/framebuffer in bytes. */
    size_t pixel_size; // 0x8:0xc
    /** @brief Current background color. */
    int bg_color; // 0xc:0x10
    /** @brief Current transparent color. */
    int transparent_color; // 0x10:0x14
    /** @brief Per-LCD states shared by drawing routines. */
    lcd_draw_t draw; // 0x14:0x34
    /** @brief Per-LCD states shared by specifically font rendering routines. */
    font_info_t font; // 0x34:0x5c
    /** @brief A copy of the cursor states when the LCD descriptor was created. */
    lcd_cursor_t saved_cursor; // 0x5c:0x6c
    /** @brief Usable drawing area of the LCD. */
    rect_t drawing_area; // 0x6c:0x74
    /** @brief Unknown. */
    int unk_0x74[3]; // 0x74:0x80
    /** @brief Cursor states. */
    lcd_cursor_t *cursor; // 0x80:0x84
    /** @brief Width of the LCD in pixels. */
    short width; // 0x84:0x86
    /** @brief Height of the LCD in pixels. */
    short height; // 0x86:0x88
    /**
     * @brief Current canvas rotation.
     * @details The value is `90deg * rotation` **counter-clockwise**.
     */
    int rotation; // 0x88:0x8c
    /**
     * @brief Integer size of each pixel in bytes.
     * @note This will be 0 when a pixel takes less than a byte.
     * @todo Verify.
     */
    short depth_bytes; // 0x8c:0x8e
    /**
     * @brief Pixel row size in bytes.
     * @todo Verify.
     */
    short xsize; // 0x8e:0x90
    /**
     * @brief Rotation callback.
     * @see lcd_rotate_callback_t
     */
    lcd_rotate_callback_t rotate; // 0x90:0x94
}; // 0x94 bytes

/**
 * @brief A thread-safe variant of the LCD descriptor used on some versions of Besta RTOS.
 * @details Define `MUTEKI_HAS_THREAD_SAFE_LCD` as 1 to make this the underlying type of ::lcd_t.
 */
struct lcd_thread_safe_s {
    /** @brief Surface linked to the LCD. */
    lcd_surface_t *surface; // 0x0:0x4
    /** @brief End address of the pixel/framebuffer. */
    void *pixel_end; // 0x4:0x8
    /** @brief Total size of the pixel/framebuffer in bytes. */
    size_t pixel_size; // 0x8:0xc
    /** @brief Current background color. */
    int bg_color; // 0xc:0x10
    /** @brief Current transparent color. */
    int transparent_color; // 0x10:0x14
    /** @brief Per-LCD states shared by drawing routines. */
    lcd_draw_t draw; // 0x14:0x34
    /** @brief Per-LCD states shared by specifically font rendering routines. */
    font_info_t font; // 0x34:0x5c
    /** @brief A copy of the cursor states when the LCD descriptor was created. */
    lcd_cursor_t saved_cursor; // 0x5c:0x6c
    /** @brief Usable drawing area of the LCD. */
    rect_t drawing_area; // 0x6c:0x74
    /** @brief Unknown. */
    int unk_0x74[3]; // 0x74:0x80
    /** @brief Cursor states. */
    lcd_cursor_t *cursor; // 0x80:0x84
    /** @brief Width of the LCD in pixels. */
    short width; // 0x84:0x86
    /** @brief Height of the LCD in pixels. */
    short height; // 0x86:0x88
    /**
     * @brief Current canvas rotation.
     * @details The value is `90deg * rotation` **counter-clockwise**.
     */
    int rotation; // 0x88:0x8c
    /**
     * @brief Integer size of each pixel in bytes.
     * @note This will be 0 when a pixel takes less than a byte.
     * @todo Verify.
     */
    short depth_bytes; // 0x8c:0x8e
    /**
     * @brief Pixel row size in bytes.
     * @todo Verify.
     */
    short xsize; // 0x8e:0x90
    /**
     * @brief Rotation callback.
     * @see lcd_rotate_callback_t
     */
    lcd_rotate_callback_t rotate; // 0x90:0x94
    /** @brief Unknown. */
    int unk_0x94; // 0x94:0x98
    /** @brief A critical section descriptor. It's unclear where it is used. */
    critical_section_t *cs; // 0x98:0x9c
    /** @brief Shortcut to lock the descriptor. */
    void (*lock)(void); // 0x9c:0xa0
     /** @brief Shortcut to unlock the descriptor. */
    void (*unlock)(void); // 0xa0:0xa4
    /** @brief Unknown. */
    int unk_0xa4[23]; // 0xa4:0x100
};

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_DRAW_TYPES_LCD_H__
