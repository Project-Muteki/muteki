/*
 * Copyright 2024 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file utf16.h
 * @brief UTF-16 conversion utilities.
 * @details Provides utility functions to convert strings of various types of encoding to UTF-16.
 * @note The supported encoding types are pretty limited and there might be surprising (or straight-up broken) behavior
 * that goes beyond our control. Use libiconv, ICU, etc. if you want to do more than what is available here.
 */

#ifndef __MUTEKI_UTF16_H__
#define __MUTEKI_UTF16_H__

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Supported encoding types.
 */
typedef enum utf16_mb_encoding_e {
    /**
     * @brief Use CP936 (GB18030-based).
     * @details Supported by both ConvStrToUnicode() and ConvCharToUnicode().
     */
    UTF16_MB_ENCODING_CP936 = 0x0001,
    /**
     * @brief Use CP950 (BIG5-based).
     * @details Supported by both ConvStrToUnicode() and ConvCharToUnicode().
     */
    UTF16_MB_ENCODING_CP950 = 0x0002,
    /**
     * @brief Use CP932 (Shift-JIS-based).
     * @details Supported by both ConvStrToUnicode() and ConvCharToUnicode().
     */
    UTF16_MB_ENCODING_CP932 = 0x0004,
    /**
     * @brief Use CP949 (EUC-KR-based).
     * @details Supported by both ConvStrToUnicode() and ConvCharToUnicode().
     */
    UTF16_MB_ENCODING_CP949 = 0x0008,
    /**
     * @brief Use CP874 (TIS-620-based).
     * @details Supported by ConvStrToUnicode() only.
     */
    UTF16_MB_ENCODING_CP874 = 0x0010,
    /**
     * @brief Use CP1252 (ISO8859-1-based).
     * @details
     * Supported by ConvStrToUnicode() only.
     *
     * @warning This may not work as intended, as it is an alias to CP950 on at least BA742.
     */
    UTF16_MB_ENCODING_CP1252 = 0x1000,
    /**
     * @brief Use UTF-8.
     * @details Supported by ConvStrToUnicode() only.
     */
    UTF16_MB_ENCODING_UTF8 = 0x7fff,
    /**
     * @brief Use UTF-16.
     * @details Supported by both ConvStrToUnicode() and ConvCharToUnicode().
     */
    UTF16_MB_ENCODING_UTF16 = 0x8000,
    /**
     * @brief Use default encoding.
     * @details
     * On BA742 this is hardcoded to be CP936.
     * 
     * Supported by both ConvStrToUnicode() and ConvCharToUnicode().
     */
    UTF16_MB_ENCODING_DEFAULT = 0xffff,
} utf16_mb_encoding_e;

/**
 * @brief Convert a multi-byte codepoint to UTF-16.
 * @note Not all encoding types listed in ::utf16_mb_encoding_e are supported by this syscall. Check the documentation
 * of it before proceeding. Passing unsupported encoding type to this syscall will cause undefined behavior.
 * @x_syscall_num{0x100bc}
 * @param src Multi-byte codepoint.
 * @param src_encoding Encoding type of the input codepoint.
 * @return UTF-16 codepoint.
 * @see utf16_mb_encoding_e Supported encoding types.
 */
extern UTF16 ConvCharToUnicode(unsigned int src, unsigned short src_encoding);

/**
 * @brief Convert a multi-byte string to UTF-16.
 * @note Passing unsupported encoding type to this syscall will cause undefined behavior.
 * @x_syscall_num{0x100bd}
 * @param[in] src Source multi-byte string.
 * @param[out] dst Converted UTF-16 string.
 * @param src_encoding Encoding type of the input codepoint.
 * @return Converted UTF-16 string.
 * @see utf16_mb_encoding_e Supported encoding types.
 */
extern UTF16 *ConvStrToUnicode(const void *src, UTF16 *dst, unsigned short src_encoding);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UTF16_H__
