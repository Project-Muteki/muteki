/*
 * Copyright 2025 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file heap.h
 * @brief Heap memory management function hooks.
 *
 * The default allocator Besta RTOS provides aligns the memory it allocates to
 * 4-bytes boundary, however the AAPCS requires memory to be aligned to 8-bytes
 * boundary. This header file provides hooked version of allocator functions
 * like `malloc` and `free`, which align the memory to 8-bytes boundary by
 * inserting spaces conditionally, and also optionally traces the allocation
 * done by the hooked functions.
 */

#ifndef __OSDEP_HEAP_H__
#define __OSDEP_HEAP_H__

#include <muteki/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Allocate and format mchx memchunk.
 *
 * @param size Size of the memory to allocate.
 * @return Pointer to allocated memory, or NULL if allocation fails.
 */
__attribute__((assume_aligned(8))) extern void *osdep_heap_alloc(size_t size);

/**
 * @brief Get usable size of an allocated mchx memchunk.
 *
 * @param ptr Memory allocated by osdep_heap_alloc().
 * @return Size of usable memory.
 */
size_t osdep_heap_get_alloc_size(const void *ptr);

/**
 * @brief Free a mchx memchunk.
 *
 * @param ptr Pointer to previously allocated memory.
 * @x_void_return
 */
extern void osdep_heap_free(void *ptr);

/**
 * @brief Start the heap tracer.
 *
 * @x_void_param
 * @retval true @x_term ok
 * @retval false @x_term ok
 */
extern bool osdep_heap_trace_start(void);

/**
 * @brief Stop the heap tracer.
 *
 * @x_void_param
 * @retval true @x_term ok
 * @retval false @x_term ok
 */
extern bool osdep_heap_trace_stop(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __OSDEP_HEAP_H__
