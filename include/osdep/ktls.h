/*
 * Copyright 2023-2025 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file ktls.h
 * @brief Kernel TLS (KTLS) control.
 * @details This is what TLSv1 used as the backend. Provided here in case one wants to hold data across applets/modules
 * within the same thread.
 */
#ifndef __OSDEP_KTLS_H__
#define __OSDEP_KTLS_H__

#include <muteki/threading.h>

#ifdef __cplusplus
extern "C" {
#endif

static const unsigned int OSDEP_KTLS_KEY_MAX = sizeof(((bxc_thread_t *) NULL)->unk_0x34) / 4 - 1;

/**
 * @brief Initialize the TLS container on a specific thread.
 *
 * @param thr Pointer to a thread descriptor.
 * @retval 0 @x_term{ok}
 */
extern int osdep_ktls_init(bxc_thread_t *thr);

/**
 * @brief Get the pointer to the TLS slot on the thread descriptor.
 *
 * @param thr Pointer to a thread descriptor.
 * @param key Numerical key. Must be in the range of `(0, 8)`.
 * @return Pointer to the TLS slot. Or `NULL` when an invalid key was supplied.
 */
extern void **osdep_ktls_get(bxc_thread_t *thr, unsigned int key);

/**
 * @brief Get the value stored in the TLS slot on the thread descriptor.
 *
 * @param thr Pointer to a thread descriptor.
 * @param key Numerical key. Must be in the range of `(0, 8)`.
 * @return Value stored in the TLS slot. Or `NULL` when an error occurres.
 */
extern void *osdep_ktls_getvalue(bxc_thread_t *thr, unsigned int key);

/**
 * @brief Store a value in the TLS slot.
 * @warning If osdep_ktls_alloc() was called on this previously, the memory won't be freed properly.
 *
 * @param thr Pointer to a thread descriptor.
 * @param key Numerical key. Must be in the range of `(0, 8)`.
 * @param value Value to be stored into the TLS slot.
 * @retval 0 @x_term{ok}
 * @retval -1 @x_term{ng}
 */
extern int osdep_ktls_set(bxc_thread_t *thr, unsigned int key, void *value);

/**
 * @brief Allocate memory and store the resulting pointer in the TLS slot.
 *
 * @param thr Pointer to a thread descriptor.
 * @param key Numerical key. Must be in the range of `(0, 8)`.
 * @param bytes Number of bytes to allocate.
 * @return Allocated buffer, or NULL if the slot is not NULL, or if the allocation fails.
 */
extern void *osdep_ktls_alloc(bxc_thread_t *thr, unsigned int key, size_t bytes);

/**
 * @brief Free memory previously allocated by osdep_ktls_alloc().
 *
 * @param thr Pointer to a thread descriptor.
 * @param key Numerical key. Must be in the range of `(0, 8)`.
 * @retval 0 @x_term{ok}
 */
extern int osdep_ktls_free(bxc_thread_t *thr, unsigned int key);

/**
 * @brief Initialize the TLS container on the current thread.
 *
 * @x_void_param
 * @retval 0 @x_term{ok}
 */
extern int osdep_ktls_init_self(void);

/**
 * @brief Get the pointer to the TLS slot on the current thread.
 *
 * @param key Numerical key. Must be in the range of `(0, 8)`.
 * @return Pointer to the TLS slot. Or `NULL` when an invalid key was supplied.
 */
extern void **osdep_ktls_get_self(unsigned int key);

/**
 * @brief Get the value stored in a TLS slot of the current thread.
 *
 * @param key Numerical key. Must be in the range of `(0, 8)`.
 * @return void* 
 */
extern void *osdep_ktls_getvalue_self(unsigned int key);

/**
 * @brief Store a value in the TLS slot on the current thread.
 *
 * @param key Numerical key. Must be in the range of `(0, 8)`.
 * @param value Value to be stored into the TLS slot.
 * @retval 0 @x_term{ok}
 * @retval -1 @x_term{ng} 
 */
extern int osdep_ktls_set_self(unsigned int key, void *value);

/**
 * @brief Allocate memory and store the resulting pointer in a TLS slot of the current thread.
 *
 * @param key Numerical key. Must be in the range of `(0, 8)`.
 * @param bytes Number of bytes to allocate.
 * @return Allocated buffer, or NULL if the slot is not NULL, or if the allocation fails.
 */
extern void *osdep_ktls_alloc_self(unsigned int key, size_t bytes);

/**
 * @brief Free memory previously allocated by osdep_ktls_alloc().
 *
 * @param key Numerical key. Must be in the range of `(0, 8)`.
 * @retval 0 @x_term{ok} 
 */
extern int osdep_ktls_free_self(unsigned int key);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // __OSDEP_KTLS_H__
