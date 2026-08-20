/*
 * Copyright 2024 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file filepicker.h
 * @brief Functions related to the file picker modal.
 */

#ifndef __MUTEKI_UI_MODALS_FILEPICKER_H__
#define __MUTEKI_UI_MODALS_FILEPICKER_H__

#include "../../common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Allocate output buffer for filepicker_context_t.
 * @param calloc_impl Implementation of calloc().
 * @param entries Maximum number of entries allowed. Must be the same as filepicker_context_t::npaths.
 * @param is_utf16 `true` if the buffer should be UTF16 type. Set to `false` if the flag ::FILEPICKER_USE_MB is set.
 * @return Allocated buffer.
 */
#define FILEPICKER_CONTEXT_OUTPUT_ALLOC(calloc_impl, entries, is_utf16) \
    calloc_impl(entries * 260 + 2, is_utf16 ? sizeof(UTF16) : sizeof(char))

/**
 * @brief Max LFN path size for file picker.
 */
#define FILEPICKER_CONTEXT_OUTPUT_MAX_LFN 260

/**
 * @brief Known file picker flags.
 * @todo Add more.
 */
enum filepicker_flag_e {
    /**
     * @brief Allow multiple files.
     */
    FILEPICKER_ALLOW_MULTIPLE=0x200,
    /**
     * @brief Use multi-byte strings in the current locale's encoding instead of UTF16.
     */
    FILEPICKER_USE_MB=0x40000,
};

struct filepicker_context_s;
typedef struct filepicker_context_s filepicker_context_t;

/**
 * @brief The file picker context.
 */
struct filepicker_context_s {
    /**
     * @brief Context size.
     * @details Must be set to sizeof(filepicker_context_t) before invoking the file picker.
     */
    size_t ctx_size;
    /**
     * @brief Unknown.
     */
    void *unk_0x4;
    /**
     * @brief List of accepted file types.
     * @details
     *
     * Can be overridden by the user before invoking the file picker.
     *
     * The list must be in this format:
     * @code{.c}
     * const char *list = "<description>\0<fnmatch_pattern>\0...<description>\0<fnmatch_pattern>\0\0\0\0";
     * @endcode
     *
     * `description` should be a string describing the format and `fnmatch_pattern` should be an fnmatch pattern
     * supported by _afindfirst(). The string also needs to be terminated with triple NULs (`"\0\0\0"`).
     */
    const char *type_list;
    /**
     * @brief Unknown.
     */
    int unk_0xc;
    /**
     * @brief Paths buffer.
     * @details The user is responsible for allocating it using the FILEPICKER_CONTEXT_OUTPUT_ALLOC() macro.
     */
    void *paths;
    /**
     * @brief Start path.
     * @details Can be overridden by the user before invoking the file picker.
     */
    char *start_path;
    /**
     * @brief Unknown.
     */
    void *unk_0x18;
    /**
     * @brief Unknown.
     */
    void *unk_0x1c;
    /**
     * @brief Unknown.
     */
    char *unk_0x20;
    /**
     * @brief Seems to be used to store some kind of error text.
     */
    void *error_text;
    /**
     * @brief Unknown.
     */
    int unk_0x28;
    /**
     * @brief Unknown.
     */
    short unk_0x2c;
    /**
     * @brief Max size of the type list.
     */
    unsigned short type_list_max_size;
    /**
     * @brief Max size of each path in code units.
     * @details _GetOpenFileName() seems to compare this value to the length of the first #paths string and will clear
     * #paths if the length is larger or equal to this value. Use FILEPICKER_CONTEXT_OUTPUT_MAX_LFN if starting the
     * file picker in LFN mode.
     */
    unsigned short path_max_cu;
    /**
     * @brief Unknown.
     */
    unsigned short unk_0x32;
    /**
     * @brief Used by _GetNextFileName() to track current index of path being read.
     */
    unsigned short current_path_idx;
    /**
     * @brief Number of paths expected.
     * @details
     *
     * Can be overridden by the user before invoking the file picker.
     *
     * After invoking the file picker, this will be set to the number of available paths picked by the user.
     */
    unsigned short npaths;
    /**
     * @brief Flags.
     */
    unsigned int flags;
    /**
     * @brief Unknown.
     */
    int *unk_0x3c;
    /**
     * @brief Unknown.
     */
    int unk_0x40;
    /**
     * @brief Unknown.
     */
    void *unk_0x44;
    /**
     * @brief Unknown.
     */
    short unk_0x48;
    /**
     * @brief Unknown.
     */
    char unk_0x4a;
    /**
     * @brief Unknown.
     */
    char unk_0x4b;
    /**
     * @brief Unknown.
     */
    char unk_0x4c;
    /**
     * @brief Unknown.
     */
    char unk_0x4d;
    /**
     * @brief Unknown.
     */
    char unk_0x4e;
    /**
     * @brief Unknown.
     */
    char unk_0x4f;
    /**
     * @brief Status of the file picker.
     * @todo Document what each status meant.
     */
    unsigned short status;
    /**
     * @brief Unknown.
     */
    short unk_0x52;
};

/**
 * @brief Show a file picker in open mode.
 * @x_syscall_num `0x101d2`
 * @param ctx The file picker context.
 * @return true @x_term ok
 * @return false @x_term ng
 */
extern bool _GetOpenFileName(filepicker_context_t *ctx);

/**
 * @brief Show a file picker in save mode.
 * @x_syscall_num `0x101d3`
 * @param ctx The file picker context.
 * @return true @x_term ok
 * @return false @x_term ng
 */
extern bool _GetSaveFileName(filepicker_context_t *ctx);

/**
 * @brief Read the next path from a context struct.
 * @x_syscall_num `0x101d4`
 * @param ctx The file picker context.
 * @param[out] out UTF16 string. Recommended to be at least 260 CUs long.
 * @retval 0 @x_term ok
 * @retval 1 @x_term ng
 */
extern int _GetNextFileName(filepicker_context_t *ctx, UTF16 *out);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_MODALS_FILEPICKER_H__
