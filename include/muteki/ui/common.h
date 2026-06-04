/*
 * Copyright 2024 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file common.h
 * @brief Common data structures and definitions for the UI subsystem.
 */

#ifndef __MUTEKI_UI_COMMON_H__
#define __MUTEKI_UI_COMMON_H__

#include <muteki/common.h>
#include <muteki/threading.h>

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
 * @brief Process flags for blit operations.
 * @warning These are stand-alone and not stackable with bitwise-OR operations (<tt>'|'</tt>).
 * @todo Need to look into this further. 2 and 16 seem to draw the image verbatim like 0. Nothing is being drawn when
 * setting any other flags. (Could it be related to some other properties on the surface descriptor such as transparent
 * color?)
 */
enum blit_flag_e {
    /**
     * @brief Nothing.
     * @details This just copies the pixels as-is.
     */
    BLIT_NONE = 0,
    /**
     * @brief Perform AND operation between source line and destination line.
     * @details This effectively does a bit-wise color masking of the destination surface.
     */
    BLIT_AND = 0x2,
    /**
     * @brief Perform OR operation between source line and destination line.
     * @details This effectively does a bit-wise painting onto the destination surface.
     * Colors will also be mixed in a bit-wise fashion.
     */
    BLIT_OR = 0x3,
    /**
     * @brief Perform XOR operation between source line and destination line.
     * @details This effectively does a bit-wise color inversion of the destination surface.
     */
    BLIT_XOR = 0x4,
    /**
     * @brief Honor transparent color settings on the current LCD canvas.
     * @details This copies the pixels when it's not exactly the same as the transparent color of the current LCD
     * canvas.
     */
    BLIT_TRANSPARENT = 0xe,
    /**
     * @brief Perform alpha blending (when supported).
     * @details This is done without gamma correction.
     */
    BLIT_ALPHA_BLENDING = 0xf,
    /**
     * @brief Perform alpha blending (when supported).
     * @details This is similar to #BLIT_ALPHA_BLENDING but instead of using alpha values per-pixel, it uses only the
     * alpha value of the first pixel for the entire surface. This makes the process slightly faster if the entire
     * image has the same alpha values for every pixel.
     */
    BLIT_ALPHA_BLENDING_FAST = 0x10,
};

/**
 * @brief Valid pixel formats for LCD surfaces.
 * @details All color values are assumed to be little endian.
 */
enum lcd_surface_pixfmt_e {
    /**
     * @brief 1-bit index color.
     */
    LCD_SURFACE_PIXFMT_L1 = 1,
    /**
     * @brief 4-bit index color.
     */
    LCD_SURFACE_PIXFMT_L4 = 4,
    /**
     * @brief 8-bit index color.
     */
    LCD_SURFACE_PIXFMT_L8 = 8,
    /**
     * @brief 12-bit RGB444.
     */
    LCD_SURFACE_PIXFMT_RGB444 = 12,
    /**
     * @brief 16-bit RGB565.
     */
    LCD_SURFACE_PIXFMT_RGB565 = 16,
    /**
     * @brief 24-bit RGB.
     */
    LCD_SURFACE_PIXFMT_RGB = 24,
    /**
     * @brief 32-bit XRGB.
     */
    LCD_SURFACE_PIXFMT_XRGB = 32,
    /**
     * @brief 32-bit ARGB (alias of #LCD_SURFACE_PIXFMT_XRGB).
     */
    LCD_SURFACE_PIXFMT_ARGB = 32,
};

/**
 * @brief Valid pixel buffer encoding for LCD surfaces.
 */
enum lcd_surface_encoding_e {
    /**
     * @brief Pixel buffer is contained in HCA container.
     */
    LCD_SURFACE_ENCODING_HCA = 1,
    /**
     * @brief Pixel buffer is in raw format.
     */
    LCD_SURFACE_ENCODING_RAW = 2,
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

/**
 * @brief List of available keycodes.
 * @details Keycodes starting with `KEY_PRIME_` are extended keycodes exclusive to HP Prime G1 (EA656).
 */
enum keycode_e {
    /* 0x00 */
    KEY_ESC = 0x01,
    KEY_LEFT,
    KEY_UP,
    KEY_RIGHT,
    KEY_DOWN,
    KEY_PGUP,
    KEY_PGDN,
    KEY_CAPS = 0x0a,
    KEY_DEL = 0xc,
    KEY_ENTER = 0x0d,
    /* 0x10 */
    /**
     * @brief Bring up the function menu of the current scene.
     * @details Not to be confused with the function menu.
     */
    KEY_MENU = 0x11,
    /**
     * @brief Bring up the function menu of the current scene.
     * @details Alias of #KEY_MENU.
     */
    KEY_FUNC_MENU = 0x11,
    /**
     * @brief Change font size.
     */
    KEY_FONT = 0x12,
    /* 0x20 */
    KEY_SPACE = ' ',
    KEY_EXCL = '!',
    KEY_TAB = 0x22,
    KEY_HASH = '#',
    KEY_DOLLAR = '$',
    KEY_PERCENT = '%',
    KEY_LPAREN = '(',
    KEY_RPAREN = ')',
    KEY_STAR = '*',
    KEY_COMMA = ',',
    KEY_DASH = '-',
    KEY_DOT = '.',
    /* 0x30 */
    KEY_0 = '0',
    KEY_1 = '1',
    KEY_2 = '2',
    KEY_3 = '3',
    KEY_4 = '4',
    KEY_5 = '5',
    KEY_6 = '6',
    KEY_7 = '7',
    KEY_8 = '8',
    KEY_9 = '9',
    KEY_QUESTION = '?',
    /* 0x40 */
    KEY_AT = '@',
    KEY_A = 'A',
    KEY_B = 'B',
    KEY_C = 'C',
    KEY_D = 'D',
    KEY_E = 'E',
    KEY_F = 'F',
    KEY_G = 'G',
    KEY_H = 'H',
    KEY_I = 'I',
    KEY_J = 'J',
    KEY_K = 'K',
    KEY_L = 'L',
    KEY_M = 'M',
    KEY_N = 'N',
    KEY_O = 'O',
    /* 0x50 */
    KEY_P = 'P',
    KEY_Q = 'Q',
    KEY_R = 'R',
    KEY_S = 'S',
    KEY_T = 'T',
    KEY_U = 'U',
    KEY_V = 'V',
    KEY_W = 'W',
    KEY_X = 'X',
    KEY_Y = 'Y',
    KEY_Z = 'Z',
    /* 0x80 */
    /**
     * @brief Trigger TTS in Mandarain Chinese.
     */
    KEY_LANG_CHN = 0x80,
    /**
     * @brief Trigger TTS in Cantonese.
     */
    KEY_LANG_YUE = 0x81,
    /**
     * @brief Trigger TTS in English.
     */
    KEY_LANG_ENG = 0x82,
    /**
     * @brief Power button event.
     */
    KEY_POWER = 0x83,
    KEY_F1 = 0x84,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    /**
     * @brief Launch voice recorder.
     */
    KEY_APP_REC = 0x89,
    KEY_SHIFT = 0x8b,
    /**
     * @brief Switch input method.
     */
    KEY_IME = 0x8e,
    /**
     * @brief Trigger TTS in Japanese.
     */
    KEY_LANG_JPN = 0x8f,
    /* 0x90 */
    /**
     * @brief Activate the symbol selection input method.
     */
    KEY_SYMBOL = 0x91,
    /**
     * @brief Return to home menu.
     */
    KEY_HOME = 0x93,
    /**
     * @brief Return to home menu.
     * @details Alias of #KEY_HOME
     */
    KEY_HOME_MENU = 0x93,
    /**
     * @brief The MENU key on HP Prime.
     * @details Alias of #KEY_HOME
     */
    KEY_PRIME_MENU = 0x93,
    /** 
     * @brief Toggle simplified/traditional Chinese.
     */
    KEY_TOGGLE_SC_TC = 0x94,
    /**
     * @brief Show in-app help.
     */
    KEY_HELP = 0x95,
    /**
     * @brief Save current change and exit.
     */
    KEY_SAVE = 0x96,
    /**
     * @brief Launch MP3 player.
     */
    KEY_APP_MP3_PLAYER = 0x98,
    /**
     * @brief Open the Volume + Backlight adjustment menu.
     */
    KEY_VOL_BACKLIGHT = 0x9c,
    /**
     * @brief Dictionary: Pronounce the current word syllable-by-syllable.
     */
    KEY_SYLLABLE = 0x9e,
    /**
     * @brief Dictionary and search: Go back to the first screen the user initiated a search (with input memorized).
     */
    KEY_ORIGINAL_INPUT = 0x9f,
    /* 0xa0 */
    /**
     * @brief Board-specific app launching shortcut.
     */
    KEY_APP_MISC_3 = 0xa3,
    /**
     * @brief Launch the flashcard app.
     * @details Available on CA743.
     */
    KEY_APP_FLASHCARD = 0xa3,
    /**
     * @brief Board-specific app launching shortcut.
     */
    KEY_APP_MISC_5 = 0xa5,
    /**
     * @brief Launch Daijirin Japanese dictionary.
     * @details Available on JA738.
     */
    KEY_APP_DICT_JA_DAJIRIN = 0xa5,
    /**
     * @brief Launch the Games app.
     * @details Available on CA743.
     */
    KEY_APP_GAMES = 0xa5,
    /**
     * @brief Board-specific app launching shortcut.
     */
    KEY_APP_MISC_6 = 0xa6,
    /**
     * @brief Launch Shin-Meikai Japanese dictionary.
     * @details Available on JA738.
     */
    KEY_APP_DICT_JA_SHINMEIKAI = 0xa6,
    /**
     * @brief Launch the album app.
     * @details Available on CA743.
     */
    KEY_APP_ALBUM = 0xa6,
    /**
     * @brief Board-specific app launching shortcut.
     */
    KEY_APP_MISC_7 = 0xa7,
    /**
     * @brief Launch Longman dictionary.
     * @details Available on CA743.
     */
    KEY_APP_DICT_EN_LONGMAN = 0xa7,
    /**
     * @brief Launch Genius Japanese-English dictionary.
     * @details Available on JA738.
     */
    KEY_APP_DICT_JA_EN_GENIUS = 0xa7,
    /**
     * @brief Board-specific app launching shortcut.
     */
    KEY_APP_MISC_8 = 0xa8,
    /**
     * @brief Launch Genius English-Japanese dictionary.
     * @details Available on JA738.
     */
    KEY_APP_DICT_EN_JA_GENIUS = 0xa8,
    /* 0xb0 */
    KEY_PRIME_APPS = 0xb1,
    KEY_PRIME_PLOT,
    KEY_PRIME_NUM,
    KEY_PRIME_VIEW,
    KEY_PRIME_CAS,
    KEY_PRIME_ALPHA,
    KEY_PRIME_MINUS,
    KEY_PRIME_DECIMAL_POINT,
    KEY_PRIME_PLUS,
    /* 0xc0 */
    /**
     * @brief Show the Favorites menu.
     */
    KEY_FAV = 0xc0,
    /* 0xd0 */
    /**
     * @brief Launch video player app.
     */
    KEY_APP_VIDEO_PLAYER = 0xd0,
    /**
     * @brief Open the global auxilary menu on touchscreen-only devices.
     */
    KEY_AUX_MENU = 0xd1,
    /**
     * @brief Open the English to Chinese translator app.
     */
    KEY_APP_TRANSLATOR_EN_ZH = 0xd2,
    /**
     * @brief Open the Chinese to English translator app.
     */
    KEY_APP_TRANSLATOR_ZH_EN = 0xd3,
    /**
     * @brief Launch Besta professional vocabulary dictionary.
     */
    KEY_APP_DICT_EN_PRO = 0xd4,
    /* 0xe0 */
    /**
     * @brief Launch Besta Chinese-English dictionary.
     */
    KEY_APP_DICT_ZH_EN_BESTA = 0xe8,
    /**
     * @brief Launch Besta Japanese-Chinese dictionary.
     */
    KEY_APP_DICT_JA_ZH_BESTA = 0xe9,
    /**
     * @brief Launch Besta Chinese-Japanese dictionary.
     */
    KEY_APP_DICT_ZH_JA_BESTA = 0xea,
    /* 0xf0 */
    /**
     * @brief Launch Oxford dictionary.
     */
    KEY_APP_DICT_EN_OXFORD = 0xf2,
    /**
     * @brief Launch Besta English-Chinese dictionary.
     */
    KEY_APP_DICT_EN_ZH_BESTA = 0xf3,
    /**
     * @brief Show the edit menu (cut/copy/paste).
     */
    KEY_EDIT = 0xf5,
    /**
     * @brief Launch the unified search app.
     */
    KEY_SEARCH = 0xf6,
    /**
     * @brief Open the Backlight adjustment menu.
     * @details May be an alias to #KEY_VOL_BACKLIGHT.
     */
    KEY_BACKLIGHT = 0xf8,
    /**
     * @brief Dictionary: Parrot the current word and compare the pronunciations.
     */
    KEY_COMPARE = 0xf9,
    /**
     * @brief Open the Volume adjustment menu.
     * @details May be an alias to #KEY_VOL_BACKLIGHT.
     */
    KEY_VOL = 0xfa,
    /**
     * @brief Display battery indicator popup.
     */
    KEY_BATTERY = 0xfb,
    /**
     * @brief Open LCD backlight and contrast adjustment menu.
     */
    KEY_CONTRAST = 0xfc,
    /**
     * @brief Event on USB cable insertion.
     */
    KEY_USB_INSERTION = 0xfe,
    /**
     * @brief Dictionary: Repeatedly read out the current word.
     */
    KEY_REPEAT = 0xff,
    /* 0x100+ */
    KEY_INS = 0x101,
    /**
     * @brief Scroll wheel event.
     */
    KEY_SCROLL_WHEEL_EVENT = 0x102,
    /**
     * @brief Trigger TTS.
     * @details Some models may have this key instead of the usual `KEY_LANG_*` series keys for triggering TTS.
     */
    KEY_TTS = 0x104,
    KEY_SHIFT_PGUP = 0xe047,
    KEY_PRIME_HOME = 0xe047,
    KEY_SHIFT_PGDN = 0xe04f,
};

/**
 * @brief Scroll wheel direction.
 * @details Only confirmed on BA802.
 */
enum scroll_wheel_e {
    /**
     * @brief Scroll down (clockwise direction).
     */
    SCROLL_DOWN,
    /**
     * @brief Scroll up (counter-clockwise direction).
     */
    SCROLL_UP,
};

/**
 * @brief UI event types.
 */
enum ui_event_type_e {
    /**
     * @brief Invalid/cleared.
     */
    UI_EVENT_TYPE_INVALID = 0,
    /**
     * @brief Beginning of touch/pen down event.
     */
    UI_EVENT_TYPE_TOUCH_BEGIN = 1,
    /**
     * @brief Touch/pen move event.
     */
    UI_EVENT_TYPE_TOUCH_MOVE = 2,
    /**
     * @brief End of touch/pen up event.
     */
    UI_EVENT_TYPE_TOUCH_END = 8,
    /**
     * @brief Key(s) pressed.
     */
    UI_EVENT_TYPE_KEY = 16,
    /**
     * @brief Key(s) released.
     * @details Available on S3C and TCC boards.
     */
    UI_EVENT_TYPE_KEY_UP = 0x100000,
};

/**
 * @brief Font types.
 * @details The naming follows the following format, joined with underscores (_):
 *
 * `<style>_[variant]_[size]_<cjk>_[cjkvariant]_[cjksize]_[index]`
 *
 * Where
 * - `style` is one of `SANS`, `SERIF` or `MONOSPACE`.
 * - `variant` or `cjkvariant` is one of `BOLD` (equivalent to `REGULAR` for CJK), `ITALIC`, `BOLDITALIC`
 *   (equivalent to `ITALIC` for CJK) or `REGULAR` (omitted for `variant`). For `cjkvariant`, if it is the same as
 *   `variant`, it will be omitted.
 * - `size` or `cjksize` is one of `TINY` (8px height, not available for CJK), `SMALL` (12px), `NORMAL` (16px,
 *   omitted for `size`), `LARGE` (20px) or `HUGE` (22px, not available for CJK). For `cjksize`, if it is the
 *   same as `size`, it will be omitted.
 * - `cjk` is either `CJK` (supports CJK character rendering) or `NOCJK` (does not support CJK character rendering).
 * - `index` is the current index (only used when all the above fields collide).
 */
enum font_type_e {
    SANS_TINY_CJK_NORMAL = 0,
    SANS_BOLD_TINY_CJK_NORMAL,
    SANS_ITALIC_TINY_CJK_REGULAR_NORMAL,
    SANS_BOLDITALIC_TINY_CJK_REGULAR_NORMAL,
    SERIF_SMALL_CJK,
    SERIF_BOLD_SMALL_CJK,
    SERIF_ITALIC_SMALL_CJK_REGULAR,
    SERIF_BOLDITALIC_SMALL_CJK_REGULAR,
    SERIF_CJK,
    SERIF_BOLD_CJK,
    SERIF_ITALIC_CJK_REGULAR,
    SERIF_BOLDITALIC_CJK_REGULAR,
    SERIF_LARGE_CJK,
    SERIF_BOLD_LARGE_CJK,
    SERIF_ITALIC_LARGE_CJK,
    SERIF_BOLDITALIC_LARGE_CJK,
    MONOSPACE_CJK,
    MONOSPACE_TINY_NOCJK,
    SERIF_CJK_18,
    SERIF_BOLD_CJK_19,
    SERIF_SMALL_CJK_20,
    SANS_TINY_NOCJK,
    SANS_BOLD_TINY_NOCJK,
    SANS_ITALIC_TINY_NOCJK,
    SANS_BOLDITALIC_TINY_NOCJK,
    SERIF_SMALL_CJK_25,
    SERIF_BOLD_SMALL_CJK_26,
    SERIF_ITALIC_SMALL_CJK_REGULAR_27,
    SERIF_BOLDITALIC_SMALL_CJK_REGULAR_28,
    SERIF_CJK_29,
    SERIF_BOLD_CJK_30,
    SERIF_ITALIC_CJK_REGULAR_31,
    SERIF_BOLDITALIC_CJK_REGULAR_32,
    SERIF_HUGE_CJK_LARGE,
    SANS_HUGE_CJK_LARGE,
    SERIF_ITALIC_HUGE_CJK_LARGE,
    SANS_ITALIC_HUGE_CJK_LARGE,
};

struct lcd_surface_s;
struct lcd_cursor_s;
struct ui_rect_s;
struct lcd_draw_s;
struct lcd_font_s;
struct lcd_lock_s;
struct lcd_base_s;
struct lcd_thread_safe_s;
struct ui_event_base_s;
struct ui_event_prime_s;
struct ui_message_s;
struct ui_component_s;
struct ui_multipress_event_s;

typedef struct lcd_surface_s lcd_surface_t;
typedef struct lcd_cursor_s lcd_cursor_t;
typedef struct ui_rect_s ui_rect_t;
typedef struct lcd_draw_s lcd_draw_t;
typedef struct lcd_font_s lcd_font_t;
typedef struct lcd_lock_s lcd_lock_t;
typedef struct lcd_base_s lcd_base_t;
typedef struct lcd_thread_safe_s lcd_thread_safe_t;
typedef struct ui_event_base_s ui_event_base_t;
typedef struct ui_event_prime_s ui_event_prime_t;
typedef struct ui_message_s ui_message_t;
typedef struct ui_component_s ui_component_t;
typedef struct ui_multipress_event_s ui_multipress_event_t;

#if defined(MUTEKI_HAS_PRIME_UI_EVENT) && MUTEKI_HAS_PRIME_UI_EVENT == 1
#define ui_event_s ui_event_prime_s
typedef struct ui_event_prime_s ui_event_t;
#else
#define ui_event_s ui_event_base_s
typedef struct ui_event_base_s ui_event_t;
#endif

#if defined(MUTEKI_HAS_THREAD_SAFE_LCD) && MUTEKI_HAS_THREAD_SAFE_LCD == 1
#define lcd_s lcd_thread_safe_s
typedef struct lcd_thread_safe_s lcd_t;
#else
#define lcd_s lcd_base_s
typedef struct lcd_base_s lcd_t;
#endif

/**
 * @brief Descriptor of an LCD drawing surface or hardware framebuffer.
 * @details This contains format description of the pixel/framebuffer and a pointer to the actual buffer.
 */
struct lcd_surface_s {
    /**
     * @brief Header magic.
     * @details Always `"PX"` without NUL.
     */
    char magic[2]; // 0:2
    /**
     * @brief Width of the framebuffer.
     */
    short width; // 2:4
    /**
     * @brief Height of the framebuffer.
     */
    short height; // 4:6
    /**
     * @brief Depth of the color in bits.
     * @see lcd_surface_pixfmt_e
     */
    short depth; // 6:8
    /**
     * @brief Pixel row size in bytes (aka. pitch if you know SDL).
     * @details This is usually 4-bytes aligned.
     */
    short xsize; // 8:10
    /**
     * @brief Pixel buffer encoding.
     * @see lcd_surface_encoding_e
     */
    short encoding; // 10:12
    /**
     * @brief Palette used to pack RGBA color into pixels.
     * @details Only make sense when using indexed color. It should be `NULL` otherwise.
     */
    int *palette; // 12:16
    /**
     * @brief The actual framebuffer/surface pixel buffer region.
     * @details The actual format varies and depends on the parameters provided above.
     */
    void *buffer; // 16:20
}; // 20 bytes

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
 * @brief Rectangle used to represent usable drawing area in an LCD descriptor.
 * @details The area is double-inclusive (i.e. `{0, 0, 479, 271}` represents an area of 480x272 px).
 */
struct ui_rect_s {
    /** @brief @x_term x0 */
    short x0; // (lcd_t[0x6c:0x6e])
    /** @brief @x_term y0 */
    short y0; // (lcd_t[0x6e:0x70])
    /** @brief @x_term x1 */
    short x1; // (lcd_t[0x70:0x72])
    /** @brief @x_term y1 */
    short y1; // (lcd_t[0x72:0x74])
}; // 0x8 bytes

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
    /** @brief Predefined dash pattern. */
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
 * @brief Font rendering routine common states.
 */
struct lcd_font_s {
    /**
     * @brief Font type.
     * @see font_type_e
     */
    short type; // 0x0:0x2 (lcd[0x34:0x36])
    /** @brief Height of the font. */
    short height; // 0x2:0x4 (lcd[0x36:0x38])
    /**
     * @brief Unknown.
     * @details Normally the same as `height` but sometimes can be a different value. Touched by SetFontType().
     */
    short unk_0x4; // 0x4:0x6 (lcd[0x38:0x3a])
    /** @brief Unknown. */
    short unk_0x6; // 0x6:0x8 (lcd[0x3a:0x3c])
    /** @brief Unknown. */
    short unk_0x8; // 0x8:0xa (lcd[0x3c:0x3e])
    /**
     * @brief Unknown.
     * @details Seems to be always `0x100`. Touched by SetFontType().
     */
    short unk_0xa; // 0xa:0xc (lcd[0x3e:0x40])
    /**
     * @brief Unknown.
     * @details Seems to be always `0x801`. Touched by SetFontType().
     */
    short unk_0xc; // 0xc:0xe (lcd[0x40:0x42])
    short unk_0xe[13]; // 0xe:0x28 (lcd[0x42:0x5c])
}; // 0x28 bytes

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
    lcd_font_t font; // 0x34:0x5c
    /** @brief A copy of the cursor states when the LCD descriptor was created. */
    lcd_cursor_t saved_cursor; // 0x5c:0x6c
    /** @brief Usable drawing area of the LCD. */
    ui_rect_t drawing_area; // 0x6c:0x74
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
    lcd_font_t font; // 0x34:0x5c
    /** @brief A copy of the cursor states when the LCD descriptor was created. */
    lcd_cursor_t saved_cursor; // 0x5c:0x6c
    /** @brief Usable drawing area of the LCD. */
    ui_rect_t drawing_area; // 0x6c:0x74
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

/**
 * @brief Multipress/multitouch event.
 * @details This is a simplified version of the main UI event struct, that only contains the necessary fields to
 * represent a multitouch or a key-press event. Used on Prime G1.
 */
struct ui_multipress_event_s {
    /**
     * @brief Type of event.
     * @see ui_event_type_e List of event types.
     */
    unsigned int type;
    /**
     * @brief Finger ID of a touch event.
     */
    unsigned short finger_id;
    union {
        struct {
            /**
             * @brief Keycode for the first pressed key.
             */
            unsigned short key_code0;
            /**
             * @brief Keycode for the second pressed key (maybe unused).
             */
            unsigned short key_code1;
        };
        struct {
            /**
             * @brief The X coordinate of where the touch event is located, in pixels.
             * @details Only available when ::type is ::UI_EVENT_TYPE_TOUCH_BEGIN,
             * ::UI_EVENT_TYPE_TOUCH_MOVE, or ::UI_EVENT_TYPE_TOUCH_END.
             */
            unsigned short touch_x;
            /**
             * @brief The Y coordinate of where the touch event is located, in pixels.
             * @details Only available when ::type is ::UI_EVENT_TYPE_TOUCH_BEGIN,
             * ::UI_EVENT_TYPE_TOUCH_MOVE, or ::UI_EVENT_TYPE_TOUCH_END.
             */
            unsigned short touch_y;
        };
    };
    /**
     * @brief Unknown. Maybe unused and probably padding.
     */
    unsigned short unk_0xb;
};

/**
 * @brief Structure for low level UI events.
 * @see ui_event_prime_s HP Prime G1's extension of this struct.
 */
struct ui_event_base_s {
    /**
     * @brief Event recipient.
     * @details If set to `NULL`, the event is a broadcast event (e.g. input event). Otherwise, the
     * widget's ui_component_t::on_event callback will be called with this event.
     */
    ui_component_t *recipient; // 0-4
    /**
     * @brief The type of event (0x10 being key event)
     * @see ui_event_type_e List of event types.
     */
    int event_type; // 4-8 16: key (?).
    union {
        struct {
            /** 
             * @brief Keycode for the first pressed key.
             * @details Only available when ::event_type is ::UI_EVENT_TYPE_KEY.
             */
            unsigned short key_code0; // 8-10
            /**
             * @brief Keycode for the second pressed key.
             * @details Only available when ::event_type is ::UI_EVENT_TYPE_KEY.
             * @note Depending on the exact keys pressed simultaneously, this is not always accurate. Moreover,
             * some devices may lack support of simultaneous key presses.
             */
            unsigned short key_code1; // 10-12 sometimes set when 2 keys are pressed simultaneously. Does not always work.
        };
        struct {
            /**
             * @brief The X coordinate of where the touch event is located, in pixels.
             * @details Only available when ::event_type is ::UI_EVENT_TYPE_TOUCH_BEGIN,
             * ::UI_EVENT_TYPE_TOUCH_MOVE, or ::UI_EVENT_TYPE_TOUCH_END.
             */
            unsigned short touch_x;
            /**
             * @brief The Y coordinate of where the touch event is located, in pixels.
             * @details Only available when ::event_type is ::UI_EVENT_TYPE_TOUCH_BEGIN,
             * ::UI_EVENT_TYPE_TOUCH_MOVE, or ::UI_EVENT_TYPE_TOUCH_END.
             */
            unsigned short touch_y;
        };
    };
    union {
        /**
        * @brief Unknown.
        * @details Set along with a ::KEY_USB_INSERTION event. Seems to point to some data. Exact purpose unknown.
        */
        void *usb_data; // 12-16 pointer that only shows up on USB insertion event.
        /**
         * @brief Scroll wheel direction, either SCROLL_DOWN or SCROLL_UP.
         * @see scroll_wheel_e
         */
        unsigned int scroll_wheel;
    };

    /**
     * @brief Unknown.
     * @details Maybe used on event types other than touch and key press.
     */
    void *unk16; // 16-20 sometimes a pointer especially on unstable USB connection? junk data?
    /**
     * @brief Unknown.
     * @details Seems to be always 0, although ClearEvent() explicitly sets this to 0. Maybe used on event types other
     * than touch and key press.
     */
    void *unk20; // 20-24 seems to be always 0. Unused?
};

/**
 * @brief Structure for low level UI events (Prime G1 extension).
 * @details Define `MUTEKI_HAS_PRIME_UI_EVENT` as 1 to make this the underlying type of ui_event_t.
 */
struct ui_event_prime_s {
    /**
     * @brief Event recipient.
     * @details If set to `NULL`, the event is a broadcast event (e.g. input event). Otherwise, the
     * widget's ui_component_t::on_event callback will be called with this event.
     */
    ui_component_t *recipient; // 0-4
    /**
     * @brief The type of event (0x10 being key event)
     * @see ui_event_type_e List of event types.
     */
    int event_type; // 4-8 16: key (?).
    union {
        struct {
            /** 
             * @brief Keycode for the first pressed key.
             * @details Only available when ::event_type is ::UI_EVENT_TYPE_KEY.
             */
            unsigned short key_code0; // 8-10
            /**
             * @brief Keycode for the second pressed key.
             * @details Only available when ::event_type is ::UI_EVENT_TYPE_KEY.
             * @note Depending on the exact keys pressed simultaneously, this is not always accurate. Moreover,
             * some devices may lack support of simultaneous key presses.
             */
            unsigned short key_code1; // 10-12 sometimes set when 2 keys are pressed simultaneously. Does not always work.
        };
        struct {
            /**
             * @brief The X coordinate of where the touch event is located, in pixels.
             * @details Only available when ::event_type is ::UI_EVENT_TYPE_TOUCH_BEGIN,
             * ::UI_EVENT_TYPE_TOUCH_MOVE, or ::UI_EVENT_TYPE_TOUCH_END.
             */
            unsigned short touch_x;
            /**
             * @brief The Y coordinate of where the touch event is located, in pixels.
             * @details Only available when ::event_type is ::UI_EVENT_TYPE_TOUCH_BEGIN,
             * ::UI_EVENT_TYPE_TOUCH_MOVE, or ::UI_EVENT_TYPE_TOUCH_END.
             */
            unsigned short touch_y;
        };
    };
    union {
        /**
        * @brief Unknown.
        * @details Set along with a ::KEY_USB_INSERTION event. Seems to point to some data. Exact purpose unknown.
        */
        void *usb_data; // 12-16 pointer that only shows up on USB insertion event.
        /**
         * @brief Scroll wheel direction, either SCROLL_DOWN or SCROLL_UP.
         * @see scroll_wheel_e
         */
        unsigned int scroll_wheel;
    };
    /**
     * @brief Unknown.
     * @details Maybe used on event types other than touch and key press.
     */
    void *unk16; // 16-20 sometimes a pointer especially on unstable USB connection? junk data?
    /**
     * @brief Unknown.
     * @details Seems to be always 0, although ClearEvent() explicitly sets this to 0. Maybe used on event types other
     * than touch and key press.
     */
    void *unk20; // 20-24 seems to be always 0. Unused?
    /**
     * @brief Number of valid multipress events available for processing.
     */
    unsigned short available_multipress_events; // 24-26
    /**
     * @brief Unknown. Sometimes can be 0x2 on startup.
     */
    unsigned short unk_0x1a; // 26-28
    /**
     * @brief The multipress events.
     */
    ui_multipress_event_t multipress_events[8]; // 28-124
};

/**
 * @brief Event struct used by SendMessage().
 */
struct ui_message_s {
    /**
     * @brief The event struct.
     */
    ui_event_t event; // 0x0:0x18
    /**
     * @brief Unknown.
     */
    int unk_0x18; // 0x18:0x1c
}; // 0x1c bytes

/**
 * @brief Common part of all widgets and views.
 * @todo Find a way to make the callbacks respect subtypes? Currently type-specific callbacks need to have their `self`
 * argument defined as `ui_component_t *` (or `void *` if we opt to do that) which is a bit less ideal.
 */
struct ui_component_s {
    /**
     * @brief Next component in a component ring or list.
     * @details This will be set to `NULL` when initialized, and, depending on context, will be a pointer to the next
     * component, a pointer to this component or `NULL`.
     */
    ui_component_t *next; // 0x0:0x4
    /**
     * @brief Parent component.
     * @details This will usually be a view.
     */
    ui_component_t *parent; // 0x4:0x8
    /**
     * @brief Component state bit field.
     * @details Each component may use this differently to represent different states.
     */
    unsigned short state; // 0x8:0xa
    /**
     * @brief Unknown. Set to 1 on buttons and 0xfd01 on deskbox views.
     */
    short unk_0xa; // 0xa:0xc
    /**
     * @brief Component visual footprint.
     * @details This defines the size and location of the component on screen.
     */
    ui_rect_t footprint; // 0xc:0x14
    /**
     * @brief Component state change callback.
     */
    void (*on_change_state)(ui_component_t *self, unsigned short mask, bool value); // 0x14:0x18
    /**
     * @brief Component callback for erasing background.
     * @todo Figure out exactly what this does.
     */
    void (*on_erase_bg)(ui_component_t *self); // 0x18:0x1c
    /**
     * @brief Component draw callback.
     */
    void (*on_draw)(ui_component_t *self); // 0x1c:0x20
    /**
     * @brief Component event callback.
     */
    void (*on_event)(ui_component_t *self, ui_event_t *event); // 0x20:0x24
    /**
     * @brief Unknown. Used in MessageBox().
     */
    void *unk_0x24; // 0x24:0x2a
    /**
     * @brief Unknown.
     */
    short unk_0x28;
    /**
     * @brief Unknown. Set to 0x65 on buttons.
     */
    short unk_0x2a; // 0x2a:0x2c
    /**
     * @brief Unknown.
     */
    int unk_0x2c; // 0x2c:0x30
    /**
     * @brief @x_term tag
     */
    unsigned short tag; // 0x30:0x32
    /**
     * @brief Unknown. Probably padding.
     */
    short unk_0x32; // 0x32:0x34
};

/**
 * @brief Convert separate RGB values to integer RGB representation
 * @param r Red value.
 * @param g Green value.
 * @param b Blue value.
 */
#define RGB_FROM_U8(r, g, b) ((r & 0xff) << 16 | ((g & 0xff) << 8) | (b & 0xff))

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_COMMON_H__
