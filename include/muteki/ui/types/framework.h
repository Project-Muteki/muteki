/*
 * Copyright 2024-2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file framework.h
 * @brief UI framework type (component, group, event) definitions.
 */
#ifndef __MUTEKI_UI_TYPES_FRAMEWORK_H__
#define __MUTEKI_UI_TYPES_FRAMEWORK_H__

#include "../../draw/types/primitives.h"
#include "event.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Component behavior flag. Usually set by the component constructors but can sometimes change over time.
 * @todo Still need to verify these on the real machine.
 */
enum ui_component_flag_e {
    /**
     * @brief Component is selectable. 
     * @details Affects the group child selection through PGROUP_setCurrent() and input event processing. Setting this bit to 0 blocks the above 2.
     */
    UI_COMPONENT_FLAG_SELECTABLE = 0x0001,
    /**
     * @brief Component is a terminal node when resolving focusable items in a container.
     * @details When a container is finding a component that will gain focus, stop the search immediately and select this component.
     */
    UI_COMPONENT_FLAG_FOCUS_TERMINAL = 0x0002,
    /**
     * @brief Skip this component when searching for the next component to gain focus.
     */
    UI_COMPONENT_FLAG_FOCUS_SKIP = 0x0004,
    /**
     * @brief Deskbox-specific: Bubble ::KEY_HOME presses to system.
     * @details Requires ::UI_DESKBOX_FLAG_ALLOW_SYSTEM_EVENTS to properly work.
     */
    UI_COMPONENT_FLAG_DESKBOX_ALLOW_HOME_KEY = 0x0100,
    /**
     * @brief Deskbox-specific: Handle system events and exit word.
     */
    UI_COMPONENT_FLAG_DESKBOX_ALLOW_SYSTEM_EVENTS = 0x0400,
    /**
     * @brief Group-specific: Handle exit word.
     */
    UI_COMPONENT_FLAG_GROUP_ALLOW_EXIT_WORD = 0x0400,
    /**
     * @brief Deskbox-specific: Bubble translator and search key presses to system.
     */
    UI_COMPONENT_FLAG_DESKBOX_ALLOW_SEARCH_KEYS = 0x0800,
    /**
     * @brief Deskbox-specific: Invoke the inline search modal with the selected text instead of the normal search modal on ::KEY_SEARCH key presses.
     */
    UI_COMPONENT_FLAG_DESKBOX_ALLOW_INLINE_SEARCH = 0x2000,
    /**
     * @brief Group-specific: Do not periodically draw this group.
     */
    UI_COMPONENT_FLAG_GROUP_NO_AUTO_DRAW = 0x8000,
    /**
     * @brief Deskbox-specific: Bubble app key presses to system. 
     */
    UI_COMPONENT_FLAG_DESKBOX_ALLOW_APP_KEYS = 0x8000,
};

struct ui_component_s;
struct ui_event_std_s;
struct ui_event_prime_s;
struct ui_event_ext_s;
struct ui_event_ext_prime_s;
struct ui_event_sys_s;
struct ui_multipress_event_s;
struct ui_group_s;

typedef struct ui_component_s ui_component_t;
typedef struct ui_event_std_s ui_event_std_t;
typedef struct ui_event_prime_s ui_event_prime_t;
typedef struct ui_event_ext_s ui_event_ext_t;
typedef struct ui_event_ext_prime_s ui_event_ext_prime_t;
typedef struct ui_event_sys_s ui_event_sys_t;
typedef struct ui_multipress_event_s ui_multipress_event_t;
typedef struct ui_group_s ui_group_t;

#if defined(MUTEKI_HAS_PRIME_UI_EVENT) && MUTEKI_HAS_PRIME_UI_EVENT == 1
#define ui_event_s ui_event_prime_s
typedef struct ui_event_prime_s ui_event_t;
#else
#define ui_event_s ui_event_std_s
typedef struct ui_event_std_s ui_event_t;
#endif

/**
 * @brief Common part of all widgets and modals.
 * @todo Find a way to make the callbacks respect subtypes? Currently type-specific callbacks need to have their `self`
 * argument defined as `ui_component_t *` (or `void *` if we opt to do that) which is a bit less ideal.
 */
struct ui_component_s {
    /**
     * @brief Next component in a component ring or list.
     * @details This will be set to `NULL` when initialized, and, depending on context, will be a pointer to the next
     * component, a pointer to this component or `NULL`.
     */
    ui_component_t *next; // 0x0:0x4
    /**
     * @brief Parent component.
     * @details This will usually be a group.
     */
    ui_component_t *parent; // 0x4:0x8
    /**
     * @brief Component state bit field.
     * @details Each component may use this differently to represent different states.
     */
    unsigned short state; // 0x8:0xa
    /**
     * @brief Component behavior bit field.
     * @see ui_component_flag_e
     */
    unsigned short flags; // 0xa:0xc
    /**
     * @brief Component visual footprint.
     * @details This defines the size and location of the component on screen.
     */
    rect_t footprint; // 0xc:0x14
    /**
     * @brief Component state change callback.
     * @see PVIEW_setState
     */
    void (*on_set_state)(ui_component_t *self, unsigned short mask, bool value); // 0x14:0x18
    /**
     * @brief Component callback for erasing background.
     * @todo Figure out exactly what this does.
     * @see PVIEW_EraseBackGround
     */
    void (*on_erase_bg)(ui_component_t *self); // 0x18:0x1c
    /**
     * @brief Component draw callback.
     * @see PVIEW_draw
     */
    void (*on_draw)(ui_component_t *self); // 0x1c:0x20
    /**
     * @brief Component event callback.
     * @see PVIEW_handleEvent
     */
    void (*on_event)(ui_component_t *self, ui_event_t *event); // 0x20:0x24
    /**
     * @brief User data pointer. Content is component-specific.
     */
    void *user_data; // 0x24:0x2a
    /**
     * @brief @x_term tag
     */
    unsigned short tag;
    /**
     * @brief Unknown. Set to 0x65 on buttons.
     */
    unsigned short runtime_type; // 0x2a:0x2c
    /**
     * @brief Unknown.
     */
    int unk_0x2c; // 0x2c:0x30
    /**
     * @brief Unknown.
     */
    unsigned short unk_0x30; // 0x30:0x32
    /**
     * @brief Unknown. Probably padding.
     */
    short unk_0x32; // 0x32:0x34
};

/**
 * @brief UI component group/container struct.
 * @details This is implemented as a singly-linked ring of components.
 */
struct ui_group_s {
    /**
     * @brief The inherited component struct.
     */
    ui_component_t component;  // 0x0:0x34
    /**
     * @brief The current head item.
     */
    ui_component_t *current;  // 0x34:0x38
    /**
     * @brief Execute callback.
     * @see PGROUP_execute
     */
    int (*on_exec)(ui_group_t *self);  // 0x38:0x3c
    /**
     * @brief Redraw callback.
     * @see PGROUP_redraw
     */
    void (*on_redraw)(ui_group_t *self);  // 0x3c:0x40
    /**
     * @brief Find the component that has ::current as its next component.
     * @see PGROUP_preView
     */
    ui_component_t *(*on_find_previous)(ui_group_t *self);  // 0x40:0x44
    /**
     * @brief Set the current head component.
     * @see PGROUP_setCurrent
     */
    ui_component_t *(*on_set_current)(ui_group_t *self, ui_component_t *child);  // 0x44:0x48
    /**
     * @brief Insert a new component after ::on_find_previous.
     * @see PGROUP_insert
     */
    void (*on_insert)(ui_group_t *self, ui_component_t *child);  // 0x48:0x4c
    /**
     * @brief Execution result.
     * @details The default ::on_exec implementation will keep executing until this value is no longer 0, or exit word is
     * allowed and has been set to a non-zero value. Thus a value of 0 returned by ExecView() indicates that the
     * execution has been interrupted by an exit word.
     */
    int execution_result;  // 0x4c:0x50
};


/**
 * @brief Multipress/multitouch event.
 * @details This is a simplified version of the main UI event struct, that only contains the necessary fields to
 * represent a multitouch or a key-press event. Used on Prime G1.
 */
struct ui_multipress_event_s {
    /**
     * @brief Type of event.
     * @see ui_event_type_e List of event types.
     */
    unsigned int type;
    /**
     * @brief Finger ID of a touch event.
     */
    unsigned short finger_id;
    union {
        struct {
            /**
             * @brief Keycode for the first pressed key.
             */
            unsigned short key_code0;
            /**
             * @brief Keycode for the second pressed key (maybe unused).
             */
            unsigned short key_code1;
        };
        struct {
            /**
             * @brief The X coordinate of where the touch event is located, in pixels.
             * @details Only available when ::type is ::UI_EVENT_TYPE_TOUCH_BEGIN,
             * ::UI_EVENT_TYPE_TOUCH_MOVE, or ::UI_EVENT_TYPE_TOUCH_END.
             */
            unsigned short touch_x;
            /**
             * @brief The Y coordinate of where the touch event is located, in pixels.
             * @details Only available when ::type is ::UI_EVENT_TYPE_TOUCH_BEGIN,
             * ::UI_EVENT_TYPE_TOUCH_MOVE, or ::UI_EVENT_TYPE_TOUCH_END.
             */
            unsigned short touch_y;
        };
    };
    /**
     * @brief Unknown. Maybe unused and probably padding.
     */
    unsigned short unk_0xb;
};

/**
 * @brief Main event struct extension.
 * @details This is the event struct used by GetEvent() and PutEvent().
 */
struct ui_event_ext_s {
    union {
        /**
        * @brief Extra user data.
        * @details Format is event-specific. Usually unused.
        */
        void *user_data2; // 12-16 pointer that only shows up on USB insertion event.
        /**
         * @brief Extra user data as a scalar value.
         */
        unsigned int user_data2_scalar;
    };
    /**
     * @brief Seems to be tied to where the event comes from.
     * @details Set to 1 in SendMessage() events, 2 in PutEvent() events that is of neither the ::UI_EVENT_TYPE_KEY
     * type or the `0x20` type. Otherwise it would usually be 0.
     * @see ui_event_source_e
     */
    unsigned short event_source; // 20-22
    /**
     * @brief Unknown. Seems unused.
     */
    unsigned short unk22; // 22-24
};

struct ui_event_ext_prime_s {
    /**
     * @brief Number of valid multipress events available for processing.
     */
    unsigned short available_multipress_events; // 24-26
    /**
     * @brief Unknown. Sometimes can be 0x2 on startup.
     */
    unsigned short unk_0x1a; // 26-28
    /**
     * @brief The multipress events.
     */
    ui_multipress_event_t multipress_events[8]; // 28-124
};

/**
 * @brief Common struct subset shared across system and main events.
 * @details This is the event struct used by GetPenEvent() and PutSystemEvent().
 */
struct ui_event_sys_s {
    /**
     * @brief Event recipient.
     * @details If set to `NULL`, the event is a broadcast event (e.g. input event). Otherwise, the
     * widget's ui_component_t::on_event callback will be called with this event.
     * @note Unused in system events.
     */
    ui_component_t *recipient; // 0-4
    /**
     * @brief The type of event (0x10 being key event)
     * @see ui_event_type_e List of event types.
     */
    int event_type;
    union {
        /**
         * @brief Raw value of the event.
         */
        unsigned int value;
        struct {
            /**
             * @brief Keycode for the first pressed key.
             * @details Only available when ::event_type is ::UI_EVENT_TYPE_KEY.
             */
            unsigned short key_code0;
            /**
             * @brief Keycode for the second pressed key.
             * @details Only available when ::event_type is ::UI_EVENT_TYPE_KEY.
             * @note Depending on the exact keys pressed simultaneously, this is not always accurate. Moreover,
             * some devices may lack support of simultaneous key presses.
             */
            unsigned short key_code1;
        };
        struct {
            /**
             * @brief The X coordinate of where the touch event is located, in pixels.
             * @details Only available when ::event_type is ::UI_EVENT_TYPE_TOUCH_BEGIN,
             * ::UI_EVENT_TYPE_TOUCH_MOVE, or ::UI_EVENT_TYPE_TOUCH_END.
             */
            unsigned short touch_x;
            /**
             * @brief The Y coordinate of where the touch event is located, in pixels.
             * @details Only available when ::event_type is ::UI_EVENT_TYPE_TOUCH_BEGIN,
             * ::UI_EVENT_TYPE_TOUCH_MOVE, or ::UI_EVENT_TYPE_TOUCH_END.
             */
            unsigned short touch_y;
        };
    };
    union {
        /**
        * @brief User data.
        * @details Format is event-specific.
        */
        void *user_data; // 12-16 pointer that only shows up on USB insertion event.
        /**
         * @brief User data as a scalar value.
         */
        unsigned int user_data_scalar;
        /**
         * @brief Scroll wheel direction, either SCROLL_DOWN or SCROLL_UP.
         * @see scroll_wheel_e
         */
        unsigned int scroll_wheel;
    };
};

/**
 * @brief Structure for UI events.
 * @see ui_event_prime_s HP Prime G1's extension of this struct.
 */
struct ui_event_std_s {
    /**
     * @brief Event body.
     */
    ui_event_sys_t body;
    /**
     * @brief Extended main event data.
     */
    ui_event_ext_t ext;
};

/**
 * @brief Structure for low level UI events (Prime G1 extension).
 * @details Define `MUTEKI_HAS_PRIME_UI_EVENT` as 1 to make this the underlying type of ui_event_t.
 */
struct ui_event_prime_s {
    /**
     * @brief Event body.
     */
    ui_event_sys_t body;
    /**
     * @brief Extended main event data.
     */
    ui_event_ext_t ext;
    /**
     * @brief HP Prime multitouch extension.
     */
    ui_event_ext_prime_t ext_prime;
};

/**
 * @brief Event struct used by SendMessage().
 */
struct ui_message_s {
    /**
     * @brief The event struct.
     */
    ui_event_t event; // 0x0:0x18
    /**
     * @brief Unknown.
     */
    int unk_0x18; // 0x18:0x1c
}; // 0x1c bytes

#ifdef __cplusplus
} // extern "C"
#endif

#endif  // __MUTEKI_UI_TYPES_FRAMEWORK_H__
