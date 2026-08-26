/*
 * Copyright 2024-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file ini.h
 * @brief INI file operation API.
 * @details The functions in this API have the same names (other than the prefixed underscore) and function signatures
 * as their [Win32 API counterpart](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getprivateprofileint).
 * However some behaviors seen in the original Win32 API were missing or altered, particularly on how these functions
 * handle `NULL`s. Therefore one should not treat them the same way as treating their Win32 API counterpart.
 */

#ifndef __MUTEKI_INI_H__
#define __MUTEKI_INI_H__

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Read a value from an INI file and parse it as an integer.
 * @x_syscall_num{0x10110}
 * @param section The section where the key is located.
 * @param key The key.
 * @param default_value The default value to be returned when the key does not exist in the INI file.
 * @param path DOS 8.3 path to INI file.
 * @return The value that is associated with `section.key`, or `default` if the key does not exist.
 */
extern unsigned int _GetPrivateProfileInt(const char *section, const char *key, int default_value, const char *path);

/**
 * @brief Read a value from an INI file.
 * @details For values, the function will copy at most `outsize - 1` bytes and the last byte in the `out` buffer will
 * always be set to <tt>'\0'</tt>.
 * @warning It is unsafe to use this function as not all systems check `outsize` and buffer overrun is not preventable
 * on such systems.
 * @note The section/key listing behavior as seen in the corresponding Win32 API call is unimplemented and setting
 * `section` or `key` to `NULL` seems to only let the call fail unconditionally.
 * @x_syscall_num{0x10111}
 * @param section The section where the key is located.
 * @param key The key.
 * @param default_value The default value to be copied to `out` when the key does not exist in the INI file, or `""`
 * when set to `NULL`.
 * @param[out] out Pointer to the output buffer.
 * @param outsize Maximum size of the output buffer.
 * @param path DOS 8.3 path to INI file.
 * @return The length of the string copied to the `out` buffer.
 */
extern unsigned int _GetPrivateProfileString(
    const char *section,
    const char *key,
    const char *default_value,
    char *out,
    size_t outsize,
    const char *path
);

/**
 * @brief Write a value to an INI file.
 * @details `NULL` in `section`, `key` or `value` will be interpreted as the string `"<NULL>"`. A `NULL`
 * key but not a `NULL` section seems to also cause the function to stop using the specified section.
 * @x_syscall_num{0x10112}
 * @param section The section where the key is located.
 * @param key The key.
 * @param value The value.
 * @param path DOS 8.3 path to INI file.
 * @retval true @x_term{ok}
 * @retval false @x_term{ng}
 */
extern bool _WritePrivateProfileString(const char *section, const char *key, const char *value, const char *path);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_INI_H__
