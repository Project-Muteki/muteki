#ifndef __MUTEKI_SYSCALL_H__
#define __MUTEKI_SYSCALL_H__

#include <muteki/common.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SYSCALL_HAS_STACK_ARGS (0x80000000ul)

/**
 * @brief Call arbitrary syscall with arbitrary number of arguments.
 * @details
 * This will reformat the stack frame, write an SVC instruction to an unused space of the stack, and jump to it.
 *
 * The higher 8 bits of the syscall number can encode special meanings. Currently setting the highest bit 
 * (SYSCALL_HAS_STACK_ARGS) signals the function to take account of arguments allocated on the stack. That is,
 * that bit should be set whenever there's more than 3 syscall arguments passed to this function.
 *
 * @param number The syscall number.
 * @param ... Syscall arguments.
 * @return Return value of the syscall. Can be casted to recover values of different type. Note that 64-bit values
 * are not supported and only the lower 32-bit of such values can be retrieved.
 */
long syscall(long number, ...);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_SYSCALL_H__