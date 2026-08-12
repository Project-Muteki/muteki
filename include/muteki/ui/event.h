/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file event.h
 * @brief Low-level UI event handing API.
 * @details
 * There are 2 event queues:
 * - The main event queue, which is primarily used for broadcasting events to components.
 * - The system event queue, which is primarily used for input events like key presses and touch.
 *
 * GetEvent() consolidates events from both event queues into a single handler.
 *
 * As a part of inter-component communication, the event subsystem also includes a way to send an event directly to a
 * UI component. See SendMessage() and SendMessageExt().
 */

#ifndef __MUTEKI_UI_EVENT_H__
#define __MUTEKI_UI_EVENT_H__

#include <muteki/ui/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Toggle key states.
 */
enum toggle_key_state_e {
    /**
     * @brief Toggle key is inactive.
     * @details No event will/should be modified.
     */
    TOGGLE_KEY_INACTIVE = 0,
    /**
     * @brief Toggle key is active.
     * @details Events may modified in case of SHIFT, and should be modified by the user in case of CAPS.
     */
    TOGGLE_KEY_ACTIVE,
    /**
     * @brief Toggle key is triggered and will deactivate itself.
     * @details This is only seen on SHIFT where once another key is pressed, the toggle state will be set to this
     * value.
     */
    TOGGLE_KEY_TRIGGERED,
};

/**
 * @brief Configuration struct for key press event generator.
 * @details
 * The generator is responsible for detecting long key presses and sending repeated "pressed" events when such long
 * presses are detected.
 *
 * The intervals below have the resolution of 25ms per step and the actual millisecond value needs to be calculated by
 * using `milliseconds = 25ms * (interval + 1)`. That is, setting the interval to 0 will give you 25ms of delay,
 * setting it to 1 will give you 50ms, and so forth.
 * 
 * The `beep_on_press` value sets the system configuration variable `0x10` (SYS_CONFIG_BEEP_ON_EVENT) that will reflect
 * in the settings app. This is, however, temporary and will reset when the user reboots the system.
 */
typedef struct {
    /**
     * @brief Delay interval before triggering long press repeat.
     */
    unsigned short long_press_delay;
    /**
     * @brief Repeat rate when long press repeat is triggered.
     */
    unsigned short long_press_repeat_rate;
    /**
     * @brief Whether or not to beep on key/touchscreen presses.
     */
    unsigned short beep_on_press;
} key_press_event_config_t;

/**
 * @brief Read current configuration of key event generator.
 * @x_syscall_num `0x10032`
 * @param[out] config The configuration struct for holding the exported configuration.
 * @x_void_return
 * @see key_press_event_config_t The config struct and details on the expected format.
 */
extern void GetSysKeyState(key_press_event_config_t *config);

/**
 * @brief Configure key event generator.
 * @x_syscall_num `0x10033`
 * @param[in] config The configuration struct to be imported.
 * @x_void_return
 * @see key_press_event_config_t The config struct and details on the expected format.
 */
extern void SetSysKeyState(const key_press_event_config_t *config);

/**
 * @brief Consume event from the system UI event queue (mostly for touch).
 * @details
 * If touch is active, this will generate a touch hold event on the fly. Similarly, a touch release event will be
 * generated if touch is not active. Due to this behavior, this function never blocks.
 * @note The name suggests that it is only for touch/pen event but it can also consume a key event, or anything
 * that is in the system event queue.
 * @x_syscall_num `0x1003b`
 * @param[out] event A simplified event struct that contains only the fields related to input.
 * @retval 0 No event was consumed.
 * @retval 1 Event is consumed.
 */
extern int GetPenEvent(ui_event_sys_t *event);

/**
 * @brief Check whether or not there are any pending system UI events.
 * @x_syscall_num `0x1003c`
 * @x_void_param
 * @retval 0 No event available, and touch is not active.
 * @retval 1 Some event is available, or touch is active.
 */
extern int CheckPenEvent(void);

/**
 * @brief Discard all unconsumed system UI events.
 * @x_syscall_num `0x1003d`
 * @x_void
 */
extern void ClearPenEvent(void);

/**
 * @brief Produce a system UI event.
 * @x_syscall_num `0x1003e`
 * @param event New system UI event.
 * @retval true Successfully signaled the subscribers.
 * @retval false Error occurred when signaling the subscribers.
 */
extern int PutSystemEvent(ui_event_sys_t *event);

/**
 * @brief Consume an event.
 * @details
 * This will try to consume the next event from firstly the main event queue, and when the main event queue is empty,
 * the system event queue.
 * @note This function will subscribe to the OS event of the UI event subsystem and will block when there is currently
 * no event to be processed.
 * @x_syscall_num `0x1003f`
 * @param[out] event Pointer to a ::ui_event_t struct.
 * @retval true Some events were consumed and returned.
 * @retval false No event was consumed and returned.
 */
extern bool GetEvent(ui_event_t *event);

/**
 * @brief Consume an event from the main event queue.
 * @details
 * This pops an event from the main event queue only without blocking.
 *
 * This is mostly used as a pass in GetEvent() internally.
 * @x_syscall_num `0x10040`
 * @param[out] event Event.
 * @retval true An event has been consumed.
 * @retval false No event has been consumed.
 */
extern bool GetPendEvent(ui_event_t *event);

/**
 * @brief Set the event type bit mask.
 * @details
 * The default is to allow all event types (`0xffffffff`).
 * If the corresponding bit is 0, the event processing with GetEvent() will stall until the bit is set back to 1.
 * @x_syscall_num `0x10041`
 * @param mask New event type bit mask of allowed event types.
 * @return The old event type bit mask.
 * @see ui_event_type_e List of event types.
 */
extern unsigned int SetEventType(unsigned int mask);

/**
 * @brief Get the current event type bit mask.
 * @x_syscall_num `0x10042`
 * @x_void_param
 * @return The current event type bit mask.
 */
extern unsigned int GetEventType(void);

/**
 * @brief Produce an event in the main event queue.
 * @x_syscall_num `0x10043`
 * @param[in, out] event The event.
 * @retval true Successfully signaled the subscribers.
 * @retval false Attempt to produce an invalidated event, or failed to signal the subscribers.
 */
extern bool PutEvent(ui_event_t *event);

/**
 * @brief Produce an event in the main event queue (by values).
 * @details This zero-allocates a temporary event struct on the stack, sets the user-specified fields and then calls
 * PutEvent() to produce an event.
 * @x_syscall_num `0x10045`
 * @param type Event type.
 * @param value Raw 32-bit event value.
 * @param user_data User data pointer.
 * @x_void_return
 * @see ui_event_type_e
 */
extern void PutEventExt(int type, unsigned int value, void *user_data);

/**
 * @brief Peek pending events in the main event queue, and also special events.
 * @x_syscall_num `0x10046`
 * @param[out] event pointer to a ::ui_event_t struct.
 * @retval true Some events need to be consumed. This may be a special event that has no data. In that case
 * event.event_type is cleared.
 * @retval false No event needs to be consumed.
 * @see TestKeyEvent Related function that peeks the system events.
 */
extern bool TestPendEvent(ui_event_t *event);

/**
 * @brief Empty all unconsumed events in the main event queue.
 * @x_syscall_num `0x10047`
 * @x_void
 */
extern void ClearPendEvent(void);

/**
 * @brief Reset the touch state tracker only.
 * @x_syscall_num `0x10048`
 * @x_void
 */
extern void ClearPenState(void);

/**
 * @brief Invalidate an event struct.
 * @details This function zeros out ui_event_t::event_type and ui_event_t::event_source.
 * @x_syscall_num `0x10049`
 * @param[in, out] event Pointer to a ::ui_event_t struct.
 * @x_void_return
 */
extern void ClearEvent(ui_event_t *event);

/**
 * @brief Discard unprocessed UI events.
 * @details This drains both the main and the system event queues, the ::UI_EVENT_TYPE_SPECIAL flag, and resets the
 * touch state tracker.
 * @note This does not clear flags related to ::UI_EVENT_TYPE_INTERNAL. However a ::UI_EVENT_TYPE_INTERNAL event with
 * value ::UI_EVENT_INTERNAL_BYPASS will be cleared during the clearing of the system event queue.
 * @x_syscall_num `0x1004a`
 * @x_void
 */
extern void ClearAllEvents(void);

/**
 * @brief Peek pending events in the system event queue.
 * @x_syscall_num `0x1004b`
 * @param[out] event pointer to a ::ui_event_t struct.
 * @retval true Some events were processed.
 * @retval false No event was processed.
 * @see TestPendEvent
 */
extern bool TestKeyEvent(ui_event_t *event);

/**
 * @brief Manually set the state of the SHIFT toggle key.
 * @x_syscall_num `0x100ad`
 * @param new_state The new state.
 * @return The previous state.
 * @see toggle_key_state_e Valid toggle key states.
 */
extern unsigned short SetShiftState(unsigned short new_state);

/**
 * @brief Manually set the state of the CAPS toggle key.
 * @x_syscall_num `0x100ae`
 * @param new_state The new state.
 * @return The previous state.
 * @see toggle_key_state_e Valid toggle key states.
 */
extern unsigned short SetCapsState(unsigned short new_state);

/**
 * @brief Get the state of the SHIFT toggle key.
 * @x_syscall_num `0x100af`
 * @x_void_param
 * @return The current state.
 * @see toggle_key_state_e Valid toggle key states.
 */
extern unsigned short GetShiftState(void);

/**
 * @brief Get the state of the SHIFT toggle key.
 * @x_syscall_num `0x100af`
 * @x_void_param
 * @return The current state.
 * @see toggle_key_state_e Valid toggle key states.
 */
extern unsigned short GetCapsState(void);

/**
 * @brief Synchronously send an event to a recipient, bypassing the event queuing mechanism.
 * @x_syscall_num `0x10188`
 * @param[in, out] event The event. Must contain a valid ui_event_t::recipient.
 * @return true Event is consumed by the recipient.
 * @return false Event is not consumed by the recipient or recipient is `NULL`.
 */
extern bool SendMessage(ui_event_t *event);

/**
 * @brief Synchronously send an event to a recipient (by values).
 * @details This allocates a temporary event struct on the stack, sets the user-specified fields and then calls
 * SendMessage() to send the generated event.
 * @x_syscall_num `0x10189`
 * @param[in, out] recipient The UI component receiving the event.
 * @param event_type Event type.
 * @param value Raw 32-bit event value.
 * @param[in, out] user_data User data pointer.
 * @return true Event is consumed by the recipient.
 * @return false Event is not consumed by the recipient or recipient is `NULL`.
 */
extern bool SendMessageExt(ui_component_t *recipient, int event_type, unsigned int value, void *user_data);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_EVENT_H__
