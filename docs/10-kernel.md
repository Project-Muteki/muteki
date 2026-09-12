# The Besta RTOS kernel

## Introduction

The Besta RTOS kernel is based on a modified [uC/OS-II](https://github.com/weston-embedded/uC-OS2) kernel. uC/OS-II was a popular commercial, source-available RTOS originally developed by Micriµm, before its current rights holder, Weston Embedded, open-sourced it under Apache-2.0 license. Besta RTOS shares mostly the same scheduler, thread model and synchronization primitives as uC/OS-II, however the naming of the threading and synchronization primitive API is borrowed from Win32, rather than inherited from uC/OS-II.

