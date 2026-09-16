# The Besta RTOS kernel

## Introduction

The Besta RTOS kernel is based on a modified [uC/OS-II](https://github.com/weston-embedded/uC-OS2) kernel. uC/OS-II was a popular commercial, source-available RTOS originally developed by Micriµm, before its current rights holder, Weston Embedded, open-sourced it under Apache-2.0 license. Besta RTOS shares mostly the same scheduler, thread model and synchronization primitives as uC/OS-II, however the naming of the threading and synchronization primitive API is partially borrowed from Win32, rather than inherited from uC/OS-II alone.

## Scheduler and thread model

The Besta RTOS scheduler algorithm is mostly inherited from uC/OS-II. Both scheduler use an [8x8 ready table](https://micrium.atlassian.net/wiki/spaces/osiidoc/pages/163854/Kernel+Structure#Ready-List) to mark active threads and to do @f$ \Theta(1) @f$ lookup of the next highest priority thread. As such, most of the members in the @ref bxc_thread_t structure have a counterpart in uC/OS-II's [Task Control Block](https://micrium.atlassian.net/wiki/spaces/osiidoc/pages/163854/Kernel+Structure#Task-Control-Blocks-(OS_TCBs)) (TCB). For example, @ref bxc_thread_t.slot has the exact same purpose as `OSTCBPrio`. Besta also added a few extensions to the @ref bxc_thread_t that are not a part of the vanilla uC/OS-II kernel (more on them later).

### Scheduler timing

Fundamentally the scheduler keeps track of time using **scheduler ticks** (with period denoted as @f$T@f$). This is typically driven by an interrupt channel backed by a hardware timer on the device, and the period is possibly defined per-device. On top of the tick interrupt period, Besta RTOS also establishes the concept of **time units** (denoted as @f$U@f$). This is what the applets would normally see when it comes to scheduler timing (through for example OSSleep() or the timeout parameter passed to waitables), and 1 @f$U@f$ roughly equals to 1 millisecond in real time. In practice, one @f$U@f$ can represent time less than one @f$T@f$. The scheduler thus keeps track of an integer @f$U_{unit}@f$ of how many time units equal to one tick interrupt period (i.e. @f$U_{unit} = \lfloor \frac{T}{U} \rfloor@f$). If @ref bxc_thread_t.sleep_counter is less than or equal to a single scheduler tick (@f$U_{unit}@f$), the scheduler makes it available to be selected for the next execution.

### The idle task

During scheduler initialization, a idle thread is automatically created using @ref OSCreateThread with an assigned priority value of 63. The scheduler also makes special handling.

### Thread timeout mechanism

When picking the next thread to execute, the original uC/OS-II scheduler always chooses the highest priority active thread. On its intended platforms (i.e. small, self-contained, possibly microcontroller-based systems) this makes sense, as thread interactions are typically clearly defined on these systems to prevent possible starvation of the lower priority threads. On Besta RTOS however, the starvation could be a problem as Besta RTOS's use-case leans more towards a multi-program operating system focused on user interaction than uC/OS-II's default tightly-integrated controller use-case, meaning a single bug in an external program could halt critical system tasks and/or causing sluggish UI response. Likely as a step to mitigate this problem, Besta RTOS introduced a timeout mechanism to its scheduler. When a thread is created, a timeout value (@ref bxc_thread_t.timeout) that is inverse-proportional to its priority value is assigned to the thread. Let @f$ P_{thread} @f$ be the priority value of the thread, the formula that is used to determine the initial timeout value is @f$ \lfloor \frac{64 - P_{thread}}{16} \rfloor + 1 @f$. Behavior-wise, the timeout is 5 for priority 0, 4 for priority 1-16, 3 for 17-32, etc. The idle thread is excluded from this calculation, as it does not need a timeout by design.

The scheduler decrements the timeout value of the current active thread (that is **not** already timed out/yielded) on every scheduler tick. When the value reaches 0, the thread will be put into the "timed out" state (adding @ref BXC_WAIT_ON_YIELD to @ref bxc_thread_t.wait_reason), and the next active highest priority thread will then be picked to run. This process will repeat itself until no thread is active other than the idle thread. When that happens, the scheduler enumerates the thread linked list and takes all threads that have the wait reason of @ref BXC_WAIT_ON_YIELD out of the timed out/yielded state, and the execution resumes at the highest priority thread again.

The timeout value can also be reset in various circumstances. 
