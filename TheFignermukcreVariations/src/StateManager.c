#include "raylib.h"
#include "StateManager.h"

#include "Credits.h"
#include "GameplayMixed.h"
#include "GameSettings.h"
#include "Intro.h"
#include "ModeSelect.h"
#include "Options.h"
#include "Title.h"

void wly_SetGameplayState(wly_GameSettings settings){
    wly_prevState = wly_currentState;
    wly_currentState = WLY_STATE_Gameplay_Mixed;
      
    wly_CurrentDraw2D = &wly_GameplayMixed_Draw;
    wly_CurrentUpdate = &wly_GameplayMixed_Update;
    wly_GameplayMixed_Init(settings);
}

void wly_SetState(WLY_STATE newState){
    wly_prevState = wly_currentState;
    wly_currentState = newState;
      
    switch(newState){
    case WLY_STATE_Credits:
        wly_CurrentDraw2D = &wly_Credits_Draw;
        wly_CurrentUpdate = &wly_Credits_Update;
        wly_Credits_Init();
        break;
    case WLY_STATE_EXIT:
        wly_doMainLoop = false;
        break;
    case WLY_STATE_Intro:
        wly_CurrentDraw2D = &wly_Intro_Draw;
        wly_CurrentUpdate = &wly_Intro_Update;
        wly_Intro_Init();
        break;
    case WLY_STATE_ModeSelect:
        wly_CurrentDraw2D = &wly_ModeSelect_Draw;
        wly_CurrentUpdate = &wly_ModeSelect_Update;
        wly_ModeSelect_Init();
        break;
    case WLY_STATE_Options:
        wly_CurrentDraw2D = &wly_Options_Draw;
        wly_CurrentUpdate = &wly_Options_Update;
        wly_Options_Init();
        break;
    case WLY_STATE_Title:
        wly_CurrentDraw2D = &wly_Title_Draw;
        wly_CurrentUpdate = &wly_Title_Update;
        wly_Title_Init();
        break;
    }
}