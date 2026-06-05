/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file event.h
 * @brief Low-level UI event handing API.
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
void GetSysKeyState(key_press_event_config_t *config);

/**
 * @brief Configure key event generator.
 * @x_syscall_num `0x10033`
 * @param[in] config The configuration struct to be imported.
 * @x_void_return
 * @see key_press_event_config_t The config struct and details on the expected format.
 */
void SetSysKeyState(const key_press_event_config_t *config);

/**
 * @brief Discard all unprocessed UI events.
 * @x_syscall_num `0x1004a`
 * @x_void
 */
extern void ClearAllEvents(void);

/**
 * @brief Process pending events.
 * @details Exact purpose of this function is currently unclear.
 * @x_syscall_num `0x10046`
 * @param event pointer to a ::ui_event_t struct.
 * @retval true Some events were processed.
 * @retval false No event was processed.
 * @see TestKeyEvent Related function that specifically processes key events.
 */
extern bool TestPendEvent(ui_event_t *event);

/**
 * @brief Process pending key events.
 * @details Exact purpose of this function is currently unclear. It is at least responsible for beeping on key presses,
 * etc.
 * @x_syscall_num `0x1004b`
 * @param event pointer to a ::ui_event_t struct.
 * @retval true Some events were processed.
 * @retval false No event was processed.
 * @see TestPendEvent
 */
extern bool TestKeyEvent(ui_event_t *event);

/**
 * @brief Get event.
 * @details Exact purpose of this function is currently unclear. It may be responsible for setting some fields in
 * the ::ui_event_t struct.
 * @note This function will block when there is currently no event to be processed.
 * @x_syscall_num `0x1003f`
 * @param[out] event Pointer to a ::ui_event_t struct.
 * @retval true Some events were returned.
 * @retval false No event was returned.
 */
extern bool GetEvent(ui_event_t *event);

/**
 * @brief Invalidate an event struct
 * @details This function zeros out ui_event_t::event_type and ui_event_t::unk20.
 * @x_syscall_num `0x10049`
 * @param[out] event Pointer to a ::ui_event_t struct.
 * @x_void_return
 */
void ClearEvent(ui_event_t *event);

/**
 * @brief Manually set the state of the SHIFT toggle key.
 * @x_syscall_num `0x100ad`
 * @param new_state The new state.
 * @return The previous state.
 * @see toggle_key_state_e Valid toggle key states.
 */
unsigned short SetShiftState(unsigned short new_state);

/**
 * @brief Manually set the state of the CAPS toggle key.
 * @x_syscall_num `0x100ae`
 * @param new_state The new state.
 * @return The previous state.
 * @see toggle_key_state_e Valid toggle key states.
 */
unsigned short SetCapsState(unsigned short new_state);

/**
 * @brief Get the state of the SHIFT toggle key.
 * @x_syscall_num `0x100af`
 * @x_void_param
 * @return The current state.
 * @see toggle_key_state_e Valid toggle key states.
 */
unsigned short GetShiftState(void);

/**
 * @brief Get the state of the SHIFT toggle key.
 * @x_syscall_num `0x100af`
 * @x_void_param
 * @return The current state.
 * @see toggle_key_state_e Valid toggle key states.
 */
unsigned short GetCapsState(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_EVENT_H__
