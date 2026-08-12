#include "muteki/syscall.h"

#if defined(__clang__)

// clang: Ignore unused parameter warning and clangd's warning on asm().
#define NO_EXPECTED_WARNINGS_BEGIN \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wunused-parameter\"") \
    _Pragma("clang diagnostic ignored \"-Wlanguage-extension-token\"")
#define NO_EXPECTED_WARNINGS_END \
    _Pragma("clang diagnostic pop")

#elif defined(__GNUC__)  // !defined(__clang__)

// gcc: Ignore unused parameter warning.
#define NO_EXPECTED_WARNINGS_BEGIN \
    _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"")
#define NO_EXPECTED_WARNINGS_END \
    _Pragma("GCC diagnostic pop")

#else  // !defined(__GNUC__)

// No-op
#define NO_EXPECTED_WARNINGS_BEGIN
#define NO_EXPECTED_WARNINGS_END

#endif  // defined(__clang__) || defined(__GNUC__)

__attribute__((used)) static unsigned int trampoline_area[] = {0xe1a00000};

NO_EXPECTED_WARNINGS_BEGIN
__attribute__((naked, target("arch=armv4t")))
long syscall(long number, ...) {
    // Starts as sysno a0 a1 a2 | a3 ... or sysno a0 a1 a2 | ...
    asm(
        // sysno x a1 a2 | a0 a3 ... or sysno x a1 a2 | a0 ...
        "push {r1}\n\t"
        // sysno a1 a2 x | a0 a3 ... or sysno a1 a2 x | a0 ...
        "mov r1, r2\n\t"
        "mov r2, r3\n\t"
        // If passing 3+ arguments
        "tst r0, #0x80000000\n\t"
        "beq 1f\n\t"
        // x a1 a2 x | sysno a0 a3 ...
        "push {r0}\n\t"
        // a0 a1 a2 x | sysno x a3 ...
        "ldr r0, [sp, #4]\n\t"
        // a0 a1 a2 a3 | sysno x x ...
        "ldr r3, [sp, #8]\n\t"
        // x a1 a2 a3 | sysno x a0 ...
        "str r0, [sp, #8]\n\t"
        // sysno a1 a2 a3 | x x a0 ...
        "ldr r0, [sp]\n\t"
        // sysno a1 a2 a3 | a0 ...
        "add sp, sp, #8\n"
        // Fall through

        "1:\n\t"
        // sysno x a2 a3? | a1 lr a0 ...
        "push {r1, lr}\n\t"

        "bic r0, r0, #0xff000000\n\t"
        "orr r1, r0, #0xef000000\n\t"

        // sysno svc a2 a3? | a1 lr a0 ...
        "ldr r0, =trampoline_area\n\t"
        // trampoline svc a2 a3? | a1 lr a0 ...
        "str r1, [r0]\n\t"
        // trampoline a1 a2 a3? | lr a0 ...
        "pop {r1}\n\t"
        "bx r0"
    );
}
NO_EXPECTED_WARNINGS_END
