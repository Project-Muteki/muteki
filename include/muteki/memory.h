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

#include <muteki/common.h>

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
 * @brief Allocate memory on the secondary heap.
 * @details
 * This allocates a block of memory on a secondary heap, that may be backed by a large block of memory on the primary
 * heap. This secondary heap is usually 64KiB in size.
 *
 * This seems to have a tie to the UI subsystem as UI-related syscalls seem to use this extensively.
 * 
 * @warning Memory allocated with this function has to be freed with the FreeBlock() call.
 *
 * @x_syscall_num `0x10145`
 *
 * @param size Size of the memory.
 * @param tag Write this value to `<allocated address> + 0x30` when the allocated memory is located on a fresh
 * chunk (i.e. allocated on previously unused memory directly following the last chunk). Seems to be used in UI
 * subsystem to differentiate components in some circumstances.
 * @param new_segment Set to `true` to start a new segment. This will prevent this chunk and all chunks above from
 * being reused until this memory is freed.
 * @return The allocated memory.
 */
extern void *AllocBlock(size_t size, unsigned short tag, bool new_segment);

/**
 * @brief Free memory previously allocated with AllocBlock().
 * @details If a segment chunk is freed, everything below it will also be implicitly freed.
 * @x_syscall_num `0x10146`
 * @param ptr Pointer to previously allocated memory.
 * @x_void_return
 */
extern void FreeBlock(void *ptr);

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
