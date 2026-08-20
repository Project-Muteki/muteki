/*
 * Copyright 2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file primitives.h
 * @brief Drawing routine primitive types.
 */


#ifndef __MUTEKI_DRAW_TYPES_FONT_H__
#define __MUTEKI_DRAW_TYPES_FONT_H__

#include "../../common.h"

#ifdef __cplusplus
extern "C" {
#endif

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

/**
 * @brief Font rendering routine common states.
 */
struct font_info_s {
    /**
     * @brief Font type.
     * @see font_type_e
     */
    unsigned short type; // 0x0:0x2 (lcd[0x34:0x36])
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

typedef struct font_info_s font_info_t;

#ifdef __cplusplus
} // extern "C"
#endif

#endif  // __MUTEKI_DRAW_TYPES_FONT_H__
