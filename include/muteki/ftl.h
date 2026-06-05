/*
 * Copyright 2022-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file ftl.h
 * @brief Flash Translation Layer (FTL) and block device API.
 * @details @x_term require-krnllib
 */

#ifndef __MUTEKI_FTL_H__
#define __MUTEKI_FTL_H__

#include <muteki/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief NAND flash parameter.
 */
typedef struct {
    /** Magic. Needs to be set to `0x44`. */
    short magic;
    /** Length of NAND flash ID. */
    short nand_id_length;
    /** NAND flash ID. */
    unsigned char nand_id[8];
    /** NAND data size in MiB. */
    size_t size_mib;
    /** NAND block erase size in bytes. */
    size_t erase_size;
    /** NAND data page size in bytes. */
    size_t data_page_size;
    /** NAND spare (OOB) page size in bytes. */
    size_t spare_page_size;
    /** Unknown. Set to 0x32 on BA110L */
    int unk_0x1c;
    /** Human-readable name of the NAND device. */
    char name[32];
    /** Unknown. Set to 1 on BA110L. */
    int unk_0x40;
} nand_params_t;

/**
 * @brief Get the size of block device the current active drive belongs to.
 * @details When the size is >= 4GiB, the size is split into 2 parts and they must be joined together by using 
 * @code{.c}
 * size_t size_hi = 0;
 * size_t size_lo = FTL_GetCurDiskSize(&size_hi);
 * unsigned long long actual_size = (((unsigned long long) size_hi) << 32) | size_lo;
 * @endcode
 * @param size_hi The high 32-bit of the size.
 * @return The low 32-bit of the size.
 */
extern size_t FTL_GetCurDiskSize(size_t *size_hi);

/**
 * @brief Read sectors from the block device the current active drive belongs to.
 * @details Sector size seems to be constant 512 bytes.
 * @param sector Start sector number.
 * @param ptr Target pointer. The size must be multiple of 512 bytes.
 * @param count How many sectors to read.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern int FTL_ReadSector(size_t sector, void *ptr, size_t count);

/**
 * @brief Allocate a RAM disk.
 * @details This may be used internally to buffer file writes under circumstances when low latency I/O is
 * required, such as when streaming audio data from on-board NAND flash.
 * @param size Size of the RAM disk in bytes.
 * @return The drive ID of the allocated RAM disk, or -1 if the creation was unsuccessful.
 */
extern short FTL_CreateRamDisk(size_t size);

/**
 * @brief Close the previously allocated RAM disk.
 * @x_void
 */
extern void FTL_DestroyRamDisk(void);

/**
 * @brief Get NAND flash parameters.
 * @param sbz @x_term sbz
 * @param params Parameter structure to write to.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
int nand_get_params(int sbz, nand_params_t *params);

/**
 * @brief Read NAND flash pages at a specific page number.
 * @param sbz @x_term sbz
 * @param page Page number where the read should start.
 * @param ptr Buffer where the page content will be stored.
 * @param count Number of pages to read.
 * @param arg5 Unknown. May be related to reading spare sector?
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
int nand_read_page(int sbz, size_t page, void *ptr, size_t count, short arg5);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_FTL_H__
