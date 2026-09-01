/*
 * Copyright 2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file id.h
 * @brief Device identification and information utilities.
 */

#ifndef __MUTEKI_ID_H__
#define __MUTEKI_ID_H__

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Hardware feature masks.
 * @details A bit `1` means that the corresponding hardware feature is NOT present on the device.
 */
enum id_device_feature_mask_e {
    /**
     * @brief Device does not have a touch panel and does not support touch screen operation.
     */
    DEVICE_FEATURE_NO_TOUCH_PANEL = 0x0001,
    /**
     * @brief Device does not have a hardware keyboard and/or prefer to use a software keyboard.
     */
    DEVICE_FEATURE_NO_HW_KEYBOARD = 0x0002,
    /**
     * @brief Device does not have physical F-keys and alternative keys/menus must be used in place of them.
     */
    DEVICE_FEATURE_NO_FKEY = 0x0008,
};

/**
 * @brief Device info.
 */
typedef struct id_device_info_s {
    /**
     * @brief Name of the installed system data partition.
     */
    char data_name[16];
    /**
     * @brief @x_term{sbz}
     */
    unsigned char sbz_0x10[8];
    /**
     * @brief Installed system data partition version as shown in the diagnostics menu.
     */
    char data_version[16];
    /**
     * @brief Installed ROM/kernel partition version as shown in the diagnostics menu.
     */
    char rom_version[16];
    /**
     * @brief Disabled hardware features.
     * @see id_device_feature_mask_e
     */
    unsigned short feature_masks;
    /**
     * @brief Device serial number.
     */
    char serial_number[16];
    /**
     * @brief Device default language code/region.
     */
    unsigned short default_language_code;
    /**
     * @brief System data partition type. Should be `0x801d`
     */
    unsigned short data_type;
    /**
     * @brief Installed operating system version.
     */
    char os_version[16];
    /**
     * @brief Name of the installed ROM/kernel partition.
     */
    char rom_name[16];
    /**
     * @brief Unknown. Seems to be constant 1.
     */
    short unk_0x6e;
    /**
     * @brief Unknown. Seems to be constant 1.
     */
    short unk_0x70;
    /**
     * @brief @x_term{sbz}
     */
    unsigned char sbz_0x72[0x3a];
} id_device_info_t;

/**
 * @brief Read the device info.
 * @x_syscall_num{0x100c1}
 * @param out_info Pointer to the output device info struct.
 * @return @p out_info if successful, `NULL` if not.
 */
id_device_info_t *GetMasterIDInfo(id_device_info_t *out_info);

/**
 * @brief Read the device serial number.
 * @warning The caller must zero-alloc the buffer to ensure that the string is properly terminated.
 * @x_syscall_num{0x102c8}
 * @param serial_out Output buffer that will hold the serial number.
 * @return Number of bytes read.
 */
size_t GetMasterSerialNumber(char serial_out[17]);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_ID_H__
