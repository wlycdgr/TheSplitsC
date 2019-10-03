#ifndef SPLITSLIB_SI_SemanticInput_H
#define SPLITSLIB_SI_SemanticInput_H

// TODO implement gamepad scrolling once IsGamepadButtonPressed and IsGamepadButtonReleased are fixed
// (ray confirmed that they are buggy as of 10/16/2016 & will be fixed in 1.6)
#define WLY_GoBackKey1 KEY_ESCAPE
#define WLY_MenuDecreaseKey KEY_LEFT
#define WLY_MenuIncreaseKey KEY_RIGHT
#define WLY_MenuScrollDownGamepadDPad GAMEPAD_XBOX_BUTTON_DOWN
#define WLY_MenuScrollDownKey1 KEY_DOWN
#define WLY_MenuScrollDownKey2 KEY_S
#define WLY_MenuScrollUpKey1 KEY_UP
#define WLY_MenuScrollUpKey2 KEY_W
#define WLY_MenuSelectGamepad1 GAMEPAD_XBOX_BUTTON_A
#define WLY_MenuSelectGamepad2 GAMEPAD_XBOX_BUTTON_X
#define WLY_MenuSelectKey1 KEY_ENTER
#define WLY_MenuToggleKey KEY_SPACE
#define WLY_NextKey1 KEY_SPACE
#define WLY_PauseKey KEY_X
#define WLY_SwitchActiveMenuKey KEY_SPACE
#define WLY_SwitchPlaygridMoveDirectionKey KEY_C
#define WLY_SwitchPlaygridsKey KEY_SPACE

bool splitslib_si_IsGoBackPressed();
bool splitslib_si_IsMenuDecreaseDown();
bool splitslib_si_IsMenuDecreasePressed();
bool splitslib_si_IsMenuDecreaseReleased();
bool splitslib_si_IsMenuIncreaseDown();
bool splitslib_si_IsMenuIncreasePressed();
bool splitslib_si_IsMenuIncreaseReleased();
bool splitslib_si_IsMenuScrollDownDown();
bool splitslib_si_IsMenuScrollDownPressed();
bool splitslib_si_IsMenuScrollDownReleased();
bool splitslib_si_IsMenuScrollUpDown();
bool splitslib_si_IsMenuScrollUpPressed();
bool splitslib_si_IsMenuScrollUpReleased();
bool splitslib_si_IsMenuSelectPressed();
bool splitslib_si_IsMenuTogglePressed();
bool splitslib_si_IsNextPressed();
bool splitslib_si_IsPausePressed();
bool splitslib_si_IsSwitchActiveMenuPressed();
bool splitslib_si_IsSwitchPlaygridMoveDirectionPressed();
bool splitslib_si_IsSwitchPlaygridsPressed();

#endif