#ifndef StateManager_H
#define StateManager_H

#include "raylib.h"

typedef enum WLY_GAMEPLAYSUBSTATE {
    WLY_GAMEPLAYSUBSTATE_GameOver = 0,
    WLY_GAMEPLAYSUBSTATE_Pause,
    WLY_GAMEPLAYSUBSTATE_Play
} WLY_GAMEPLAYSUBSTATE;

typedef enum WLY_INTROSUBSTATE {
    WLY_INTROSUBSTATE_NYU = 0,
    WLY_INTROSUBSTATE_Raylib,
    WLY_INTROSUBSTATE_TrollcoreEnterprises
} WLY_INTROSUBSTATE;

typedef enum WLY_STATE{
    WLY_STATE_Credits = 0,
    WLY_STATE_Gameplay,
    WLY_STATE_Instructions,
    WLY_STATE_Intro,
    WLY_STATE_Options,
    WLY_STATE_Scoreboard,
    WLY_STATE_Title
} WLY_STATE;

typedef void (*wly_FuncptrVoidVoid)();
wly_FuncptrVoidVoid wly_CurrentDraw2D;
wly_FuncptrVoidVoid wly_CurrentUpdate;

void wly_SetState(WLY_STATE newState);

// please don't mess with these directly
WLY_STATE wly_currentState;
WLY_STATE wly_prevState;

bool wly_doMainLoop; // set this to false to break out of main loop & quit the game

#endif