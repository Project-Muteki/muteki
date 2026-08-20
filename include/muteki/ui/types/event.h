/*
 * Copyright 2024-2026 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file event.h
 * @brief Event constants.
 */

#ifndef __MUTEKI_UI_TYPES_EVENT_H__
#define __MUTEKI_UI_TYPES_EVENT_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief List of available keycodes.
 * @details Keycodes starting with `KEY_PRIME_` are extended keycodes exclusive to HP Prime G1 (EA656).
 */
enum ui_keycode_e {
    /* 0x00 */
    KEY_ESC = 0x01,
    KEY_LEFT,
    KEY_UP,
    KEY_RIGHT,
    KEY_DOWN,
    KEY_PGUP,
    KEY_PGDN,
    KEY_CAPS = 0x0a,
    KEY_DEL = 0xc,
    KEY_ENTER = 0x0d,
    /* 0x10 */
    /**
     * @brief Bring up the function menu of the current scene.
     * @details Not to be confused with the function menu.
     */
    KEY_MENU = 0x11,
    /**
     * @brief Bring up the function menu of the current scene.
     * @details Alias of #KEY_MENU.
     */
    KEY_FUNC_MENU = 0x11,
    /**
     * @brief Change font size.
     */
    KEY_FONT = 0x12,
    /* 0x20 */
    KEY_SPACE = ' ',
    KEY_EXCL = '!',
    KEY_TAB = 0x22,
    KEY_HASH = '#',
    KEY_DOLLAR = '$',
    KEY_PERCENT = '%',
    KEY_LPAREN = '(',
    KEY_RPAREN = ')',
    KEY_STAR = '*',
    KEY_COMMA = ',',
    KEY_DASH = '-',
    KEY_DOT = '.',
    /* 0x30 */
    KEY_0 = '0',
    KEY_1 = '1',
    KEY_2 = '2',
    KEY_3 = '3',
    KEY_4 = '4',
    KEY_5 = '5',
    KEY_6 = '6',
    KEY_7 = '7',
    KEY_8 = '8',
    KEY_9 = '9',
    KEY_QUESTION = '?',
    /* 0x40 */
    KEY_AT = '@',
    KEY_A = 'A',
    KEY_B = 'B',
    KEY_C = 'C',
    KEY_D = 'D',
    KEY_E = 'E',
    KEY_F = 'F',
    KEY_G = 'G',
    KEY_H = 'H',
    KEY_I = 'I',
    KEY_J = 'J',
    KEY_K = 'K',
    KEY_L = 'L',
    KEY_M = 'M',
    KEY_N = 'N',
    KEY_O = 'O',
    /* 0x50 */
    KEY_P = 'P',
    KEY_Q = 'Q',
    KEY_R = 'R',
    KEY_S = 'S',
    KEY_T = 'T',
    KEY_U = 'U',
    KEY_V = 'V',
    KEY_W = 'W',
    KEY_X = 'X',
    KEY_Y = 'Y',
    KEY_Z = 'Z',
    /* 0x80 */
    /**
     * @brief Trigger TTS in Mandarin Chinese.
     */
    KEY_LANG_CHN = 0x80,
    /**
     * @brief Trigger TTS in Cantonese.
     */
    KEY_LANG_YUE = 0x81,
    /**
     * @brief Trigger TTS in English.
     */
    KEY_LANG_ENG = 0x82,
    /**
     * @brief Power button event.
     */
    KEY_POWER = 0x83,
    KEY_F1 = 0x84,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    /**
     * @brief Launch voice recorder.
     */
    KEY_APP_REC = 0x89,
    KEY_SHIFT = 0x8b,
    /**
     * @brief Switch input method.
     */
    KEY_IME = 0x8e,
    /**
     * @brief Trigger TTS in Japanese.
     */
    KEY_LANG_JPN = 0x8f,
    /* 0x90 */
    /**
     * @brief Activate the symbol selection input method.
     */
    KEY_SYMBOL = 0x91,
    /**
     * @brief Return to home menu.
     */
    KEY_HOME = 0x93,
    /**
     * @brief Return to home menu.
     * @details Alias of #KEY_HOME
     */
    KEY_HOME_MENU = 0x93,
    /**
     * @brief The MENU key on HP Prime.
     * @details Alias of #KEY_HOME
     */
    KEY_PRIME_MENU = 0x93,
    /** 
     * @brief Toggle simplified/traditional Chinese.
     */
    KEY_TOGGLE_SC_TC = 0x94,
    /**
     * @brief Show in-app help.
     */
    KEY_HELP = 0x95,
    /**
     * @brief Save current change and exit.
     */
    KEY_SAVE = 0x96,
    /**
     * @brief Launch MP3 player.
     */
    KEY_APP_MP3_PLAYER = 0x98,
    /**
     * @brief Open the Volume + Backlight adjustment menu.
     */
    KEY_VOL_BACKLIGHT = 0x9c,
    /**
     * @brief Dictionary: Pronounce the current word syllable-by-syllable.
     */
    KEY_SYLLABLE = 0x9e,
    /**
     * @brief Dictionary and search: Go back to the first screen the user initiated a search (with input memorized).
     */
    KEY_ORIGINAL_INPUT = 0x9f,
    /* 0xa0 */
    /**
     * @brief Board-specific app launching shortcut.
     */
    KEY_APP_MISC_3 = 0xa3,
    /**
     * @brief Launch the flashcard app.
     * @details Available on CA743.
     */
    KEY_APP_FLASHCARD = 0xa3,
    /**
     * @brief Board-specific app launching shortcut.
     */
    KEY_APP_MISC_5 = 0xa5,
    /**
     * @brief Launch Daijirin Japanese dictionary.
     * @details Available on JA738.
     */
    KEY_APP_DICT_JA_DAJIRIN = 0xa5,
    /**
     * @brief Launch the Games app.
     * @details Available on CA743.
     */
    KEY_APP_GAMES = 0xa5,
    /**
     * @brief Board-specific app launching shortcut.
     */
    KEY_APP_MISC_6 = 0xa6,
    /**
     * @brief Launch Shin-Meikai Japanese dictionary.
     * @details Available on JA738.
     */
    KEY_APP_DICT_JA_SHINMEIKAI = 0xa6,
    /**
     * @brief Launch the album app.
     * @details Available on CA743.
     */
    KEY_APP_ALBUM = 0xa6,
    /**
     * @brief Board-specific app launching shortcut.
     */
    KEY_APP_MISC_7 = 0xa7,
    /**
     * @brief Launch Longman dictionary.
     * @details Available on CA743.
     */
    KEY_APP_DICT_EN_LONGMAN = 0xa7,
    /**
     * @brief Launch Genius Japanese-English dictionary.
     * @details Available on JA738.
     */
    KEY_APP_DICT_JA_EN_GENIUS = 0xa7,
    /**
     * @brief Board-specific app launching shortcut.
     */
    KEY_APP_MISC_8 = 0xa8,
    /**
     * @brief Launch Genius English-Japanese dictionary.
     * @details Available on JA738.
     */
    KEY_APP_DICT_EN_JA_GENIUS = 0xa8,
    /* 0xb0 */
    KEY_PRIME_APPS = 0xb1,
    KEY_PRIME_PLOT,
    KEY_PRIME_NUM,
    KEY_PRIME_VIEW,
    KEY_PRIME_CAS,
    KEY_PRIME_ALPHA,
    KEY_PRIME_MINUS,
    KEY_PRIME_DECIMAL_POINT,
    KEY_PRIME_PLUS,
    /* 0xc0 */
    /**
     * @brief Show the Favorites menu.
     */
    KEY_FAV = 0xc0,
    /* 0xd0 */
    /**
     * @brief Launch video player app.
     */
    KEY_APP_VIDEO_PLAYER = 0xd0,
    /**
     * @brief Open the global auxilary menu on touchscreen-only devices.
     */
    KEY_AUX_MENU = 0xd1,
    /**
     * @brief Open the English to Chinese translator app.
     */
    KEY_APP_TRANSLATOR_EN_ZH = 0xd2,
    /**
     * @brief Open the Chinese to English translator app.
     */
    KEY_APP_TRANSLATOR_ZH_EN = 0xd3,
    /**
     * @brief Launch Besta professional vocabulary dictionary.
     */
    KEY_APP_DICT_EN_PRO = 0xd4,
    /* 0xe0 */
    /**
     * @brief Launch Besta Chinese-English dictionary.
     */
    KEY_APP_DICT_ZH_EN_BESTA = 0xe8,
    /**
     * @brief Launch Besta Japanese-Chinese dictionary.
     */
    KEY_APP_DICT_JA_ZH_BESTA = 0xe9,
    /**
     * @brief Launch Besta Chinese-Japanese dictionary.
     */
    KEY_APP_DICT_ZH_JA_BESTA = 0xea,
    /* 0xf0 */
    /**
     * @brief Launch Oxford dictionary.
     */
    KEY_APP_DICT_EN_OXFORD = 0xf2,
    /**
     * @brief Launch Besta English-Chinese dictionary.
     */
    KEY_APP_DICT_EN_ZH_BESTA = 0xf3,
    /**
     * @brief Show the edit menu (cut/copy/paste).
     */
    KEY_EDIT = 0xf5,
    /**
     * @brief Launch the unified search app.
     */
    KEY_SEARCH = 0xf6,
    /**
     * @brief Open the Backlight adjustment menu.
     * @details May be an alias to #KEY_VOL_BACKLIGHT.
     */
    KEY_BACKLIGHT = 0xf8,
    /**
     * @brief Dictionary: Parrot the current word and compare the pronunciations.
     */
    KEY_COMPARE = 0xf9,
    /**
     * @brief Open the Volume adjustment menu.
     * @details May be an alias to #KEY_VOL_BACKLIGHT.
     */
    KEY_VOL = 0xfa,
    /**
     * @brief Display battery indicator popup.
     */
    KEY_BATTERY = 0xfb,
    /**
     * @brief Open LCD backlight and contrast adjustment menu.
     */
    KEY_CONTRAST = 0xfc,
    /**
     * @brief Event on USB cable insertion.
     */
    KEY_USB_INSERTION = 0xfe,
    /**
     * @brief Dictionary: Repeatedly read out the current word.
     */
    KEY_REPEAT = 0xff,
    /* 0x100+ */
    KEY_INS = 0x101,
    /**
     * @brief Scroll wheel event.
     */
    KEY_SCROLL_WHEEL_EVENT = 0x102,
    /**
     * @brief Trigger TTS.
     * @details Some models may have this key instead of the usual `KEY_LANG_*` series keys for triggering TTS.
     */
    KEY_TTS = 0x104,
    KEY_SHIFT_PGUP = 0xe047,
    KEY_PRIME_HOME = 0xe047,
    KEY_SHIFT_PGDN = 0xe04f,
};

/**
 * @brief Scroll wheel direction.
 * @details Only confirmed on BA802.
 */
enum scroll_wheel_e {
    /**
     * @brief Scroll down (clockwise direction).
     */
    SCROLL_DOWN,
    /**
     * @brief Scroll up (counter-clockwise direction).
     */
    SCROLL_UP,
};

/**
 * @brief UI event types.
 */
enum ui_event_type_e {
    /**
     * @brief Invalid/cleared.
     */
    UI_EVENT_TYPE_INVALID = 0,
    /**
     * @brief Beginning of touch/pen down event.
     */
    UI_EVENT_TYPE_TOUCH_BEGIN = 1,
    /**
     * @brief Touch/pen move event.
     */
    UI_EVENT_TYPE_TOUCH_MOVE = 2,
    /**
     * @brief End of touch/pen up event.
     */
    UI_EVENT_TYPE_TOUCH_END = 8,
    /**
     * @brief Key(s) pressed.
     */
    UI_EVENT_TYPE_KEY = 16,
    /**
     * @brief Special event. Bypasses the queues and sets a flag instead. Exact purpose is unclear.
     */
    UI_EVENT_TYPE_SPECIAL = 32,
    /**
     * @brief Inter-component communication.
     */
    UI_EVENT_TYPE_COMPONENT_CMD = 0x0100,
    /**
     * @brief Internal system event. Exact purpose is unclear.
     * @see ui_event_internal_e
     */
    UI_EVENT_TYPE_INTERNAL = 0x4000,
    /**
     * @brief Key(s) released.
     * @details Available on S3C and TCC boards.
     */
    UI_EVENT_TYPE_KEY_UP = 0x100000,
};

/**
 * @brief Internal system UI event types.
 * @details
 * These values may go into the ui_event_t::value field when ui_event_t::event_type is ::UI_EVENT_TYPE_INTERNAL.
 *
 * Unlisted values behave similarly to ::UI_EVENT_INTERNAL_FLAG.
 */
enum ui_event_internal_e {
    /**
     * @brief Set a flag but do not queue the event.
     */
    UI_EVENT_INTERNAL_FLAG = 0,
    /**
     * @brief Request GetEvent() to do a low battery check.
     */
    UI_EVENT_INTERNAL_BATTERY_CHECK_REQUEST = 1,
    /**
     * @brief Queue this event normally into the system event queue. Exact purpose is unclear.
     */
    UI_EVENT_INTERNAL_BYPASS = 2,
};

/**
 * @brief Event source.
 * @details Describes how the event is produced and where it is being produced.
 * @note This can be unreliable sometimes as certain functions like PutEvent() have rules that bypass this value.
 */
enum ui_event_source_e {
    /**
     * @brief Event is a system event (from the system event queue).
     */
    UI_EVENT_SOURCE_SYSTEM = 0,
    /**
     * @brief Event is a message (produced via SendMessage()).
     */
    UI_EVENT_SOURCE_SEND_MESSAGE = 1,
    /**
     * @brief Event is a regular event and not a ::UI_EVENT_TYPE_KEY or ::UI_EVENT_TYPE_SPECIAL (from the main event
     * queue).
     */
    UI_EVENT_SOURCE_PUT_EVENT = 2,
};

#ifdef __cplusplus
} // extern "C"
#endif

#endif  // __MUTEKI_UI_TYPES_EVENT_H__
