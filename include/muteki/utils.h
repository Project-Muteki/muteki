/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file utils.h
 * @brief Miscellaneous utility syscalls.
 * @details Contains syscalls that cannot be easily grouped with other syscalls.
 */

#ifndef __MUTEKI_UTILS_H__
#define __MUTEKI_UTILS_H__

#include <muteki/common.h>
#include <muteki/ui/canvas.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get the descriptor of the currently active LCD surface.
 * @details This is functionally equivalent to the following:
 * @code{.c}
 * lcd_surface_t *GetActiveVRamAddress() {
 *     return GetActiveLCD()->surface;
 * }
 * @endcode
 * @x_term require-krnllib
 * @x_syscall_num `0x20022`
 * @x_void_param
 * @return The LCD surface descriptor.
 */ 
extern lcd_surface_t *GetActiveVRamAddress(void);

/**
 * @brief Play a beep using the built in piezo speaker.
 * @details This usually plays a 4000Hz tone for 10ms.
 * @x_syscall_num `0x1009e`
 * @x_void_param
 * @retval true @x_term ok
 */
extern bool WarningBeep(void);

/**
 * @brief Format and send a message to debug UART port.
 * @details Supports all conversion specifiers Printf() and PrintfXY() supports. This internally calls an
 * implementation of `vsnprintf()` to generate the final message.
 * @note This function may be stubbed on some devices and UART test point availability vary between devices.
 * Moreover, the size of the internal buffer used by `vsnprintf()` to save formatted message might also be
 * device-specific. On BA742 this buffer is `0x800` bytes long, with `0x7ff` bytes actually available to the
 * formatted message.
 * @x_syscall_num `0x102a1`
 * @param format The format string.
 * @param ... Any subsequent values.
 * @x_void_return
 */
extern void WriteComDebugMsg(char *format, ...) SYS_IS_PRINTF(1, 2);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UTILS_H__
