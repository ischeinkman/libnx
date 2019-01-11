/**
 * @file swkbd.h
 * @brief Wrapper for using the swkbd (software keyboard) LibraryApplet.
 * @author yellows8
 * @copyright libnx Authors
 */
#pragma once
#include "../types.h"
#include "../services/applet.h"

typedef enum {
    SwkbdTextCheckResult_OK      = 0,  ///< Success, valid string.
    SwkbdTextCheckResult_Bad     = 1,  ///< Failure, invalid string. Error message is displayed in a message-box, pressing OK will return to swkbd again.
    SwkbdTextCheckResult_Prompt  = 2,  ///< Failure, invalid string. Error message is displayed in a message-box, pressing Cancel will return to swkbd again, while pressing OK will continue as if the text was valid.
    SwkbdTextCheckResult_Silent  = 3,  ///< Failure, invalid string. With value 3 and above, swkbd will silently not accept the string, without displaying any error.
} SwkbdTextCheckResult;

typedef enum {
    SwkbdType_Normal = 0,  ///< Normal keyboard.
    SwkbdType_NumPad = 1,  ///< Number pad. The buttons at the bottom left/right are only available when they're set by \ref swkbdConfigSetLeftOptionalSymbolKey / \ref swkbdConfigSetRightOptionalSymbolKey.
    SwkbdType_QWERTY = 2,  ///< QWERTY (and variants) keyboard only.
} SwkbdType;

/// Bitmask for \ref SwkbdArgV0 keySetDisableBitmask. This disables keys on the keyboard when the corresponding bit(s) are set.
enum {
    SwkbdKeyDisableBitmask_Space        = BIT(1),  ///< Disable space-bar.
    SwkbdKeyDisableBitmask_At           = BIT(2),  ///< Disable '@'.
    SwkbdKeyDisableBitmask_Percent      = BIT(3),  ///< Disable '%'.
    SwkbdKeyDisableBitmask_ForwardSlash = BIT(4),  ///< Disable '/'.
    SwkbdKeyDisableBitmask_Backslash    = BIT(5),  ///< Disable '\'.
    SwkbdKeyDisableBitmask_Numbers      = BIT(6),  ///< Disable numbers.
    SwkbdKeyDisableBitmask_DownloadCode = BIT(7),  ///< Used for \ref swkbdConfigMakePresetDownloadCode.
    SwkbdKeyDisableBitmask_UserName     = BIT(8),  ///< Used for \ref swkbdConfigMakePresetUserName. Disables '@', '%', and '\'.
};

/// Value for \ref SwkbdArgV0 textDrawType. Only applies when stringLenMax is 1..32, otherwise swkbd will only use SwkbdTextDrawType_Box.
typedef enum {
    SwkbdTextDrawType_Line          = 0,  ///< The text will be displayed on a line. Also enables displaying the Header and Sub text.
    SwkbdTextDrawType_Box           = 1,  ///< The text will be displayed in a box.
    SwkbdTextDrawType_DownloadCode  = 2,  ///< Used by \ref swkbdConfigMakePresetDownloadCode on 5.0.0+. Enables using \ref SwkbdArgV7 unk_x3e0.
} SwkbdTextDrawType;

/// SwkbdInline Interactive input storage request ID.
typedef enum {
    SwkbdRequestCommand_Finalize = 0x4,
    SwkbdRequestCommand_SetCustomizeDic = 0x7,
    SwkbdRequestCommand_Calc = 0xA,
} SwkbdRequestCommand;

/// TextCheck callback set by \ref swkbdConfigSetTextCheckCallback, for validating the input string when the swkbd ok-button is pressed. This buffer contains an UTF-8 string. This callback should validate the input string, then return a \ref SwkbdTextCheckResult indicating success/failure. On failure, this function must write an error message to the tmp_string buffer, which will then be displayed by swkbd.
typedef SwkbdTextCheckResult (*SwkbdTextCheckCb)(char* tmp_string, size_t tmp_string_size);

/// Base swkbd arg struct.
typedef struct {
    SwkbdType type;                  ///< See \ref SwkbdType.
    u16 okButtonText[18/2];
    u16 leftButtonText;
    u16 rightButtonText;
    u8  dicFlag;                     ///< Enables dictionary usage when non-zero (including the system dictionary).
    u8  pad_x1b;
    u32 keySetDisableBitmask;        ///< See SwkbdKeyDisableBitmask_*.
    u32 initialCursorPos;            ///< Initial cursor position in the string: 0 = start, 1 = end.
    u16 headerText[130/2];
    u16 subText[258/2];
    u16 guideText[514/2];
    u16 pad_x3aa;
    u32 stringLenMax;                ///< When non-zero, specifies the max string length. When the input is too long, swkbd will stop accepting more input until text is deleted via the B button (Backspace). See also \ref SwkbdTextDrawType.
    u32 stringLenMaxExt;             ///< When non-zero, specifies the max string length. When the input is too long, swkbd will display an icon and disable the ok-button.
    u32 passwordFlag;                ///< Use password: 0 = disable, 1 = enable.
    SwkbdTextDrawType textDrawType;  ///< See \ref SwkbdTextDrawType.
    u16 returnButtonFlag;            ///< Controls whether the Return button is enabled, for newlines input. 0 = disabled, non-zero = enabled.
    u8  blurBackground;              ///< When enabled with value 1, the background is blurred.
    u8  pad_x3bf;
    u32 initialStringOffset;
    u32 initialStringSize;
    u32 userDicOffset;
    s32 userDicEntries;
    u8 textCheckFlag;
    u8 pad_x3d1[7];
    SwkbdTextCheckCb textCheckCb;  ///< This really doesn't belong in a struct sent to another process, but official sw does this.
} SwkbdArgV0;

/// Arg struct for version 0x30007+.
typedef struct {
    SwkbdArgV0 arg;
    u32 unk_x3e0[8];  ///< When set and enabled via \ref SwkbdTextDrawType, controls displayed text grouping (inserts spaces, without affecting output string).
} SwkbdArgV7;

typedef struct {
    SwkbdArgV7 arg;

    u8* workbuf;
    size_t workbuf_size;
    s32 max_dictwords;

    u32 version;
} SwkbdConfig;

/// User dictionary word.
typedef struct {
    u8 unk_x0[0x64];
} SwkbdDictWord;

typedef struct {
    u32 unk_x0;
    u8 mode;            ///< Controls the LibAppletMode when launching the applet. Non-zero indicates LibAppletMode_Background, otherwise LibAppletMode_Unknown3
    u8 unk_x5;          ///< Only set on 5.0.0+.
    u8 pad[2];
} SwkbdInitializeArg;

typedef struct {
    u32 unk_x0;
    u16 str[9];
    u8 unk_x16[0x4];
    u8 unk_x1a;
    u8 unk_x1b;
    u32 unk_x1c;
    s32 unk_x20;
    s32 unk_x24;
    u8 unk_x28;
    u16 unk_x29;
    u8 unk_x2b;
    u32 unk_x2c;
    u8 unk_x30;
    u8 unk_x31[0x17];
} SwkbdAppearArg;

typedef struct {
    u32 unk_x0;
    u16 size;                    ///< Size of this struct.
    u8 unk_x6;
    u8 unk_x7;
    u64 flags;
    SwkbdInitializeArg initArg;  ///< Flags bitmask 0x1.
    float volume;                ///< Flags bitmask 0x2.
    s32 cursorPos;               ///< Flags bitmask 0x10.
    SwkbdAppearArg appearArg;
    u8 unk_x68[0x3d4];
    u8 utf8Mode;                 ///< Flags bitmask 0x20.
    u8 unk_x43d;
    u8 enableBackspace;          ///< Flags bitmask 0x8000. Added with 5.x.
    u8 unk_x43f[3];
    u8 keytopAsFloating;         ///< Flags bitmask 0x200.
    u8 footerScalable;           ///< Flags bitmask 0x100.
    u8 alphaEnabledInInputMode;  ///< Flags bitmask 0x200.
    u8 inputModeFadeType;        ///< Flags bitmask 0x100.
    u8 disableTouch;             ///< Flags bitmask 0x200.
    u8 disableUSBKeyboard;       ///< Flags bitmask 0x800.
    u8 unk_x448[5];
    u16 unk_x44d;
    u8 unk_x44f;
    float keytopScale0;          ///< Flags bitmask 0x200.
    float keytopScale1;          ///< Flags bitmask 0x200.
    float keytopTranslate0;      ///< Flags bitmask 0x200.
    float keytopTranslate1;      ///< Flags bitmask 0x200.
    float keytopBgAlpha;         ///< Flags bitmask 0x100.
    float unk_x464;
    float balloonScale;          ///< Flags bitmask 0x200.
    float unk_x46c;
    u64 unk_x470[4];
    u8 unk_x490[0x10];
} PACKED SwkbdInlineCalcArg;

/// InlineKeyboard
typedef struct {
    u32 version;
    AppletHolder holder;
    SwkbdInlineCalcArg calcArg;
} SwkbdInline;

/**
 * @brief Creates a SwkbdConfig struct.
 * @param c SwkbdConfig struct.
 * @param max_dictwords Max \ref SwkbdDictWord entries, 0 for none.
 */
Result swkbdCreate(SwkbdConfig* c, s32 max_dictwords);

/**
 * @brief Closes a SwkbdConfig struct.
 * @param c SwkbdConfig struct.
 */
void swkbdClose(SwkbdConfig* c);

/**
 * @brief Clears the args in the SwkbdConfig struct and initializes it with the Default Preset.
 * @note Do not use this before \ref swkbdCreate.
 * @note Sets the following fields: type = \ref SwkbdType_QWERTY, initialCursorPos = 1, returnButtonFlag = 1, blurBackground = 1. Pre-5.0.0: textDrawType = SwkbdTextDrawType_Box.
 * @param c SwkbdConfig struct.
 */
void swkbdConfigMakePresetDefault(SwkbdConfig* c);

/**
 * @brief Clears the args in the SwkbdConfig struct and initializes it with the Password Preset.
 * @note Do not use this before \ref swkbdCreate.
 * @note Sets the following fields: type = \ref SwkbdType_QWERTY, initialCursorPos = 1, passwordFlag = 1, blurBackground = 1.
 * @param c SwkbdConfig struct.
 */
void swkbdConfigMakePresetPassword(SwkbdConfig* c);

/**
 * @brief Clears the args in the SwkbdConfig struct and initializes it with the UserName Preset.
 * @note Do not use this before \ref swkbdCreate.
 * @note Sets the following fields: type = \ref SwkbdType_Normal, keySetDisableBitmask = SwkbdKeyDisableBitmask_UserName, initialCursorPos = 1, blurBackground = 1.
 * @param c SwkbdConfig struct.
 */
void swkbdConfigMakePresetUserName(SwkbdConfig* c);

/**
 * @brief Clears the args in the SwkbdConfig struct and initializes it with the DownloadCode Preset.
 * @note Do not use this before \ref swkbdCreate.
 * @note Sets the following fields: type = \ref SwkbdType_Normal (\ref SwkbdType_QWERTY on 5.0.0+), keySetDisableBitmask = SwkbdKeyDisableBitmask_DownloadCode, initialCursorPos = 1, blurBackground = 1. 5.0.0+: stringLenMax = 16, stringLenMaxExt = 1, textDrawType = SwkbdTextDrawType_DownloadCode. unk_x3e0[0-2] = 0x3, 0x7, and 0xb.
 * @param c SwkbdConfig struct.
 */
void swkbdConfigMakePresetDownloadCode(SwkbdConfig* c);

/**
 * @brief Sets the Ok button text. The default is "".
 * @param c SwkbdConfig struct.
 * @param str UTF-8 input string.
 */
void swkbdConfigSetOkButtonText(SwkbdConfig* c, const char* str);

/**
 * @brief Sets the LeftOptionalSymbolKey, for \ref SwkbdType_NumPad. The default is "".
 * @param c SwkbdConfig struct.
 * @param str UTF-8 input string.
 */
void swkbdConfigSetLeftOptionalSymbolKey(SwkbdConfig* c, const char* str);

/**
 * @brief Sets the RightOptionalSymbolKey, for \ref SwkbdType_NumPad. The default is "".
 * @param c SwkbdConfig struct.
 * @param str UTF-8 input string.
 */
void swkbdConfigSetRightOptionalSymbolKey(SwkbdConfig* c, const char* str);

/**
 * @brief Sets the Header text. The default is "".
 * @note See \ref SwkbdArgV0 stringLenMax.
 * @param c SwkbdConfig struct.
 * @param str UTF-8 input string.
 */
void swkbdConfigSetHeaderText(SwkbdConfig* c, const char* str);

/**
 * @brief Sets the Sub text. The default is "".
 * @note See \ref SwkbdArgV0 stringLenMax.
 * @param c SwkbdConfig struct.
 * @param str UTF-8 input string.
 */
void swkbdConfigSetSubText(SwkbdConfig* c, const char* str);

/**
 * @brief Sets the Guide text. The default is "".
 * @note The swkbd applet only displays this when the current displayed cursor position is 0.
 * @param c SwkbdConfig struct.
 * @param str UTF-8 input string.
 */
void swkbdConfigSetGuideText(SwkbdConfig* c, const char* str);

/**
 * @brief Sets the Initial text. The default is "".
 * @param c SwkbdConfig struct.
 * @param str UTF-8 input string.
 */
void swkbdConfigSetInitialText(SwkbdConfig* c, const char* str);

/**
 * @brief Sets the user dictionary.
 * @param c SwkbdConfig struct.
 * @param input Input data.
 * @param entries Total entries in the buffer.
 */
void swkbdConfigSetDictionary(SwkbdConfig* c, const SwkbdDictWord *input, s32 entries);

/**
 * @brief Sets the TextCheck callback.
 * @param c SwkbdConfig struct.
 * @param cb \ref SwkbdTextCheckCb callback.
 */
void swkbdConfigSetTextCheckCallback(SwkbdConfig* c, SwkbdTextCheckCb cb);

/**
 * @brief Launch swkbd with the specified config. This will return once swkbd is finished running.
 * @note The string buffer is also used for the buffer passed to the \ref SwkbdTextCheckCb, when it's set. Hence, in that case this buffer should be large enough to handle TextCheck string input/output. The size passed to the callback is the same size passed here, -1.
 * @param c SwkbdConfig struct.
 * @param out_string UTF-8 Output string buffer.
 * @param out_string_size UTF-8 Output string buffer size, including NUL-terminator.
 */
Result swkbdShow(SwkbdConfig* c, char* out_string, size_t out_string_size);

/**
 * @brief Creates a SwkbdInline object.
 * @note This is essentially an asynchronous version of the regular swkbd.
 * @param s SwkbdInline object.
 */
Result swkbdInlineCreate(SwkbdInline* s);

/**
 * @brief Closes a SwkbdInline object. If the applet is running, this will tell the applet to exit, then wait for the applet to exit + applet exit handling.
 * @param s SwkbdInline object.
 */
Result swkbdInlineClose(SwkbdInline* s);

/**
 * @brief Launches the applet with the SwkbdInline object.
 * @param s SwkbdInline object.
 */
Result swkbdInlineLaunch(SwkbdInline* s);

/**
 * @brief Handles updating SwkbdInline state, this should be called periodically.
 * @note Handles applet exit if needed, and also sends the \ref SwkbdInlineCalcArg to the applet if needed. Hence, this should be called at some point after writing to \ref SwkbdInlineCalcArg.
 * @note Handles applet Interactive storage output when needed.
 * @param s SwkbdInline object.
 */
Result swkbdInlineUpdate(SwkbdInline* s);

/**
 * @brief Appear the kbd and set \ref SwkbdAppearArg. The applet will not start displaying on the screen with this.
 * @note \ref swkbdInlineUpdate must be called at some point afterwards for this to take affect.
 * @param s SwkbdInline object.
 * @param arg Input SwkbdAppearArg.
 */
void swkbdInlineAppear(SwkbdInline* s, SwkbdAppearArg* arg);

/**
 * @brief Disappear the kbd.
 * @note \ref swkbdInlineUpdate must be called at some point afterwards for this to take affect.
 * @param s SwkbdInline object.
 */
void swkbdInlineDisappear(SwkbdInline* s);

/**
 * @brief Creates a \ref SwkbdAppearArg which can then be passed to \ref swkbdInlineAppear.
 * @param arg Output \ref SwkbdAppearArg.
 * @param type Type. Must be 0..5, otherwise this will return.
 * @param flag Unknown flag
 * @param str Input UTF-8 string.
 */
void swkbdInlineMakeAppearArg(SwkbdAppearArg* arg, u32 type, bool flag, const char* str);
