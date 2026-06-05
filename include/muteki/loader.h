/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file loader.h
 * @brief Binary loader API.
 */

#ifndef __MUTEKI_LOADER_H__
#define __MUTEKI_LOADER_H__

#include <muteki/common.h>
#include <muteki/file.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Defines the default subroutine identifiers to be used with ExecuteProgram().
 * @see ExecuteProgram
 */
enum applet_subroutine_e {
    /** Run the main subroutine. */
    APPLET_SUBROUTINE_MAIN = 0,
    /** Run a subroutine that clears the persistent states of the applet. */
    APPLET_SUBROUTINE_RESET_STATES = 5,
};

/**
 * @brief Applet argument passing V4 struct.
 * @details Mirrors the parameter set by the parent ExecuteProgram() call.
 * @see ExecuteProgram
 */
typedef struct applet_args_v4_s {
    /** DOS 8.3 path to executable. */
    char *dospath;
    /** Subroutine to invoke. */
    int *subroutine;
    /** User argument 1. */
    void **applet_arg1;
    /** User argument 2. */
    void **applet_arg2;
} applet_args_v4_t;

/**
 * @brief Cart file descriptor.
 * @todo Add more details.
 */
typedef struct loader_cart_descriptor_s {
    /**
     * @brief Unknown.
     */
    unsigned int unk_0x0;
    /**
     * @brief Unknown.
     */
    unsigned int unk_0x4;
    /**
     * @brief Low 32 bits of the capacity of the cart.
     */
    size_t size_low_32;
    /**
     * @brief Unknown.
     */
    unsigned int unk_0xc;
    /**
     * @brief Unknown.
     */
    void *unk_0x10;
    /**
     * @brief Unknown.
     */
    unsigned char unk_0x14;
    /**
     * @brief High 8 bits of the capacity of the cart.
     */
    unsigned char size_high_8;
    /**
     * @brief Unknown.
     */
    unsigned char unk_0x16;
    /**
     * @brief Unknown.
     */
    unsigned char unk_0x17;
    /**
     * @brief Unknown.
     */
    unsigned int unk_0x18;
    /**
     * @brief Unknown.
     */
    unsigned int unk_0x1c;
} loader_cart_descriptor_t; // 0x20

/**
 * @brief File descriptor specific to loader.
 */
typedef struct loader_file_descriptor_s {
    /**
     * @brief ROM cart descriptor.
     */
    loader_cart_descriptor_t *cart;
    /**
     * @brief Parent file descriptor.
     * @todo Retype this to the proper file descriptor type once we actually figure out how it works.
     */
    void *parent_fd; /* Created by retype action */
    /**
     * @brief Base offset of the sub-file.
     */
    size_t subfile_base;
    /**
     * @brief Base offset of this sub-file, within #parent_fd.
     */
    size_t size;
    /**
     * @brief Current `ftell` offset of this sub-file.
     */
    size_t subfile_offset;
    /**
     * @brief Unknown. Seems to be unused.
     */
    short unk_0x14; /* _OpenSubFile sets this to 1 */
    /**
     * @brief Unknown.
     */
    short unk_0x16;
    /**
     * @brief Unknown.
     */
    unsigned int unk_0x18;
    /**
     * @brief Unknown.
     */
    unsigned int unk_0x1c;
} loader_file_descriptor_t;

/**
 * @brief Resource descriptor.
 * @todo Add more details.
 */
typedef struct loader_resource_descriptor_s {
    size_t mapped_size;
    size_t mapped_offset;
    size_t data_size;
    size_t data_offset;
    loader_file_descriptor_t *ldrfd;
} loader_resource_descriptor_t;

/**
 * @brief Resource descriptor subfile request.
 * @todo Add more details.
 */
typedef struct loader_resource_subfile_request_s {
    size_t base;
    size_t size;
} loader_resource_subfile_request_t;

/**
 * @brief Executable image info.
 * @todo Add more details.
 */
typedef struct loader_image_info_s {
    size_t bestape_code_size;
    size_t bestape_data_size;
    size_t bestape_bss_size;
    size_t bestape_total_raw_size; /* Size of headers and all raw section data */
    size_t bestape_total_size;
    size_t bestape_header_size;
    unsigned int bestape_checksum;
    unsigned int unk_0x1c;
    unsigned char unk_0x20[24];
} loader_image_info_t;

/**
 * @brief Loader implementation vtable.
 * @todo Add more details.
 */
typedef struct loader_impl_s {
    void *load;
    void *load_into;
    void *unload;
    void *exec;
    void *probe;
    unsigned char *(*dlopen)(char *);
    void *unk_0x18;
    void *dlsym;
    void *unk_0x20;
    void *unk_0x24;
    loader_resource_descriptor_t * (*open_resource_descriptor)(loader_file_descriptor_t *);
    void (*close_resource_descriptor)(loader_resource_descriptor_t *);
    int (*get_resource)(loader_resource_descriptor_t *, UTF16 *, UTF16 *, loader_resource_subfile_request_t *);
    int (*collect_image_info)(loader_file_descriptor_t *, loader_image_info_t *);
} loader_impl_t;

/**
 * @brief Loaded applet executable.
 */
typedef struct loader_loaded_s {
    /**
     * @brief DOS 8.3 path to executable file.
     */
    char path[80];
    /**
     * @brief Numerical ID of loaded applet.
     */
    int id;
    /**
     * @brief Reference counter.
     */
    int refcount;
    /**
     * @brief Reference counter for currently active (running) instances.
     */
    int active_refcount;
    /**
     * @brief Mapped executable.
     */
    unsigned char *mapped_executable;
    /**
     * @brief Loader implementation being used to load this applet.
     */
    loader_impl_t *loader;
    /**
     * @brief Unknown.
     */
    short unk_0x64;
    /**
     * @brief Executable type.
     */
    short type;
    /**
     * @brief Unknown.
     * @details Seems to be copied from offset `0xc` of the corresponding ROM sepc file.
     */
    unsigned int rom_spec_offset_0xc;
    /**
     * @brief LFN path.
     */
    void *path_lfn;
    /**
     * @brief Assets bundle file descriptor.
     */
    loader_file_descriptor_t *asset_file;
    /**
     * @brief Executable file descriptors.
     */
    loader_file_descriptor_t *ldrfd;
    /**
     * @brief Unknown.
     */
    unsigned char unk_0x78[28];
    /**
     * @brief POSIX file descriptor of the executable.
     * @details Probably only set when POSIX file descriptors are supported.
     */
    void *posixfd_rom;
} loader_loaded_t;

/**
 * @brief Basic info on a loaded applet.
 */
typedef struct loader_applet_info_s {
    /**
     * @brief Magic bytes of ROM spec.
     */
    unsigned short rom_magic;
    /**
     * @brief Unknown, seems unused.
     */
    unsigned short unused_0x2;
    /**
     * @brief Type field inside the ROM spec.
     */
    unsigned short rom_type;
    /**
     * @brief Unknown, seems unused.
     */
    unsigned short unused_0x4;
    /**
     * @brief Checksum of the executable file.
     * @details On Besta RTOS Arm, this seems to be the checksum of the PE file.
     */
    unsigned int exe_checksum;
    /**
     * @brief File size of the executable.
     */
    size_t exe_raw_size;
    /**
     * @brief Unknown, seems unused.
     */
    unsigned char unused_0x10[0x74];
} loader_applet_info_t;

/**
 * @brief Open a loader file descriptor from a file.
 * @details
 *
 * `mode_or_subfile` can either be a mode or one of the following UNC paths:
 * 
 * <table>
 * <tr><th>Path</th> <th>Effect</th></tr>
 *
 * <tr><td><tt>\\\\.\\ROMFILE</tt></td> <td>Reopens the current ROM/applet file.</td></tr>
 * <tr><td><tt>\\\\.\\DATAFILE</tt></td> <td>Opens the data partition of the cartridge as a file. More details TBA.</td></tr>
 * </table>
 *
 * @x_syscall_num `0x100ec`
 *
 * @param[in] pathname DOS 8.3 path to the file.
 * @param[in] mode_or_subfile Either a mode string accepted by _afopen(), or a special UNC path to open assets of the
 * current applet/cartridge.
 * @return Loader file descriptor
 */
extern loader_file_descriptor_t *_OpenFile(const char *pathname, const char *mode_or_subfile);

/**
 * @brief Open a loader file descriptor from a file (LFN path).
 * @x_syscall_num `0x100ee`
 * @param[in] pathname LFN path to the file.
 * @param[in] mode_or_subfile Either a mode string accepted by __wfopen() (in UTF-16), or a special UNC path (**in ASCII**) to open
 * assets of the current applet/cartridge.
 * @return Loader file descriptor
 */
extern loader_file_descriptor_t *_OpenFileW(const UTF16 *pathname, const void *mode_or_subfile);

/**
 * @brief Close a loader file descriptor.
 * @x_syscall_num `0x100ef`
 * @param ldrfd The loader file descriptor.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern int _CloseFile(loader_file_descriptor_t *ldrfd);

/**
 * @brief Read data from a loader file descriptor.
 * @x_syscall_num `0x100f0`
 * @param ldrfd The loader file descriptor.
 * @param[out] buf Buffer that holds the data.
 * @param size Number of bytes to be read.
 * @return Actual number of bytes read.
 */
extern size_t _ReadFile(loader_file_descriptor_t *ldrfd, void *buf, size_t size);

/**
 * @brief Seek to a specific position in an opened sub-file.
 * @x_syscall_num `0x100f1`
 * @param ldrfd The loader file descriptor.
 * @param offset Seek offset.
 * @param whence Treat offset as relative to start of file/current offset/end of file.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern int _FseekFile(loader_file_descriptor_t *ldrfd, size_t offset, int whence);

/**
 * @brief Get the size of sub-file.
 * @x_syscall_num `0x100f2`
 * @param ldrfd The loader file descriptor.
 * @return The size of the sub-file, or `-1` when something is wrong.
 */
extern ssize_t _FileSize(loader_file_descriptor_t *ldrfd);

/**
 * @brief Open a sub-file from a parent loader file descriptor.
 * @details Sub-files are areas in the parent file. The parameter `base` and `max_size` controls where and how large the area
 * will be. The sub-file descriptors operate independently of the parent file descriptor, a la `dup()` in POSIX. This is also
 * used internally to parse ROM files and asset packs.
 * @x_syscall_num `0x100f3`
 * @param parent Parent file descriptor.
 * @param base Base offset of the file.
 * @param max_size Max size of the area included in this sub-file.
 * @return The loader sub-file descriptor.
 */
extern loader_file_descriptor_t *_OpenSubFile(loader_file_descriptor_t *parent, size_t base, size_t max_size);

/**
 * @brief Get the current offset of sub-file.
 * @x_syscall_num `0x100f4`
 * @param ldrfd The loader file descriptor.
 * @return The offset of the sub-file, or `-1` when something is wrong.
 */
extern ssize_t _TellFile(loader_file_descriptor_t *ldrfd);

/**
 * @brief Load and execute an applet by its DOS 8.3 pathname.
 * @x_syscall_num `0x10114`
 * @param pathname DOS 8.3 path to the applet executable file, or name of a registered system applet.
 * @param subroutine Subroutine to invoke.
 * @param applet_arg1 Argument 1. Can either be a pointer to some data or an integer.
 * @param applet_arg2 Argument 2. Can either be a pointer to some data or an integer.
 * @return Exit value returned from the applet subroutine.
 */
extern int RunApplicationA(const char *pathname,int subroutine,void *applet_arg1,void *applet_arg2);

/**
 * @brief Get the title name of the ROM file.
 * @details This will return UTF16-encoded localized title name that matches the current locale.
 * @x_syscall_num `0x10115`
 * @param[in] pathname DOS 8.3 path to the ROM file or executable.
 * @param[out] out_name The title name in current locale.
 * @param max_size Max size of the title name.
 * @return Length of the title name in number of UTF16 code units.
 */
extern size_t GetApplicationNameA(const char *pathname, UTF16 *out_name, size_t max_size);

/**
 * @brief Load an applet executable.
 * @details The `pathname` specified must be a DOS 8.3 name.
 * @x_syscall_num `0x10116`
 * @param pathname Path to executable.
 * @return A pointer to a structure describing the loaded executable, or NULL if the loading process failed.
 */
extern loader_loaded_t *LoadProgramA(const char *pathname);

/**
 * @brief Unload a loaded applet.
 * @x_syscall_num `0x10117`
 * @param prog The loaded executable description returned by LoadProgramA() or LoadProgramW().
 * @see LoadProgramA
 * @see LoadProgramW
 */
extern int FreeProgram(loader_loaded_t *applet);

/**
 * @brief Invoke a specific subroutine of the loaded applet with arguments.
 * @details
 * Once called, the control is fully transferred to the loaded applet and this function will block until the invoked subroutine fully exits.
 *
 * Actual format of arguments seem to be applet-specific.
 *
 * @x_syscall_num `0x10118`
 *
 * @param prog The loaded applet descriptor returned by LoadProgramA or LoadProgramW.
 * @param subroutine Subroutine to invoke.
 * @param applet_arg1 Argument 1. Can either be a pointer to some data or an integer.
 * @param applet_arg2 Argument 2. Can either be a pointer to some data or an integer.
 * @return Exit value returned from the applet subroutine.
 * @see LoadProgramA
 * @see LoadProgramW
 * 
 */
extern int ExecuteProgram(loader_loaded_t *applet, int subroutine, const void *applet_arg1, const void *applet_arg2);

/**
 * @brief Get the path to the current running executable (argv[0]).
 * @x_syscall_num `0x10119`
 * @x_void_param
 * @return The DOS 8.3 path to the current running executable.
 */
extern const char *GetCurrentPathA(void);

/**
 * @brief Return the instance of a running applet by its DOS 8.3 path name.
 * @details This is similar to GetApplicationProcA() but will return `NULL` in case loader_loaded_s::active_refcount of that applet is 0.
 * @x_syscall_num `0x1011a`
 * @param pathname DOS 8.3 path to the ROM file or executable.
 * @return Pointer to the applet instance, or `NULL` when applet is not loaded or not currently running.
 */
extern loader_loaded_t *ProgramIsRunningA(const char *pathname);

/**
 * @brief Load and execute an applet by its LFN pathname.
 * @x_syscall_num `0x1027f`
 * @param pathname LFN path to the applet executable file.
 * @param subroutine Subroutine to invoke.
 * @param applet_arg1 Argument 1. Can either be a pointer to some data or an integer.
 * @param applet_arg2 Argument 2. Can either be a pointer to some data or an integer.
 * @return Exit value returned from the applet subroutine.
 */
extern int RunApplicationW(const UTF16 *pathname, int subroutine, void *applet_arg1, void *applet_arg2);

/**
 * @brief Get the title name of the ROM file (LFN path).
 * @details This will return UTF16-encoded localized title name that matches the current locale.
 * @warning Not all systems implement this. When not implemented, the length read will always be 0.
 * @x_syscall_num `0x10280`
 * @param[in] pathname LFN path to the ROM file or executable.
 * @param[out] out_name The title name in current locale.
 * @param max_size Max size of the title name.
 * @return Length of the title name in number of UTF16 code units.
 */
extern size_t GetApplicationNameW(const UTF16 *pathname, UTF16 *out_name, size_t max_size);

/**
 * @brief Load an applet executable. (UTF-16 variant)
 * @x_syscall_num `0x10281`
 * @param pathname UTF-16 LFN path to executable.
 * @return A pointer to a structure describing the loaded executable, or `NULL` if the loading process failed.
 * @see LoadProgramA
 */
extern loader_loaded_t *LoadProgramW(const UTF16 *pathname);

/**
 * @brief Get the path to the current running executable (argv[0]). (UTF-16 variant)
 * @x_syscall_num `0x10282`
 * @x_void_param
 * @return The UTF-16 LFN path to the current running executable.
 */
extern const UTF16 *GetCurrentPathW(void);

/**
 * @brief Return the instance of a running applet by its LFN.
 * @details This is similar to GetApplicationProcW() but will return `NULL` in case loader_loaded_s::active_refcount of that applet is 0.
 * @x_syscall_num `0x10283`
 * @param pathname LFN path to the ROM file or executable.
 * @return Pointer to the applet instance, or `NULL` when applet is not loaded or not currently running.
 */
extern loader_loaded_t *ProgramIsRunningW(const UTF16 *pathname);

/**
 * @brief Load an applet executable from a loader file descriptor.
 * @x_syscall_num `0x10284`
 * @param ldrfd Loader file descriptor of the applet executable.
 * @param pathname LFN path to the applet executable.
 * @return A pointer to a structure describing the loaded executable, or `NULL` if `ldrfd` is `NULL` or the loading process failed.
 */
extern loader_loaded_t *LoadHFileProgramW(loader_file_descriptor_t *ldrfd, const UTF16 *pathname);

/**
 * @brief Load an applet executable from a loader file descriptor.
 * @x_syscall_num `0x10285`
 * @param ldrfd Loader file descriptor of the applet executable.
 * @param pathname DOS 8.3 path to the applet executable, or name of a registered system applet.
 * @return A pointer to a structure describing the loaded executable, or `NULL` if `ldrfd` is `NULL` or the loading process failed.
 */
extern loader_loaded_t *LoadHFileProgramA(loader_file_descriptor_t *ldrfd, const char *pathname);

/**
 * @brief Search and return the applet instance by DOS 8.3 pathname.
 * @details To query the current applet, one can use the following:
 *
 * @code{.c}
 * loader_loaded_t *current_applet = GetApplicationProcA(GetCurrentPathA());
 * @endcode
 *
 * or
 *
 * @code{.c}
 * loader_loaded_t *current_applet = ProgramIsRunningA(GetCurrentPathA());
 * @endcode
 *
 * @x_syscall_num `0x10289`
 * @param pathname DOS 8.3 path to the loaded applet file.
 * @return Pointer to the applet instance, or `NULL` when applet is not loaded.
 */
extern loader_loaded_t *GetApplicationProcA(const char *pathname);

/**
 * @brief Mark a loaded applet as stay resident by its DOS 8.3 pathname.
 * @details This prevents the applet from being unloaded even when the applet is not running and after FreeProgram() has been called on it.
 * @x_syscall_num `0x1028a`
 * @param pathname DOS 8.3 path to the applet executable file, or name of a registered system applet.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern int StayResidentProgramA(const char *pathname);

/**
 * @brief Unmark a loaded applet as stay resident by its DOS 8.3 pathname.
 * @details This reverts a previous StayResidentProgramA() call.
 * @x_syscall_num `0x1028b`
 * @param pathname DOS 8.3 path to the applet executable file, or name of a registered system applet.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern int UnStayResidentProgramA(const char *pathname);

/**
 * @brief Check if an loaded applet has been set to stay resident.
 * @x_syscall_num `0x1028c`
 * @param[in, out] applet Loaded applet descriptor to perform the check on.
 * @retval 0 Applet is not stay resident.
 * @retval 1 Applet is stay resident.
 */
extern int CheckProgramIsStayResident(loader_loaded_t *applet);

/**
 * @brief Search and return the applet instance by LFN.
 * @details Similar to GetApplicationProcA(), to query the current applet, one can use the following:
 *
 * @code{.c}
 * loader_loaded_t *current_applet = GetApplicationProcW(GetCurrentPathW());
 * @endcode
 *
 * or
 *
 * @code{.c}
 * loader_loaded_t *current_applet = ProgramIsRunningW(GetCurrentPathW());
 * @endcode
 *
 * However using the GetApplicationProcA() counterpart results in slightly more performant code.
 *
 * @x_syscall_num `0x10292`
 * @param pathname LFN path to the loaded applet file.
 * @return Pointer to the applet instance, or `NULL` when applet is not loaded.
 */
extern loader_loaded_t *GetApplicationProcW(const UTF16 *pathname);

/**
 * @brief Mark a loaded applet as stay resident by its LFN pathname.
 * @details This prevents the applet from being unloaded even when the applet is not running and after FreeProgram() has been called on it.
 * @x_syscall_num `0x10295`
 * @param pathname LFN path path to the applet executable file.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern int StayResidentProgramW(const UTF16 *pathname);

/**
 * @brief Unmark a loaded applet as stay resident by its LFN pathname.
 * @details This reverts a previous StayResidentProgramW() call.
 * @x_syscall_num `0x10296`
 * @param pathname LFN path to the applet executable file.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern int UnStayResidentProgramW(const UTF16 *pathname);

/**
 * @brief Obtain information on loaded applet by its DOS 8.3 pathname.
 * @x_syscall_num `0x102cb`
 * @param pathname Pathname of the applet.
 * @param[out] Information on the applet.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern int GetApplicationHeadInfoA(const char *pathname,loader_applet_info_t *info);

/**
 * @brief Obtain information on loaded applet by its LFN pathname.
 * @x_syscall_num `0x102cc`
 * @param pathname Pathname of the applet.
 * @param[out] Information on the applet.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern int GetApplicationHeadInfoW(const UTF16 *pathname, loader_applet_info_t *info);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_LOADER_H__
