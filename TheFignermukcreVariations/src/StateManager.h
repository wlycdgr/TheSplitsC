#ifndef StateManager_H
#define StateManager_H

#include "raylib.h"
#include "GameSettings.h"

typedef enum WLY_GAMEPLAY_STATE {
    WLY_GAMEPLAY_STATE_Dying = 0,
    WLY_GAMEPLAY_STATE_GameOver,
    WLY_GAMEPLAY_STATE_Pause,
    WLY_GAMEPLAY_STATE_Play
} WLY_GAMEPLAY_STATE;

typedef enum WLY_INTROSUBSTATE {
    WLY_INTROSUBSTATE_NYU = 0,
    WLY_INTROSUBSTATE_Raylib,
    WLY_INTROSUBSTATE_TrollcoreEnterprises
} WLY_INTROSUBSTATE;

typedef enum WLY_STATE{
    WLY_STATE_Credits = 0,
    WLY_STATE_EXIT,
    WLY_STATE_Gameplay_Mixed,
    WLY_STATE_Intro,
    WLY_STATE_ModeSelect,
    WLY_STATE_Options,
    WLY_STATE_Title
} WLY_STATE;

typedef void (*wly_FuncptrVoidVoid)();
wly_FuncptrVoidVoid wly_CurrentDraw2D;
wly_FuncptrVoidVoid wly_CurrentUpdate;

void wly_SetGameplayState(wly_GameSettings settings);
void wly_SetState(WLY_STATE newState);

// please don't mess with these directly
WLY_STATE wly_currentState;
WLY_STATE wly_prevState;

bool wly_doMainLoop; // set this to false to break out of main loop & quit the game

#endif