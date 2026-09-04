/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file errno.h
 * @brief Besta error code constants and functions.
 *
 */

#ifndef __MUTEKI_ERRNO_H__
#define __MUTEKI_ERRNO_H__

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Kernel error code type */
typedef unsigned int bxc_errno_t;

/**
 * @brief Flags for FormatMessage()
 * @see https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-formatmessage
 */
enum bxc_message_flags_e {
    /** Allocate buffer for the message. */
    BXC_FORMAT_MESSAGE_ALLOCATE_BUFFER = 0x100,
    /** Use system error message string table to generate the message. */
    BXC_FORMAT_MESSAGE_FROM_SYSTEM = 0x1000,
};

/**
 * @brief Kernel errno namespace.
 */
enum bxc_errno_ns_flag_e {
    /** User (errors set by OSSetLastError()). */
    BXC_ERRNO_NS_USER = 0x2,
    /** Kernel. */
    BXC_ERRNO_NS_KERNEL = 0x4,
    /** Executable loader. */
    BXC_ERRNO_NS_EXEC = 0x8,
    /** Applet specific (errors set by _SetLastError()). */
    BXC_ERRNO_NS_APPLET = 0x2000,
};

/**
 * @brief Error code for ::BXC_ERRNO_NS_EXEC.
 */
enum bxc_errno_exec_e {
    /**
     * @brief Unsupported executable type.
     * @details This may be returned when attempting to execute a TLCS-900 executable or x86 EXE file on Arm systems.
     * @todo Double check.
     */
    BXC_ERR_EXEC_UNSUPPORTED = 0x0001,
    /**
     * @brief Invalid executable file.
     * @details This may be returned when attempting to execute a data-only ROM.
     * @todo Double check.
     */
    BXC_ERR_EXEC_INVALID,
    /**
     * @brief Failed to open executable file.
     */
    BXC_ERR_EXEC_OPEN_FAILED,
    /**
     * @brief Failed to parse executable file.
     */
    BXC_ERR_EXEC_LOADER_FAILURE,
    /**
     * @brief Failed to allocate memory when loading executable file.
     */
    BXC_ERR_EXEC_MALLOC_FAILED,
    /**
     * @brief Decompression failed.
     */
    BXC_ERR_EXEC_DECOMPRESSION_FAILED,
    /**
     * @brief Refuse to load an executable file because MP3 player is running in the background.
     * @details Probably an left-over from the TLCS-900 days when MP3 can be played in the background.
     */
    BXC_ERR_EXEC_MP3_PLAYER_IS_RUNNING,
    /**
     * @brief Generic error.
     * @details This is usually shown when the loader functions encountered an internal error.
     */
    BXC_ERR_EXEC_GENERIC_ERROR,
    /**
     * @brief Executable mapping table is full.
     * @details This is returned when too many executable is loaded. May be overwritten by a more generic code like
     * #BXC_ERR_EXEC_LOADER_FAILURE.
     */
    BXC_ERR_EXEC_MAP_FULL,
    BXC_ERR_EXEC_UNKNOWN_FORMAT = 0x000a,
    /**
     * @brief Failed to load executable file due to an error originated from the header parser.
     */
    BXC_ERR_EXEC_BESTAPE_HEADER_PARSER_FAILURE = 0x000f,
    /**
     * @brief Failed to load executable file due to an error originated from the shared object loader.
     */
    BXC_ERR_EXEC_BESTAPE_LD_FAILURE = 0x0010,
    /**
     * @brief Failed to load executable file due to an error originated from the relocator.
     */
    BXC_ERR_EXEC_BESTAPE_RELOC_FAILURE = 0x0011,
    /**
     * @brief Invalid pathname.
     */
    BXC_ERR_EXEC_INVALID_PATH = 0x0012,
    /**
     * @brief Module is not loaded.
     */
    BXC_ERR_EXEC_MODULE_NOT_LOADED = 0x0013,
    /**
     * @brief Executable is not loaded.
     */
    BXC_ERR_EXEC_NOT_LOADED = 0x0016,
};

/**
 * @brief Error code for ::BXC_ERRNO_NS_USER.
 */
enum bxc_errno_user_e {
    /** Message queue is busy. */
    BXC_ERR_THREADING_QUEUE_BUSY = 0x000a,
    /** Message queue is full. */
    BXC_ERR_THREADING_QUEUE_FULL = 0x001e,
    /** Message queue is empty. */
    BXC_ERR_THREADING_QUEUE_EMPTY = 0x001f,
    /** Thread slot is in use. */
    BXC_ERR_THREADING_SLOT_IN_USE = 0x0028,
    /** Non-reserved slots are full. Cannot create thread. */
    BXC_ERR_THREADING_SLOT_FULL = 0x002b,
    /** Failed to allocate thread struct. */
    BXC_ERR_THREADING_STRUCT_MALLOC_FAILED = 0x0046,
    /** Thread is not sleeping. */
    BXC_ERR_THREADING_THREAD_NOT_SLEEPING = 0x0050,
    /** Thread is not suspended. */
    BXC_ERR_THREADING_THREAD_NOT_SUSPENDED = 0x0065,
    /** Failed to allocate thread stack. */
    BXC_ERR_THREADING_STACK_MALLOC_FAILED = 0x0066,
    /** Invalid thread descriptor magic. */
    BXC_ERR_THREADING_INVALID_DESCRIPTOR = 0x006e,
    /** Descriptor is busy. Seen on event descriptors when some threads are waiting on the event. */
    BXC_ERR_THREADING_DESCRIPTOR_BUSY = 0x008c,
};

/**
 * @brief Error code for ::BXC_ERRNO_NS_KERNEL.
 */
enum bxc_errno_kernel_e {
    /** Peripheral communication failed. */
    BXC_ERR_PERIPHERAL_COMM_FAILED = 0x0003,
    /** General hardware error. */
    BXC_ERR_GENERAL_HW_ERROR,

    /** FTL: Data corruption detected. */
    BXC_ERR_FTL_DATA_CORRUPTED = 0x0010,
    /** FTL: Data corruption detected and error correction attempt failed. */
    BXC_ERR_FTL_ECC_FAILED,
    /** FTL: Data corruption detected and error correction attempt succeeded. */
    BXC_ERR_FTL_ECC_TRIGGERED = 0x0018,
    /** FTL: Invalid Logical Block Address. */
    BXC_ERR_FTL_INVALID_LBA = 0x0021,
    /** Medium is write protected. */
    BXC_ERR_MEDIUM_WP_ENABLED = 0x0027,
    /** Medium changed */
    BXC_ERR_MEDIUM_CHANGED,
    /** Medium is of an incompatible type. */
    BXC_ERR_MEDIUM_INCOMPATIBLE = 0x0030,
    /** Medium I/O error. */
    BXC_ERR_MEDIUM_ERROR,
    /** Medium is not loaded. */
    BXC_ERR_MEDIUM_UNLOADED = 0x003a,

    /** Storage device not found. */
    BXC_ERR_SYS_STORAGE_DEVICE_NOT_FOUND = 0x0060,
    /** Erase of storage device failed. */
    BXC_ERR_SYS_ERASE_FAILED,
    /** Out of memory. */
    BXC_ERR_SYS_OUT_OF_MEMORY,
    /** Low battery. */
    BXC_ERR_SYS_LOW_BATTERY,
    /** Lock switch is on. System is locked. */
    BXC_ERR_SYS_LOCK_SWITCH_ENABLED,

    /** Block device format prompt. */
    BXC_ERR_MKFS_PROMPT = 0x0102,
    /** Block device needs formatting. */
    BXC_ERR_MKFS_UNSUPPORTED,
    /** mkfs: No space left for device. */
    BXC_ERR_MKFS_NO_SPACE_LEFT,
    /** mkfs: Mode error. */
    BXC_ERR_MKFS_MODE_ERROR,
    /** mkfs: I/O error. */
    BXC_ERR_MKFS_IO_ERROR,

    /** Invalid drive letter */
    BXC_ERR_FS_INVALID_DRIVE_LETTER = 0x0113,

    /** Filename contains invalid characters. */
    BXC_ERR_FS_INVALID_FILENAME = 0x0140,
    /** General filesystem I/O error. */
    BXC_ERR_FS_OPERATION_ERROR,
    /** File or directory exists. */
    BXC_ERR_FS_ENTRY_EXISTS,
    /**
     * @brief Directory/mount record cannot grow.
     */
    BXC_ERR_FS_DIR_RECORD_GROWTH,
    /**
     * @brief No such file or directory.
     * @details This is raised when a file/directory cannot be found, or attempting to access an unmounted drive.
     * @see ::BXC_ERR_FS_PATH_TRAVERSAL Raised when entries cannot be found due to directory traversal failures.
     */
    BXC_ERR_FS_NO_SUCH_ENTRY,
    /** File/directory is not available. */
    BXC_ERR_FS_FILE_UNAVAILABLE,
    /**
     * @brief Seeking a subfile out-of-bound.
     * @details Only seems to be used by the loader's subfile API.
     */
    BXC_ERR_FS_SUBFILE_OOB_SEEK,
    /** Conflicting file/directory attributes */
    BXC_ERR_FS_CONFLICTING_ATTR,
    /** Too many open files. */
    BXC_ERR_FS_TOO_MANY_OPEN_FILES,
    /** File/directory is locked for exclusive access. */
    BXC_ERR_FS_FILE_LOCKED,
    /** Attribute error. */
    BXC_ERR_FS_FILE_ATTR_ERROR,
    /** No space left for device. */
    BXC_ERR_FS_NO_SPACE_LEFT,

    /**
     * @brief Path traversal failure.
     * @details This is raised when attempting to access a file but the path traversal fails at a directory, or wrong
     * type of node is found during path traversal (i.e. expecting a directory but a file is found, and vice-versa).
     * @see ::BXC_ERR_FS_NO_SUCH_ENTRY Raised when no entry can be found, but not caused by a traversal failure.
     */
    BXC_ERR_FS_PATH_TRAVERSAL = 0x0154,
    /**
     * @brief File is read-only.
     * @details File descriptor does not support read function.
     */
    BXC_ERR_FS_READ_ONLY_FILE = 0x0158,
    /**
     * @brief File is write-only.
     * @details File descriptor does not support write function.
     */
    BXC_ERR_FS_WRITE_ONLY_FILE,
    /** Path too long. */
    BXC_ERR_FS_PATH_TOO_LONG = 0x0162,
    /**
     * @brief Generic error.
     * @details This is usually shown when the file and filesystem functions encountered an internal error, or that the
     * supplied file descriptor is invalid.
     */
    BXC_ERR_FS_GENERIC_ERROR = 0x0163,
    /**
     * @brief EOF reached when accessing (R/W) a file.
     * @note This is usually handled internally by either truncating the result or allocating more space. This could
     * explain why the strerror message of this errno seems to indicate that it is a "record overflow" condition, since
     * this errno may be visible by the top caller when automatic file growth fails.
     */
    BXC_ERR_FS_EOF_REACHED = 0x0165,
    /**
     * @brief FAT16 directory entry allocation failure.
     * @details Seems to be raised when the FAT16 driver cannot find an empty slot to put a new directory entry in.
     */
    BXC_ERR_FS_FAT16_DIR_ALLOC,

    /** Database is corrupted. */
    BXC_ERR_DB_CORRUPTED = 0x0200,
    /** Failed to open database. */
    BXC_ERR_DB_OPEN_FAILED,
    /** Database index is full. Suggesting entry cleanup. */
    BXC_ERR_DB_INDEX_FULL_DELETE = 0x0203,
    /** Database is full. */
    BXC_ERR_DB_FULL,
    /** Database index is full. Suggesting sync with PC. */
    BXC_ERR_DB_INDEX_FULL_SYNC,
    /** Database: too many open files. */
    BXC_ERR_DB_TOO_MANY_OPEN_FILES,
};

/**
 * @brief Get the namespace portion of raw kernel errno value.
 * @param kerrno Kernel errno value.
 * @return The namespace.
 * @see bxc_errno_ns_flag_e
 */
static inline unsigned short BXC_ERRNO_NS(bxc_errno_t kerrno) {
    return kerrno >> 16;
}

/**
 * @brief Get the error code portion of raw kernel errno value.
 * @param kerrno Kernel errno value.
 * @return The error code.
 * @see bxc_errno_exec_e
 * @see bxc_errno_kernel_e
 */
static inline unsigned short BXC_ERRNO_ERR(bxc_errno_t kerrno) {
    return kerrno & 0xffff;
}

/**
 * @brief Helper function that builds an errno from namespace and error code.
 * 
 * @param ns Namespace.
 * @param err Error code.
 * @return The errno.
 */
static inline bxc_errno_t BXC_ERRNO(unsigned short ns, unsigned short err) {
    return (ns << 16) | err;
}

/**
 * @brief Fetch an error message.
 * @details Fetch error descriptions (encoded in ::UTF16) for last set error in a similar manner to `FormatMessage`
 * API in Windows.
 * @param flags Flags. Only ::BXC_FORMAT_MESSAGE_FROM_SYSTEM and ::BXC_FORMAT_MESSAGE_ALLOCATE_BUFFER are supported.
 * In addition, ::BXC_FORMAT_MESSAGE_FROM_SYSTEM must be set.
 * @param _sbz0 @x_term{sbz}
 * @param _sbz1 @x_term{sbz}
 * @param _sbz2 @x_term{sbz}
 * @param outbuf The output buffer.
 * If using ::BXC_FORMAT_MESSAGE_ALLOCATE_BUFFER, the pointer to the allocated buffer will be placed at this location.
 * Otherwise the message will be written directly to this location. It's caller's responsibility to free any buffer
 * allocated by ::BXC_FORMAT_MESSAGE_ALLOCATE_BUFFER.
 * @param outlen When not using ::BXC_FORMAT_MESSAGE_ALLOCATE_BUFFER, this is the length of the message buffer. Otherwise
 * this is the minimum size of the buffer allocated for the message by the function.
 * @return Length of the message written, or 0 if fails.
 */
extern size_t FormatMessage(unsigned int flags, int _sbz0, int _sbz1, int _sbz2, void *outbuf, size_t outlen);

/**
 * @brief Set the global errno.
 * @param err New errno value.
 * @x_void_return
 */
extern void OSSetLastError(bxc_errno_t err);

/**
 * @brief Get errno value from the global errno variable.
 * @x_void_param
 * @return The current errno value.
 */
extern bxc_errno_t OSGetLastError(void);

/**
 * @brief Set errno.
 * @details
 * Unlike OSSetLastError(), this also clears the kernel errno before calling OSSetLastError().
 *
 * If the errno namespace is unset, it will also automatically set ::BXC_ERRNO_NS_APPLET before passing it to
 * OSSetLastError().
 *
 * @x_term{require-krnllib}
 *
 * @param err New errno value.
 * @x_void_return
 */
extern void _SetLastError(bxc_errno_t err);

/**
 * @brief Get errno.
 * @details
 * If kernel errno is set, the errno is returned with ::BXC_ERRNO_NS_KERNEL bit set. Otherwise it will return
 * the result from OSSetLastError() with ::BXC_ERRNO_NS_USER bit set.
 *
 * @x_term{require-krnllib}
 *
 * @x_void_param
 * @return The current errno value from either kernel or OSSetLastError().
 */
extern bxc_errno_t _GetLastError(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
