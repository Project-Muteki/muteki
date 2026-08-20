/*
 * Copyright 2024-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file messagebox.h
 * @brief Functions related to the message box modal.
 */

#ifndef __MUTEKI_UI_MODALS_MESSAGEBOX_H__
#define __MUTEKI_UI_MODALS_MESSAGEBOX_H__

#include "../../common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Message box types.
 * @details
 * A valid message box type is either `UI_MB_DEFAULT` or a `UI_MB_ICON_` enum joined by one or more `UI_MB_BUTTON_` enums
 * with the bitwise-or operator (`|`). For example,
 * @code{.c}
 * MessageBox(_BUL("hello world"), UI_MB_ICON_WARNING | UI_MB_BUTTON_YES | UI_MB_BUTTON_NO);
 * @endcode
 * will cause a message box with the message `"hello world"`, the exclamation-point icon, and both the Yes and No
 * buttons to be shown.
 */
enum ui_messagebox_type_e {
    /**
     * @brief The default type.
     * @details This is usually equivalent to `(UI_MB_ICON_WARNING | UI_MB_BUTTON_OK)`.
     */
    UI_MB_DEFAULT = 0,
    /**
     * @brief Display an error (stop-sign) icon on the message box.
     */
    UI_MB_ICON_ERROR,
    /**
     * @brief Display an info icon (lower case `i`) on the message box.
     */
    UI_MB_ICON_INFO,
    /**
     * @brief Display a warning (exclamation-point) icon on the message box.
     */
    UI_MB_ICON_WARNING,
    /**
     * @brief Display a question-mark icon on the message box.
     */
    UI_MB_ICON_QUESTION,
    /**
     * @brief Provide `Yes` button on the message box.
     */
    UI_MB_BUTTON_YES = 1 << 8,
    /**
     * @brief Provide `No` button on the message box.
     */
    UI_MB_BUTTON_NO = 2 << 8,
    /**
     * @brief Provide `OK` button on the message box.
     */
    UI_MB_BUTTON_OK = 4 << 8,
    /**
     * @brief Provide `Cancel` button on the message box.
     */
    UI_MB_BUTTON_CANCEL = 8 << 8,
    /**
     * @brief Do not show any button.
     * @details This will hide all buttons on the message box. Any key press will dismiss the message box and the
     * return value will always be set to message_box_result_t::UI_MB_RESULT_OK.
     */
    UI_MB_BUTTON_NONE = 16 << 8,
};

/**
 * @brief Results returned by the MessageBox() function after the user dismisses the message box.
 */
enum ui_messagebox_result_e {
    /**
     * @brief The OK button was pressed by the user.
     * @details Pressing `Y` key on the keyboard could also trigger this (specifically when OK button is enabled via
     * ::UI_MB_BUTTON_OK).
     */
    UI_MB_RESULT_OK = 3,
    /**
     * @brief The Cancel button was pressed by the user.
     * @details Pressing `C` key or `ESC` key on the keyboard could also trigger this.
     */
    UI_MB_RESULT_CANCEL = 4,
    /**
     * @brief The Yes button was pressed by the user.
     * @details Pressing `Y` key on the keyboard could also trigger this (specifically when Yes button is enabled via
     * ::UI_MB_BUTTON_YES).
     */
    UI_MB_RESULT_YES = 5,
    /**
     * @brief The No button was pressed by the user.
     * @details Pressing `N` key or `ESC` key on the keyboard could also trigger this.
     */
    UI_MB_RESULT_NO = 6,
};

/**
 * @brief High level message box function.
 * @details
 * Create and show a message box view with the specified `text`, an icon and one or more buttons, and return
 * which button on the message box is pressed when the user dismisses it. The `type` argument controls the icon and
 * buttons shown on the message box view.
 *
 * Message boxes can be dismissed by 3 ways: Pressing the button on the message box (including selecting them and
 * pressing Enter), using the key bind (`Y` for OK/Yes, `N` for No and `C` for cancel) or pressing the ESC key when
 * Cancel-like operations are defined (i.e. Cancel or No, with Cancel taking precedence when both are shown).
 *
 * When OK and Yes are both available, what the user has selected takes precedence. That is, if the user selects Yes
 * button instead of the OK button with TAB or arrow keys, pressing the `Y` key on the keyboard will make the function
 * return ::UI_MB_RESULT_YES instead of ::UI_MB_RESULT_OK.
 *
 * @x_syscall_num `0x1013d`
 *
 * @param text UTF-16 encoded text to be displayed on the message box.
 * @param type The message box type.
 * @return Which button is pressed to dismiss the message box.
 * @see ui_messagebox_type_e Valid message box types.
 * @see ui_messagebox_result_e Valid return values of this function.
 */
extern int MessageBox(const UTF16 *text, unsigned short type);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_MODALS_MESSAGEBOX_H__
