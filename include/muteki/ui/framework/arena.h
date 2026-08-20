/*
 * Copyright 2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file arena.h
 * @brief UI arena allocator functions.
 * @details Allocate/deallocate UI components using the UI arena allocator.
 */

#ifndef __MUTEKI_UI_FRAMEWORK_COMPONENT_H__
#define __MUTEKI_UI_FRAMEWORK_COMPONENT_H__

#include "../types/framework.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Allocate memory on the secondary heap.
 * @details
 * This allocates a block of memory on the UI memory arena, that is backed by a large block of memory on the system
 * heap. This arena is usually 64KiB in size.
 *
 * @warning Memory allocated with this function has to be freed with the FreeBlock() call.
 *
 * @x_syscall_num `0x10145`
 *
 * @param size Size of the memory.
 * @param tag Write this value to `<allocated address> + 0x28` (`<chunk address> + 0x30`) when the allocated memory is
 * located on a fresh chunk (i.e. allocated on previously unused memory directly following the last chunk).
 * @param new_segment Set to `true` to start a new segment. This will prevent this chunk and all chunks above from
 * being reused until this memory is freed.
 * @return The allocated memory.
 */
extern ui_component_t *AllocBlock(size_t size, unsigned short tag, bool new_segment);

/**
 * @brief Free memory previously allocated with AllocBlock().
 * @details If a segment chunk is freed, everything below it will also be implicitly freed.
 * @x_syscall_num `0x10146`
 * @param ptr Pointer to previously allocated memory.
 * @x_void_return
 */
extern void FreeBlock(ui_component_t *ptr);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_FRAMEWORK_COMPONENT_H__
