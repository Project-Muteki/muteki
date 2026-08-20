/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file memory.h
 * @brief Memory management API.
 * @details
 * In Besta RTOS, heap memory management is entirely done in the OS kernel.
 * All applications share a single heap managed by the OS and can be accessed
 * through the malloc series API. Conventional address-space based heap memory
 * management such as sbrk() is not supported due to the absence of MMU support
 * in the OS kernel.
 *
 * The simplest way to get heap memory support in a custom libc is to redirect
 * malloc(), calloc(), realloc() and free() to the corresponding functions
 * provided here.
 */

#ifndef __MUTEKI_MEMORY_H__
#define __MUTEKI_MEMORY_H__

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Allocate heap memory.
 * @param size Size of the allocation in bytes.
 * @return Pointer to allocated memory, or `NULL` if allocation fails.
 */
extern void *lmalloc(size_t size);

/**
 * @brief Allocate and clear data units on the heap.
 * @param nmemb Number of data units to allocate.
 * @param size Size of each data unit.
 * @return Pointer to allocated memory, or `NULL` if allocation fails.
 */
extern void *lcalloc(size_t nmemb, size_t size);

/**
 * @brief Resize or reallocate memory.
 * @param ptr Pointer to previously allocated memory.
 * @param size New size in bytes.
 * @return Pointer to the newly allocated memory, or `NULL` if allocation fails.
 */
extern void *lrealloc(void *ptr, size_t size);

/**
 * @brief Free previously allocated memory.
 * @param ptr Pointer to previously allocated memory.
 * @x_void_return
 */
extern void _lfree(void *ptr);

/**
 * @brief Return the size of available memory.
 * @details @x_term require-krnllib
 * @x_void_param
 * @return Size of available memory.
 */
extern size_t GetFreeMemory(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_MEMORY_H__
