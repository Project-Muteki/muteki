/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file devio.h
 * @brief Device IO API.
 */

#ifndef __MUTEKI_DEVIO_H__
#define __MUTEKI_DEVIO_H__

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Magic for device IO descriptor.
 * @details The value is ASCII `"BXCH"`.
 */
#define DEVIO_DESC_MAGIC (0x48435842u)

/**
 * @brief Catchall device IO backend type.
 * @todo Add more details.
 */
typedef void devio_backend_t;

/**
 * @brief The device IO descriptor.
 */
typedef struct {
    /**
     * @brief Path to the device or DOS 8.3 path to the file.
     */
    char path[80];
    /**
     * @brief Backend of this descriptor. Can be a file descriptor or a service.
     */
    devio_backend_t *backend;
    /**
     * @brief Unknown.
     */
    int unk_0x54;
    /**
     * @brief Device ID if this descriptor is device-backed.
     */
    unsigned int device_id;
    /**
     * @brief Reference counter.
     */
    int refcount;
    /**
     * @brief Access mode.
     */
    int access;
    /**
     * @brief Sharing mode.
     */
    int shmode;
    /**
     * @brief Behavior on no entry.
     */
    int on_noentry;
    /**
     * @brief Upper 16-bit of the flags.
     */
    unsigned int flags_upper;
    /**
     * @brief Lower 16-bit of the flags.
     */
    unsigned int flags_lower;
    /**
     * @brief Descriptor magic. Always `BXCH`.
     */
    unsigned int magic;
    /**
     * @brief Unknown.
     */
    int unk_0x78;
} devio_descriptor_t; // 0x7c bytes

// TODO stubbed definition. Fully implement this later.
typedef char device_service_t[0x4c];

/**
 * @brief Invalid descriptor.
 * @details This is usually returned by CreateFile() when an error occurred.
 */
#define DEVIO_DESC_INVALID ((devio_descriptor_t *) 0xffffffff)

/**
 * @brief Open or create a file/device by its `pathname`.
 * @details Analogous to the Windows CreateFile() API.
 * @todo Documentation on accepted arguments are mostly based on speculations at this moment. More details and through
 * verifications needed.
 * @x_syscall_num{0x102a7}
 * @param pathname DOS 8.3 path to the file to be opened.
 * @param access Access flags.
 * @param shmode File sharing mode. (?)
 * @param secattr Seems to be ignored.
 * @param on_noentry Behavior when entry does not exist. (?)
 * @param flags Flags and attributes of the file. (?)
 * @param template_file Seems to be ignored.
 * @return Device IO descriptor.
 */
extern devio_descriptor_t *CreateFile(
    const char *pathname,
    unsigned int access,
    unsigned int shmode,
    void *secattr,
    unsigned int on_noentry,
    unsigned int flags,
    void *template_file
);

/**
 * @brief Read/receive data from a device IO descriptor.
 * @details Analogous to the Windows ReadFile() API.
 * @x_syscall_num{0x102a9}
 * @param devfd The device IO descriptor.
 * @param[out] buf Buffer to hold data read from the descriptor.
 * @param size Number of bytes to read.
 * @param[out] actual_size Number of bytes actually read.
 * @param overlapped Reserved.
 * @retval true @x_term{ok}
 * @retval false @x_term{ng}
 */
extern bool ReadFile(devio_descriptor_t *devfd, void *buf, size_t size, size_t *actual_size, void *overlapped);

/**
 * @brief Write/send data to a device IO descriptor.
 * @details Analogous to the Windows WriteFile() API.
 * @x_syscall_num{0x102aa}
 * @param devfd The device IO descriptor.
 * @param[in] buf Buffer to hold data to be written to the descriptor.
 * @param size Number of bytes to write.
 * @param[out] actual_size Number of bytes actually written.
 * @param overlapped Reserved.
 * @retval true @x_term{ok}
 * @retval false @x_term{ng}
 */
extern bool WriteFile(devio_descriptor_t *devfd, const void *buf, size_t size, size_t *actual_size, void *overlapped);

/**
 * @brief Send an IOCTL request to a device IO descriptor `devfd`.
 * @details Analogous to the Windows DeviceIoControl() API.
 * @x_syscall_num{0x102ac}
 * @param devfd Device IO descriptor.
 * @param request IOCTL request type.
 * @param in The input buffer, or NULL if not applicable.
 * @param inlen The size of the input buffer in bytes.
 * @param out The output buffer, or NULL if not applicable.
 * @param outlen The size of the output buffer in bytes.
 * @param retlen The size of returned data (if applicable).
 * @param overlapped Probably ignored.
 * @retval true @x_term{ok}
 * @retval false @x_term{ng}
 */
extern bool DeviceIoControl(
    devio_descriptor_t *devfd,
    int request,
    const void *in,
    int inlen,
    void *out,
    int outlen,
    int *retlen,
    void *overlapped
);

/**
 * @brief Close a device IO descriptor.
 * @details Analogous to the Windows CloseHandle() API.
 * @x_syscall_num{0x102ad}
 * @param devfd Descriptor to close.
 * @retval true @x_term{ok}
 * @retval false @x_term{ng}
 */
extern bool CloseHandle(devio_descriptor_t *devfd);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_DEVIO_H__urn Whether the operation was successful.
