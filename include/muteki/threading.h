/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file threading.h
 * @brief Native threading API.
 */

#ifndef __MUTEKI_THREADING_H__
#define __MUTEKI_THREADING_H__

#include "common.h"
#include "errno.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Thread wait reason enum.
 */
enum thread_wait_reason_e {
    /** Nothing */
    WAIT_ON_NONE = 0x0,
    /** Waiting on a semaphore. */
    WAIT_ON_SEMAPHORE = 0x1,
    /** Waiting on an event. */
    WAIT_ON_EVENT = 0x2,
    /** Waiting for a message queue push. */
    WAIT_ON_QUEUE = 0x4,
    /** Waiting to be unsuspended by OSResumeThread(). */
    WAIT_ON_SUSPEND = 0x8,
    /** Waiting for a critical section to be released. */
    WAIT_ON_CRITICAL_SECTION = 0x10,
    /** Waiting for sleep counter to expire. */
    WAIT_ON_SLEEP = 0x20,
};

/**
 * @brief Result of waitables.
 */
typedef enum wait_result_e {
    /** Timeout before the event is set. */
    WAIT_RESULT_TIMEOUT = 0x82,
    /** The event is set. */
    WAIT_RESULT_RESOLVED,
    /** An error occurred. */
    WAIT_RESULT_ERROR,
} wait_result_t;

/**
 * @brief Thread function type
 */
typedef int (*thread_func_t)(void *user_data);

/**
 * @brief Common data structure for waitables.
 *
 * Once requested, waitables are able to let a currently running thread to pause execution and wait for a specific event to happen. This is pretty much the same as event group and event table in uC/OS-II.
 */
typedef struct {
    /** Bitfield that indicates which waiting_by bytes are currently active. */
    unsigned char active_bytes;
    /** Bitfield that tracks threads that are waiting for this waitable. Indexed by `byte offset * 8 + bit offset` */
    unsigned char waiting_by[8];
} threading_waitable_t;

/**
 * @brief Message type for message queues.
 * @details This needs to be 4 byte aligned since the inline memcpy in the internal FIFO queue routines use hardcoded
 * ldm/stm.
 */
typedef char message_queue_message_t[16] SYS_ALIGN(4);

/**
 * @brief Nonatomic backend storage for message queues.
 * @details Simple ring-buffer-based FIFO queue data structure used internally by message queues.
 */
typedef struct {
    /** Message body. */
    message_queue_message_t *messages;
    /** Number of chunks. */
    unsigned short size;
    /** Pop index. */
    short pop_idx;
    /** Push index. */
    short push_idx;
    short _padding_0xa;
} message_queue_nonatomic_t;

/**
 * @brief Thread descriptor type.
 */
typedef struct thread_s thread_t;
/**
 * @brief Semaphore descriptor type.
 */
typedef struct semaphore_s semaphore_t;
/**
 * @brief Event descriptor type.
 */
typedef struct event_s event_t;
/**
 * @brief Critical section descriptor type.
 */
typedef struct critical_section_s critical_section_t;
/**
 * @brief Message queue descriptor type.
 */
typedef struct message_queue_s message_queue_t;

/**
 * @brief Thread descriptor structure.
 */
struct thread_s {
    /** Magic. Always `0x100`. */
    int magic; // always 0x100
    /** Stack pointer. When the thread is suspended this will point to the CPU context saved on thread stack. */
    uintptr_t *sp;
    /** Allocated stack memory. */
    void *stack;
    /** Exit code of the thread. Initializes to 0. */
    int exit_code; // init to 0
    /** Error code. */
    kerrno_t kerrno; // init to 0
    /** Unknown. Initializes to 0x80000000. */
    uintptr_t unk_0x14; // init to 0x80000000
    /** Thread function entrypoint. */
    thread_func_t thread_func;
    /** Unknown. */
    short unk_0x1c;
    /** Milliseconds left to sleep. */
    short sleep_counter;
    /**
     * Current wait reason of the thread.
     * @see thread_wait_reason_e
     */
    short wait_reason; // 0x20
    /** Slot number. For scheduler. */
    short slot; // 0x22
    /** Lower 3 bit of the slot number. For scheduler. */
    char slot_low3b;
    /** Upper 3 bit of the slot number. For scheduler. */
    char slot_high3b;
    /** Lower 3 bit bitmask of the slot number. For scheduler. */
    unsigned char slot_low3b_bit;
    /** Upper 3 bit bitmask of the slot number. For scheduler. */
    unsigned char slot_high3b_bit;
    /** Event descriptor that belongs to the event the thread is currently waiting for. */
    event_t *event;
    /** Previous thread descriptor. */
    thread_t *prev;
    /** Next thread descriptor. */
    thread_t *next;
    union {
        /** Unknown and seems to be uninitialized. */
        char unk_0x34[0x20];
        /** Kernel TLS (reusing the seemingly unused unk_0x34 fields) */
        uintptr_t ktls[8];
    };
};

/**
 * @brief Semaphore descriptor structure.
 */
struct semaphore_s {
    /** Magic. Always `0x200`. */
    int magic;
    int _padding_0x4;
    /** Counter. */
    short ctr;
    /**
     * @brief Wait state of the current semaphore.
     * @see threading_waitable_t
     */
    threading_waitable_t wait_state;
    char _padding_0x13;
};

/**
 * @brief Event descriptor structure.
 */
struct event_s {
    /** Magic. Always `0x201`. */
    int magic;
    /** Flag value. 1 is set and 0 is clear. */
    int flag;
    /** Set to non-0 will inhibit the event from getting cleared after a OSWaitForEvent() is resolved. */
    short latch_on;
    /**
     * @brief Wait state of the current event.
     * @see threading_waitable_t
     */
    threading_waitable_t wait_state;
    char _padding_0x13;
};

/**
 * @brief Critical section descriptor structure.
 */
struct critical_section_s {
    /** Magic. Always `0x202`. Note that for some reason this is the same as ::message_queue_t. */
    int magic; // 0x00000202
    /** Thread descriptor for this thread. */
    thread_t *thr;
    /** Reference counter. */
    unsigned short refcount;
    /**
     * @brief Wait state of the current critical section.
     * @see threading_waitable_t
     */
    threading_waitable_t wait_state;
    char _padding_0x13;
}; // 0x14

/**
 * @brief Message queue descriptor structure.
 */
struct message_queue_s {
    /** Magic. Always `0x202`. Note that for some reason this is the same as ::critical_section_t. */
    int magic;
    /** Storage structure i.e. the actual queue part of the queue. */
    message_queue_nonatomic_t *storage;
    short _padding_0x8;
    /**
     * @brief Wait state of the current event.
     * @see threading_waitable_t
     */
    threading_waitable_t wait_state;
    char _padding_0x13;
};

/**
 * @brief Create a new thread.
 * @x_syscall_num `0x10000`
 * @param func Function to execute in the new thread.
 * @param user_data User data for the thread.
 * @param stack_size The size of the thread stack.
 * @param defer_start Do not immediately schedule this thread and create it as suspended.
 * @return The thread descriptor.
 */
extern thread_t *OSCreateThread(thread_func_t func, void *user_data, size_t stack_size, bool defer_start);

/**
 * @brief Terminate a thread.
 * @x_syscall_num `0x10001`
 * @param thr Thread to terminate.
 * @param exit_code The exit code.
 * @retval 0 @x_term ok
 */
extern int OSTerminateThread(thread_t *thr, int exit_code);

/**
 * @brief Set the thread priority (slot number).
 * @details
 * On Besta RTOS, priority is implied in the natural order of the threads in the global thread table. Some
 * slots in the table seem to be reserved (8 for the top and 18 for the bottom) and are not accessible by just
 * allocating the thread with OSCreateThread(). User can move threads to these reserved slots by calling the
 * OSSetThreadPriority() function.
 *
 * @x_syscall_num `0x10002`
 * @param thr The thread descriptor.
 * @param new_slot The new slot number.
 * @retval true @x_term ok
 * @retval false @x_term ng
 * @see OSGetThreadPriority
 */
 extern bool OSSetThreadPriority(thread_t *thr, short new_slot);

/**
 * @brief Get the thread priority (slot number).
 * @x_syscall_num `0x10003`
 * @param thr The thread descriptor.
 * @return The slot number of the thread.
 */
extern short OSGetThreadPriority(thread_t *thr);

/**
 * @brief Suspend a thread from outside of that thread.
 * @x_syscall_num `0x10004`
 * @param thr The thread descriptor.
 * @retval true @x_term ok
 * @retval false @x_term ng
 */
extern bool OSSuspendThread(thread_t *thr);

/**
 * @brief Start/restart a previously suspended thread.
 * @x_syscall_num `0x10005`
 * @param thr The thread descriptor.
 * @retval true @x_term ok
 * @retval false @x_term ng
 */
extern bool OSResumeThread(thread_t *thr);

/**
 * @brief Force wake up a sleeping thread
 * @x_syscall_num `0x10006`
 * @details This expire the sleep counter of a thread immediately and reschedule if the thread is not suspended.
 * @param thr The thread descriptor.
 * @retval true @x_term ok
 * @retval false @x_term ng
 */
extern bool OSWakeUpThread(thread_t *thr);

/**
 * @brief Terminate current thread.
 * @x_syscall_num `0x10007`
 * @details This calls OSTerminateThread() with the descriptor of current thread as `thr`.
 * @param exit_code The exit code.
 * @retval 0 @x_term ok
 */
 extern int OSExitThread(int exit_code);

/**
 * @brief Sleep for `millis` milliseconds.
 * @x_syscall_num `0x10008`
 * @param millis Time to sleep in milliseconds.
 * @x_void_return
 */
extern void OSSleep(short millis);

/**
 * @brief Create an semaphore descriptor.
 * @x_syscall_num `0x10009`
 * @param init_ctr Initial counter value.
 * @return The semaphore descriptor.
 */
extern semaphore_t *OSCreateSemaphore(short init_ctr);

/**
 * @brief Wait and acquire a semaphore.
 * @x_syscall_num `0x1000a`
 * @param semaphore The semaphore context.
 * @param timeout Timeout in OSSleep() units.
 * @return The result.
 * @see wait_result_t
 */
extern wait_result_t OSWaitForSemaphore(semaphore_t *semaphore, short timeout);

/**
 * @brief Release a semaphore.
 * @x_syscall_num `0x1000b`
 * @param semaphore The semaphore context.
 * @retval true @x_term ok
 * @retval false @x_term ng
 */
extern bool OSReleaseSemaphore(semaphore_t *semaphore);

/**
 * @brief Destroy a semaphore.
 * @x_syscall_num `0x1000c`
 * @param semaphore The semaphore context.
 * @retval true @x_term ok
 * @retval false @x_term ng
 */
extern bool OSCloseSemaphore(semaphore_t *semaphore);

/**
 * @brief Create an event descriptor.
 * @x_syscall_num `0x1000d`
 * @param latch_on Set to non-0 will inhibit the event from getting cleared after a OSWaitForEvent() is resolved.
 * @param flag The initial flag value. Can be either 0 or 1.
 * @return The event descriptor.
 */
extern event_t *OSCreateEvent(short latch_on, int flag);

/**
 * @brief Wait for an event.
 * @x_syscall_num `0x1000e`
 * @param event The event context.
 * @param timeout Timeout in OSSleep() units.
 * @return The result.
 * @see wait_result_t
 */
extern wait_result_t OSWaitForEvent(event_t *event, short timeout);

/**
 * @brief Set the event flag.
 * @details This sets the event_t::flag to 1.
 * @x_syscall_num `0x1000f`
 * @param event The event context.
 * @retval true @x_term ok
 * @retval false @x_term ng
 */
extern bool OSSetEvent(event_t *event);

/**
 * @brief Reset the event flag.
 * @details This sets the event_t::flag to 0.
 * @x_syscall_num `0x10010`
 * @param event The event context.
 * @retval true @x_term ok
 * @retval false @x_term ng
 */
extern bool OSResetEvent(event_t *event);

/**
 * @brief Destroy the event descriptor.
 * @x_syscall_num `0x10011`
 * @param event The event context.
 * @retval true @x_term ok
 * @retval false @x_term ng
 */
extern bool OSCloseEvent(event_t *event);

/**
 * @brief Initialize a critical section descriptor.
 * @x_syscall_num `0x10012`
 * @param[out] cs The critical section descriptor.
 * @x_void_return
 */
extern void OSInitCriticalSection(critical_section_t *cs);

/**
 * @brief Enter/aquire a critical section.
 * @details
 * Besta critical sections behave like recursive mutexes. Therefore this will block when multiple threads are
 * trying to enter the same context, but it will let repeated entry attempts initiated by the same thread to pass
 * through. The context is released when all of the entries are reverted by a OSLeaveCriticalSection() call.
 *
 * @x_syscall_num `0x10013`
 * @param[in, out] cs The critical section descriptor.
 * @x_void_return
 */
extern void OSEnterCriticalSection(critical_section_t *cs);

/**
 * @brief Leave/release a critical section.
 * @x_syscall_num `0x10014`
 * @param[in, out] cs The critical section descriptor.
 * @x_void_return
 */
extern void OSLeaveCriticalSection(critical_section_t *cs);

/**
 * @brief Destroy a critical section descriptor.
 * @x_syscall_num `0x10015`
 * @param[in, out] cs The critical section descriptor.
 * @x_void_return
 */
extern void OSDeleteCriticalSection(critical_section_t *cs);

/**
 * @brief Create a message queue descriptor.
 * @x_syscall_num `0x10018`
 * @param size Size of the queue in number of messages (will use `sizeof(`::message_queue_message_t`) * size` bytes of
 * memory).
 * @return The message queue descriptor.
 */
extern message_queue_t *OSCreateMsgQue(unsigned short size);

/**
 * @brief Push a message into the queue.
 * @x_syscall_num `0x10019`
 * @param queue The message queue descriptor.
 * @param message The message being pushed.
 * @retval true @x_term ok
 * @retval false @x_term ng
 */
extern bool OSPostMsgQue(message_queue_t *queue, const message_queue_message_t *message);

/**
 * @brief Push a message into the queue and reschedule immediately.
 * @x_syscall_num `0x1001a`
 * @param queue The message queue descriptor.
 * @param message The message being pushed.
 * @retval true @x_term ok
 * @retval false @x_term ng
 */
extern bool OSSendMsgQue(message_queue_t *queue, const message_queue_message_t *message);

/**
 * @brief Peek the bottom of the queue without popping the message.
 * @x_syscall_num `0x1001b`
 * @param queue The message queue descriptor.
 * @param message The result message.
 * @retval true @x_term ok
 * @retval false @x_term ng
 */
extern bool OSPeekMsgQue(message_queue_t *queue, message_queue_message_t *message);

/**
 * @brief Pop a message from the queue.
 * @x_syscall_num `0x1001c`
 * @param queue The message queue descriptor.
 * @param message The result message.
 * @retval true @x_term ok
 * @retval false @x_term ng
 */
extern bool OSGetMsgQue(message_queue_t *queue, message_queue_message_t *message);

/**
 * @brief Destroy a message queue descriptor.
 * @x_syscall_num `0x1001d`
 * @param queue The message queue descriptor.
 * @retval true @x_term ok
 * @retval false @x_term ng
 */
extern bool OSCloseMsgQue(message_queue_t *queue);

/**
 * @brief Get the current running thread's priority (slot number).
 * @details
 * So far only Pocket Challenge implements this syscall. Calling it on other devices will very likely cause the NOSYS
 * handler to be called, which in turn will crash the system.
 *
 * On other devices, this can be simulated using mutekix:
 *
 * @code{.c}
 * #include <muteki/threading.h>
 * #include <mutekix/threading.h>
 *
 * short OSGetCurrentlyRunningTCBPrio() {
 *     return OSGetThreadPriority(mutekix_thread_get_current());
 * }
 * @endcode
 *
 * @x_term require-krnllib
 *
 * @x_syscall_num `0x200a2`
 * @x_void_param
 * @return The current running thread's priority.
 */
extern short OSGetCurrentlyRunningTCBPrio(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_THREADING_H__
