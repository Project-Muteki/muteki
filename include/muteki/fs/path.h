/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file path.h
 * @brief Filesystem path API.
 */

#ifndef __MUTEKI_FS_PATH_H__
#define __MUTEKI_FS_PATH_H__

#include "../common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Max code units (ASCII or UTF-16) a path can contain.
 * @details This includes the NUL terminator. Note that this can differ from libc definitions due to possible encoding
 * differences (e.g. newlib-muteki uses UTF-8 as the "ANSI" encoding by default).
 */
#define SYS_PATH_MAX_CU 256u

/**
 * @brief Read year from find timestamp.
 */
#define BXC_FIND_TS_YEAR(ts) ((ts >> 25) + 1980)
/**
 * @brief Read month from find timestamp.
 */
#define BXC_FIND_TS_MONTH(ts) ((ts >> 21) & 0xf)
/**
 * @brief Read day from find timestamp.
 */
#define BXC_FIND_TS_DAY(ts) ((ts >> 16) & 0x1f)

/**
 * @brief Read hour from find timestamp.
 */
#define BXC_FIND_TS_HOUR(ts) ((ts >> 11) & 0x1f)
/**
 * @brief Read minute from find timestamp.
 */
#define BXC_FIND_TS_MINUTE(ts) ((ts >> 5) & 0x3f)
/**
 * @brief Read second from find timestamp.
 */
#define BXC_FIND_TS_SECOND(ts) ((ts & 0xf) * 4)

/** Maximum number of code units an LFN pathname can have including the trailing NUL. */
#define BXC_FNSPLIT_LFN_PATHNAME_MAX 261u
/** Maximum number of code units an LFN drive specifier can have including the trailing NUL. */
#define BXC_FNSPLIT_LFN_DRIVE_MAX 4u
/** Maximum number of code units an LFN directory name can have including the trailing NUL. */
#define BXC_FNSPLIT_LFN_DIRNAME_MAX 257u
/** Maximum number of code units an LFN base name can have including the trailing NUL. */
#define BXC_FNSPLIT_LFN_BASENAME_MAX 257u
/** Maximum number of code units an LFN file suffix can have including the trailing NUL. */
#define BXC_FNSPLIT_LFN_SUFFIX_MAX 259u

/** Maximum number of code units an DOS 8.3 pathname can have including the trailing NUL. */
#define BXC_FNSPLIT_DOS_PATHNAME_MAX 81u
/** Maximum number of code units a DOS 8.3 drive specifier can have including the trailing NUL. */
#define BXC_FNSPLIT_DOS_DRIVE_MAX 4u
/** Maximum number of code units a DOS 8.3 directory name can have including the trailing NUL. */
#define BXC_FNSPLIT_DOS_DIRNAME_MAX 67u
/** Maximum number of code units a DOS 8.3 base name can have including the trailing NUL. */
#define BXC_FNSPLIT_DOS_BASENAME_MAX 10u
/** Maximum number of code units a DOS 8.3 file suffix can have including the trailing NUL. */
#define BXC_FNSPLIT_DOS_SUFFIX_MAX 9u

/**
 * @brief File/directory attributes.
 * @details Just the attributes in FAT32.
 * @see https://docs.microsoft.com/en-us/windows/win32/fileio/file-attribute-constants
 */
enum fs_attribute_e {
    /** Entry is read only. */
    BXC_FS_ATTR_READONLY = 0x1,
    /** Entry is hidden. */
    BXC_FS_ATTR_HIDDEN = 0x2,
    /** Entry is a system file/directory. */
    BXC_FS_ATTR_SYSTEM = 0x4,
    /** Entry is a directory. */
    BXC_FS_ATTR_DIR = 0x10,
    /** Entry is archived. */
    BXC_FS_ATTR_ARCHIVE = 0x20,
    BXC_FS_ATTR_DEVICE = 0x40,
    /** Entry does not have any other attribute. */
    BXC_FS_ATTR_NONE = 0x80,
};

/**
 * @brief Structure for find context.
 */
typedef struct {
    /**
     * @brief Current file descriptor.
     */
    void *unk0; // 0x0
    /**
     * @brief Next file descriptor.
     */
    void *unk4; // 0x4
    /** UTF-16-encoded long filename of the entry. */
    UTF16 *filename_lfn; // 0x8
    /** DOS 8.3 filename of the entry. */
    char *filename; // 0xc
    /**
     * @brief Seems to be a mirror of #filename
     * @see filename
     */
    char *filename2_alt; // 0x10
    /** Size of file. */
    size_t size; // 0x14
    /**
     * @brief Modify timestamp.
     * @see BXC_FIND_TS_YEAR
     * @see BXC_FIND_TS_MONTH
     * @see BXC_FIND_TS_DAY
     * @see BXC_FIND_TS_HOUR
     * @see BXC_FIND_TS_MINUTE
     * @see BXC_FIND_TS_SECOND
     */
    unsigned int mtime; // 0x18
    /**
     * @brief Create/birth timestamp.
     * @details The format of this field seems to be similar to mtime, but seems to be corrupted somehow.
     * @see mtime
     */
    unsigned int btime; // 0x1c
    /**
     * @brief Access timestamp.
     * @see mtime
     */
    unsigned int atime; // 0x20
    /** FAT filesystem file attribute mask. */
    unsigned char attrib_mask; // 0x24
    /** FAT filesystem file attributes. */
    unsigned char attrib; // 0x25
} bxc_find_context_t;

/**
 * @brief Filesystem usage statistics.
 */
typedef struct {
    /** Filesystem size in KiB. */
    size_t size_kb;
    /** Seems to be the same as bxc_fs_stat_t::size_kb. */
    size_t size2_kb;
    /** Seems to be the same as bxc_fs_stat_t::size_kb. */
    size_t size3_kb;
    /** Used space in KiB. */
    size_t used_kb;
    /** Available space in KiB. */
    size_t free_kb;
    /** Filesystem size in bytes. */
    unsigned long long SYS_DWORD size;
    /** Seems to be the same as bxc_fs_stat_t::size. */
    unsigned long long SYS_DWORD size2;
    /** Seems to be the same as bxc_fs_stat_t::size. */
    unsigned long long SYS_DWORD size3;
    /** Used space in bytes. */
    unsigned long long SYS_DWORD used;
    /** Available space in bytes. */
    unsigned long long SYS_DWORD free;
} bxc_fs_stat_t;

/**
 * @brief All-in-one struct that contain all parts used in _wfnsplit() and _wfnmerge()
 */
typedef struct bxc_fs_parts_lfn_s {
    /**
     * @brief Pathname.
     */
    UTF16 pathname[BXC_FNSPLIT_LFN_PATHNAME_MAX];
    /**
     * @brief Drive.
     */
    UTF16 drive[BXC_FNSPLIT_LFN_DRIVE_MAX];
    /**
     * @brief Directory name.
     */
    UTF16 dirname[BXC_FNSPLIT_LFN_DIRNAME_MAX];
    /**
     * @brief Base name without suffix.
     */
    UTF16 basename[BXC_FNSPLIT_LFN_BASENAME_MAX];
    /**
     * @brief Suffix.
     */
    UTF16 suffix[BXC_FNSPLIT_LFN_SUFFIX_MAX];
} bxc_fs_parts_lfn_t;

/**
 * @brief All-in-one struct that contain all parts used in _afnsplit() and _afnmerge()
 */
typedef struct bxc_fs_parts_dos_s {
    /**
     * @brief Pathname.
     */
    char pathname[BXC_FNSPLIT_DOS_PATHNAME_MAX];
    /**
     * @brief Drive.
     */
    char drive[BXC_FNSPLIT_DOS_DRIVE_MAX];
    /**
     * @brief Directory name.
     */
    char dirname[BXC_FNSPLIT_DOS_DIRNAME_MAX];
    /**
     * @brief Base name without suffix.
     */
    char basename[BXC_FNSPLIT_DOS_BASENAME_MAX];
    /**
     * @brief Suffix.
     */
    char suffix[BXC_FNSPLIT_DOS_SUFFIX_MAX];
} bxc_fs_parts_dos_t;

/**
 * @brief Delete a file.
 * @x_syscall_num `0x100dd`
 * @param pathname DOS 8.3 path to the file being removed.
 * @retval true @x_term ok
 * @retval false @x_term ng
 */
extern bool _aremove(const char *pathname);

/**
 * @brief Delete a file.
 * @x_syscall_num `0x10274`
 * @param pathname UTF-16 LFN path to the file being removed.
 * @retval true @x_term ok
 * @retval false @x_term ng
 */
extern bool __wremove(const UTF16 *pathname);

/**
 * @brief Begin a file/directory search.
 * @details Matches the first file/directory based on fnmatch pattern `fnmatch` and attribute mask `attrib_mask`. Saves
 * context at `ctx`, which can be used later with _afindnext() and _findclose().
 * 
 * @x_syscall_num `0x100d8`
 * @param fnmatch The pattern.
 * @param ctx The context object.
 * @param attrib_mask Attribute mask (use 0 to match everything regardless of attributes).
 * @retval 0 There is a match.
 * @retval -1 No more match was found.
 */
extern short _afindfirst(const char *fnmatch, bxc_find_context_t *ctx, int attrib_mask);

/**
 * @brief Continue a previously started file/directory search.
 * @details Matches the next file/directory.
 * @x_syscall_num `0x100d9`
 * @param ctx The context object.
 * @retval 0 There is a match.
 * @retval -1 No more match was found.
 */
extern short _afindnext(bxc_find_context_t *ctx);

/**
 * @brief Begin a file/directory search (LFN support).
 * @details Matches the first file/directory based on fnmatch pattern `fnmatch` and attribute mask `attrib_mask`. Saves
 * context at `ctx`, which can be used later with _afindnext() and _findclose().
 * 
 * @x_syscall_num `0x10270`
 * @param fnmatch The pattern (encoded in UTF-16).
 * @param ctx The context object.
 * @param attrib_mask Attribute mask (use 0 to match everything regardless of attributes).
 * @retval 0 There is a match.
 * @retval -1 No more match was found.
 */
extern short _wfindfirst(const UTF16 *fnmatch, bxc_find_context_t *ctx, int attrib_mask);

/**
 * @brief Continue a previously started file/directory search.
 * @details
 * Matches the next file/directory.
 *
 * Not sure if interchangeable with _afindnext() since it should have no dependency on different find context. Needs to
 * be verified later.
 *
 * @x_syscall_num `0x10271`
 * @param ctx The context object.
 * @retval 0 There is a match.
 * @retval -1 No more match was found.
 */
extern short _wfindnext(bxc_find_context_t *ctx);

/**
 * @brief Terminate a file/directory search.
 * @x_syscall_num `0x100da`
 * @param ctx The context object.
 * @retval 0 @x_term ok
 */
extern int _findclose(bxc_find_context_t *ctx);

/**
 * @brief Get attributes of specific path.
 * @details Similar to GetFileAttributeW() in Windows.
 * @x_syscall_num `0x10272`
 * @param path UTF-16 LFN path.
 * @return The attribute, or -1 on error.
 * @see fs_attribute_e
 */
extern short _wfgetattr(UTF16 *path);

/**
 * @brief Set attributes of specific path.
 * @details Similar to SetFileAttributeW() in Windows.
 * @x_syscall_num `0x10273`
 * @param path UTF-16 LFN path.
 * @param attrs The new attribute.
 * @return The new attribute, or -1 on error.
 * @see fs_attribute_e
 */
extern short _wfsetattr(UTF16 *path, short attrs);

/**
 * @brief Get attributes of specific path.
 * @details Similar to GetFileAttributeA() in Windows.
 * @x_syscall_num `0x100db`
 * @param path DOS 8.3 path.
 * @return The attribute, or -1 on error.
 * @see fs_attribute_e
 */
extern short _afgetattr(char *path);

/**
 * @brief Set attributes of specific path.
 * @details Similar to SetFileAttributeA() in Windows.
 * @x_syscall_num `0x100dc`
 * @param path DOS 8.3 path.
 * @param attrs The new attribute.
 * @return The new attribute, or -1 on error.
 * @see fs_attribute_e
 */
extern short _afsetattr(char *path, short attrs);

/**
 * @brief Create a new directory.
 * @x_syscall_num `0x10277`
 * @param path UTF-16 LFN path to the new directory.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern int _wmkdir(UTF16 *path);

/**
 * @brief Create a new directory.
 * @x_syscall_num `0x100e0`
 * @param path DOS 8.3 path to the new directory.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern int _amkdir(char *path);

/**
 * @brief Remove a directory.
 * @x_syscall_num `0x10278`
 * @details Sets kerrno namespace to ::BXC_ERRNO_NS_KERNEL and error code to `0xffff` when directory is not empty.
 * @param path UTF-16 LFN path to the directory being removed.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern int _wrmdir(UTF16 *path);

/**
 * @brief Remove a directory.
 * @x_syscall_num `0x100e1`
 * @details Sets kerrno namespace to ::BXC_ERRNO_NS_KERNEL and error code to `0xffff` when directory is not empty.
 * @param path DOS 8.3 path to the directory being removed.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern int _armdir(char *path);

/**
 * @brief Get system CWD value.
 * @x_syscall_num `0x100e3`
 * @param unk Some context object, format unknown. Use NULL to get the CWD of current applet.
 * @param buf Buffer large enough to contain a DOS8.3 path. Must be at least ::SYS_PATH_MAX_CU bytes long.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern short _agetcurdir(void *unk, char *buf);

/**
 * @brief Get system CWD value.
 * @x_syscall_num `0x1027a`
 * @param unk Some context object, format unknown. Use NULL to get the CWD of current applet.
 * @param buf Buffer large enough to contain a UTF-16 LFN path. Must be at least ::SYS_PATH_MAX_CU units long.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern short _wgetcurdir(void *unk, UTF16 *buf);

/**
 * @brief Change system working directory to a new path.
 * @x_syscall_num `0x100e2`
 * @param path DOS 8.3 path.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern short _achdir(const char *path);

/**
 * @brief Change system working directory to a new path.
 * @x_syscall_num `0x10279`
 * @param path UTF-16 LFN path.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern short _wchdir(const UTF16 *path);

/**
 * @brief Rename/move a file.
 * @x_syscall_num `0x100de`
 * @param old_path Old DOS 8.3 path.
 * @param new_path New DOS 8.3 path.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern short _arename(const char *old_path, const char *new_path);

/**
 * @brief Rename/move a file.
 * @x_syscall_num `0x10275`
 * @param old_path Old UTF-16 LFN path.
 * @param new_path New UTF-16 LFN path.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern short _wrename(const UTF16 *old_path, const UTF16 *new_path);

/**
 * @brief Get filesystem usage stats.
 * @x_syscall_num `0x100eb`
 * @param fsid Filesystem ID.
 * @param fs_stat Target to store filesystem usage stats.
 * @retval 0 @x_term ok
 */
extern int FSGetDiskRoomState(int fsid, bxc_fs_stat_t *fs_stat);

/**
 * @brief Split a DOS 8.3 pathname into parts.
 * @details DOS 8.3 counterpart of _wfnsplit().
 * @x_syscall_num `0x100c5`
 * @param[in] pathname Pathname to be split.
 * @param[out] drive Drive name, or NULL to omit this part.
 * Must be at least as long as ::BXC_FNSPLIT_DOS_DRIVE_MAX.
 * @param[out] dirname Directory, or NULL to omit this part.
 * Must be at least as long as ::BXC_FNSPLIT_DOS_DIRNAME_MAX.
 * @param[out] basename Basename without suffix, or NULL to omit this part.
 * Must be at least as long as ::BXC_FNSPLIT_DOS_BASENAME_MAX.
 * @param[out] suffix Suffix, or NULL to omit this part.
 * Must be at least as long as ::BXC_FNSPLIT_DOS_SUFFIX_MAX.
 * @return Bitfield that indicates which parts were being extracted successfully.
 * @see _wfnsplit Its LFN counterpart.
 */
extern int _afnsplit(const char *pathname, char *drive, char *dirname, char *basename, char *suffix);

/**
 * @brief Build an DOS 8.3 pathname from parts.
 * @details DOS 8.3 counterpart of _wfnmerge().
 * @x_syscall_num `0x100c6`
 * @param[out] pathname Pointer to a buffer where the constructed pathname will be stored.
 * This buffer must be at least as long as ::BXC_FNSPLIT_DOS_PATHNAME_MAX.
 * @param[in] drive Drive specifier (e.g., `C` or `C:`). If provided, it must be a valid drive name.
 * @param[in] dirname Directory path (e.g., `\path\to\a\`). If provided, it must end with a backslash.
 * @param[in] basename Base name without a suffix (e.g., `file`).
 * @param[in] suffix File suffix (e.g., `.txt`).
 * @return Bitfield that indicates which parts were added into the resulting pathname.
 * @see _wfnmerge Its LFN counterpart.
 */
extern int _afnmerge(char *pathname, const char *drive, const char *dirname, const char *basename, const char *suffix);

/**
 * @brief Split an LFN pathname into parts.
 * @details
 * Borland libc style path parsing helper. It takes a pathname in the format of `C:\path\to\a\file.txt` and parse
 * it into 4 parts:
 * - drive: The drive name consists of the drive letter and a single colon (`C:`)
 * - dirname: The directory name with the root backslash and training backslash (`\path\to\a\`)
 * - basename: Basename without the suffix (`file`)
 * - suffix: Suffix with the leading dot (`.txt`)
 * 
 * Borland C++ has a documentation regarding to their implementation:
 * https://docwiki.embarcadero.com/RADStudio/Alexandria/en/Fnsplit,_wfnsplit.
 *
 * @x_syscall_num `0x1026b`
 * @param[in] pathname Pathname to be split.
 * @param[out] drive Drive name, or NULL to omit this part.
 * Must be at least as long as ::BXC_FNSPLIT_LFN_DRIVE_MAX.
 * @param[out] dirname Directory, or NULL to omit this part.
 * Must be at least as long as ::BXC_FNSPLIT_LFN_DIRNAME_MAX.
 * @param[out] basename Basename without suffix, or NULL to omit this part.
 * Must be at least as long as ::BXC_FNSPLIT_LFN_BASENAME_MAX.
 * @param[out] suffix Suffix, or NULL to omit this part.
 * Must be at least as long as ::BXC_FNSPLIT_LFN_SUFFIX_MAX.
 * @return Bitfield that indicates which parts were being extracted successfully.
 */
extern int _wfnsplit(const UTF16 *pathname, UTF16 *drive, UTF16 *dirname, UTF16 *basename, UTF16 *suffix);

/**
 * @brief Build an LFN pathname from parts.
 * @details
 * Borland libc style path parsing helper. It constructs a pathname by combining the components: drive, directory,
 * basename, and suffix. It is designed to be the inverse of the _wfnsplit() syscall, allowing users to reconstruct a
 * full pathname from its individual parts.
 *
 * Each component is optional and can be provided as `NULL` if it is not needed in the final pathname. The syscall
 * will concatenate the non-NULL components in the order of drive, directory, basename, and suffix, ensuring proper
 * formatting such as adding backslashes where necessary.
 *
 * Borland C++ has a documentation regarding to their implementation:
 * https://docwiki.embarcadero.com/RADStudio/Alexandria/en/Fnmerge,_wfnmerge.
 *
 * @x_syscall_num `0x1026c`
 * @param[out] pathname Pointer to a buffer where the constructed pathname will be stored.
 * This buffer must be at least as long as ::BXC_FNSPLIT_LFN_PATHNAME_MAX.
 * @param[in] drive Drive specifier (e.g., `C` or `C:`). If provided, it must be a valid drive name.
 * @param[in] dirname Directory path (e.g., `\path\to\a\`). If provided, it must end with a backslash.
 * @param[in] basename Base name without a suffix (e.g., `file`).
 * @param[in] suffix File suffix (e.g., `.txt`).
 * @return Bitfield that indicates which parts were added into the resulting pathname.
 */
extern int _wfnmerge(UTF16 *pathname, const UTF16 *drive, const UTF16 *dirname, const UTF16 *basename, const UTF16 *suffix);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_FS_PATH_H__
