/*
 * Copyright 2025 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file threading.h
 * @brief Threading tools.
 */

#ifndef __OSDEP_THREADING_H__
#define __OSDEP_THREADING_H__

#include <muteki/threading.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get the current running thread.
 * @x_void_param
 * @return Current running thread.
 */
extern bxc_thread_t *osdep_thread_get_current(void);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // __OSDEP_THREADING_H__
