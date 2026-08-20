/*
 * Copyright 2025 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file utls.h
 * @brief Userspace TLS (UTLS) control.
 * @details A hash-table-backed TLS storage emulator local to the current module. Include this header to control the
 * lifecycle of UTLS.
 */

#ifndef __OSDEP_UTLS_H__
#define __OSDEP_UTLS_H__

#include <muteki/threading.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Statistics of TLS allocation
 */
typedef struct osdep_utls_stats_s {
    /**
     * @brief If true, TLS container is initialized.
     */
    bool is_initialized;
    /**
     * @brief Number of slots used. Only valid when ::is_initialized is true.
     */
    size_t slots_used;
    /**
     * @brief Number of slots already allocated for the container. Only valid when ::is_initialized is true.
     */
    size_t slots_allocated;
} osdep_utls_stats_t;

/**
 * @brief Manually initialize UTLS container.
 * @details This is normally not needed as the __aeabi_read_tp() implementation will automatically call this on first
 * use of any TLS space.
 *
 * @x_void_param
 * @x_void_return
 */
extern void osdep_utls_cinit(void);

/**
 * @brief Manually destroy the UTLS container.
 * @details Call this from the destructor code on module unload when not stay resident.
 *
 * @x_void_param
 * @x_void_return
 */
extern void osdep_utls_cfini(void);

/**
 * @brief Get the TLS space by thread descriptor pointer.
 * @note This does not allocate extra space when `thr` does not already have TLS allocated for it.
 * @param thr Thread pointer.
 * @return TLS space, or `NULL` if not yet allocated.
 */
extern void *osdep_utls_peek(const bxc_thread_t *thr);

/**
 * @brief Get statistics of TLS allocation.
 * @param stats The output stats buffer.
 * @x_void_return
 */
extern void osdep_utls_get_stats(osdep_utls_stats_t *stats);
#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // __OSDEP_UTLS_H__
