# Introduction

Welcome to Project Muteki, the one and the only (as of 2026 at least) active, public Besta RTOS reverse engineering and homebrew research project.

Besta RTOS is the bespoke operating system developed mostly by Besta (Xi'an) Co., Ltd., a subsidiary of Inventec Besta, for the Besta-branded electronic dictionaries released in 2003-2014. Inventec Besta also uses modified versions of this operating system for some of their e-learning devices made for other companies as part of their hardware ODM business. Notable examples include the [HP Prime G1 calculator](https://en.wikipedia.org/wiki/HP_Prime), [Benesse Pocket Challenge DX](https://wiki3.jp/SmileTabLabo/page/17), and [Leap Frog Crammer](https://leapfrog.fandom.com/wiki/Crammer). The OS has been ported to two CPU architectures: Toshiba TLCS-900 and Arm. They share some similarities but also have significant differences in places like executable and system partition formats. This documentation will focus mostly on the Arm port of the device, as it is the most-reverse engineered one to date, although TLCS-900 port may be mentioned at times when it makes sense. Any mention of the term "Besta RTOS" in this documentation will also mean specifically the Arm port by default.

## So what *is* Besta RTOS anyway?

Architecturally, Besta RTOS has a typical small embedded operating system look, that is very common across devices powered by microcontrollers and early microprocessors. It runs inside a single address space, even when the target device has a Memory Management/Protection Unit (MMU/MPU). The executable/shared library loading mechanism also works under that address space: the code is loaded and relocated inside the address space. All of these resemble closely to other embedded systems of the same caliber, like the [Flipper Zero](https://github.com/flipperdevices/flipperzero-firmware).

Besta RTOS also borrows heavily from various other operating systems and frameworks, both conceptually and code-wise. The kernel is a modified version of uC/OS-II, the user-facing API has many references to Win32 and Borland C Runtime, the GUI subsystem is heavily inspired by Borland Turbo Vision, etc. Various open source components were also being used in the OS development, such as YAFFS2, SQLite3 and FFmpeg. The Arm port of Besta RTOS also uses the Windows CE PE format as its primary executable and shared library format. These are likely efforts to reduce development cost and attempts to make the onboarding process of new developers with previous software development experience for Windows and popular frameworks easier.

## Goals of this project

This project has 3 main goals:

- Reverse-engineer and document the external syscall interface exposed by the Besta RTOS through `sdklib.dll` and `krnllib.dll`.
- Getting Arm EABI code (with pure software floating point math) to work on Besta RTOS with minimal changes and minimal surprises.
- Provide HOWTOs on the usage of various syscalls with examples, as well as documentations that touch some Besta RTOS internals.

The reason for the second goal is that since Besta RTOS relies heavily on the MSVC Windows CE toolchain, the calling convention it uses is the Windows CE calling convention, which is based on APCS/OABI. OABI support in mainline open-source toolchains (e.g. GCC and LLVM) is in an uncertain state since it is not used by many real-world projects anymore. To make the compiled code natively and effectively use the Windows CE ABI, non-trivial modifications need to be done to both the compiler, and any supplementary libraries that work with the compiler (in the GCC world this means libgcc and parts of the newlib that provides the libm functions). In my opinion this is significantly harder to maintain than adapting EABI to work in Besta's Windows CE ABI runtime in the long term and thus not very future-proof. Going EABI also means that, out-of-the-box, certain parts of the C/C++ standard (like the C++ exceptions) may work in a less broken fashion than using the Windows CE ABI.

Due to the fact that this project targets EABI, several compromises are made in the syscall function signature definitions. Specifically, unaligned 64-bit parameters are split into 2 unsigned int values, and there is the compiler-specific @ref SYS_ALIGN macro used to override the alignment of 64-bit in-memory values when needed.
