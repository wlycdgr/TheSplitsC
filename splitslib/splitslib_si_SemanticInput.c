// brought into compliance with style guide

/*
An abstraction layer on top of the raylib input handling API
(which uses glfw input API library).

Gathers and serves answers to questions like "did the player just pause the game?"
without requiring the client to know or worry about whether the input
came from a keyboard, a gamepad, a mouse, etc
*/

#include "raylib.h"
#include "splitslib_si_SemanticInput.h"

bool splitslib_si_IsGoBackPressed(){
    return
        IsKeyDown(WLY_GoBackKey1);
}

bool splitslib_si_IsMenuDecreaseDown()
{
    return IsKeyDown(WLY_MenuDecreaseKey);
}

bool splitslib_si_IsMenuDecreasePressed()
{
    return IsKeyPressed(WLY_MenuDecreaseKey);
}

bool splitslib_si_IsMenuDecreaseReleased()
{
    return IsKeyReleased(WLY_MenuDecreaseKey);
}

bool splitslib_si_IsMenuIncreaseDown()
{
    return IsKeyDown(WLY_MenuIncreaseKey);
}

bool splitslib_si_IsMenuIncreasePressed()
{
    return IsKeyPressed(WLY_MenuIncreaseKey);
}

bool splitslib_si_IsMenuIncreaseReleased()
{
    return IsKeyReleased(WLY_MenuIncreaseKey);
}



bool splitslib_si_IsMenuScrollDownDown(){
    return 
        IsKeyDown(WLY_MenuScrollDownKey1) ||
        IsKeyDown(WLY_MenuScrollDownKey2);
}

bool splitslib_si_IsMenuScrollDownPressed(){
    return
        IsKeyPressed(WLY_MenuScrollDownKey1) ||
        IsKeyPressed(WLY_MenuScrollDownKey2);
}

bool splitslib_si_IsMenuScrollDownReleased(){
    return         
        IsKeyReleased(WLY_MenuScrollDownKey1) ||
        IsKeyReleased(WLY_MenuScrollDownKey2);
}

bool splitslib_si_IsMenuScrollUpDown(){
    return         
        IsKeyDown(WLY_MenuScrollUpKey1) ||
        IsKeyDown(WLY_MenuScrollUpKey2);
}

bool splitslib_si_IsMenuScrollUpPressed(){
    return         
        IsKeyPressed(WLY_MenuScrollUpKey1) ||
        IsKeyPressed(WLY_MenuScrollUpKey2);
}

bool splitslib_si_IsMenuScrollUpReleased(){
    return         
        IsKeyReleased(WLY_MenuScrollUpKey1) ||
        IsKeyReleased(WLY_MenuScrollUpKey2);
}

bool splitslib_si_IsMenuSelectPressed(){
    return         
        IsKeyPressed(WLY_MenuSelectKey1) ||
        IsGamepadButtonPressed(GAMEPAD_PLAYER1, WLY_MenuSelectGamepad1) ||
        IsGamepadButtonPressed(GAMEPAD_PLAYER1, WLY_MenuSelectGamepad2);
}

bool splitslib_si_IsMenuTogglePressed()
{
    return IsKeyPressed(WLY_MenuToggleKey);
}



bool splitslib_si_IsNextPressed()
{
    return
        IsKeyPressed(WLY_NextKey1);
}

bool splitslib_si_IsPausePressed(){
    return IsKeyPressed(WLY_PauseKey);
}

bool splitslib_si_IsSwitchActiveMenuPressed(){
    return IsKeyPressed(WLY_SwitchActiveMenuKey);
}

bool splitslib_si_IsSwitchPlaygridMoveDirectionPressed(){
    return IsKeyPressed(WLY_SwitchPlaygridMoveDirectionKey);
}

bool splitslib_si_IsSwitchPlaygridsPressed(){
    return IsKeyPressed(WLY_SwitchPlaygridsKey);
}