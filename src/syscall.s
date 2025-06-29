    .arm
    .cpu arm7tdmi
    .global syscall
    .type syscall, %function

@ Starts as sysno a0 a1 a2 | a3 ... or sysno a0 a1 a2 | ...
syscall:
    @ sysno x a1 a2 | a0 a3 ... or sysno x a1 a2 | a0 ...
    push {r1}
    @ sysno a1 a2 x | a0 a3 ... or sysno a1 a2 x | a0 ...
    mov r1, r2
    mov r2, r3
    @ If passing 3+ arguments
    tst r0, #0x80000000
    beq _syscall_insert_lr
    @ x a1 a2 x | sysno a0 a3 ...
    push {r0}
    @ a0 a1 a2 x | sysno x a3 ...
    ldr r0, [sp, #4]
    @ a0 a1 a2 a3 | sysno x x ...
    ldr r3, [sp, #8]
    @ x a1 a2 a3 | sysno x a0 ...
    str r0, [sp, #8]
    @ sysno a1 a2 a3 | x x a0 ...
    ldr r0, [sp]
    @ sysno a1 a2 a3 | a0 ...
    add sp, sp, #8
    @ Fall through

_syscall_insert_lr:
    @ sysno a1 a2 a3? | lr a0 ...
    push {lr}

    bic r0, r0, #0xff000000
    orr r0, r0, #0xef000000

    @ sysno a1 a2 a3? | (svc sysno) lr a0 ...
    str r0, [sp, #-4]
    sub r0, sp, #4
    bx r0
