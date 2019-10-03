#include "raylib.h"
#include "StateManager.h"

#include "Credits.h"
#include "Gameplay.h"
#include "Instructions.h"
#include "Intro.h"
#include "Options.h"
#include "Scoreboard.h"
#include "Title.h"

void wly_SetState(WLY_STATE newState){
    wly_prevState = wly_currentState;
    wly_currentState = newState;
      
    switch(newState){
    case WLY_STATE_Credits:
        wly_CurrentDraw2D = &wly_Credits_Draw;
        wly_CurrentUpdate = &wly_Credits_Update;
        wly_Credits_Init();
        break;
    case WLY_STATE_Instructions:
        wly_CurrentDraw2D = &wly_Instructions_Draw;
        wly_CurrentUpdate = &wly_Instructions_Update;
        wly_Instructions_Init();
        break;
    case WLY_STATE_Intro:
        wly_CurrentDraw2D = &wly_Intro_Draw;
        wly_CurrentUpdate = &wly_Intro_Update;
        wly_Intro_Init();
        break;
    case WLY_STATE_Gameplay:
        wly_CurrentDraw2D = &wly_Gameplay_Draw;
        wly_CurrentUpdate = &wly_Gameplay_Update;
        wly_Gameplay_Init();
        break;
    case WLY_STATE_Options:
        wly_CurrentDraw2D = &wly_Options_Draw;
        wly_CurrentUpdate = &wly_Options_Update;
        wly_Options_Init();
        break;
    case WLY_STATE_Scoreboard:
        wly_CurrentDraw2D = &wly_Scoreboard_Draw;
        wly_CurrentUpdate = &wly_Scoreboard_Update;
        wly_Scoreboard_Init();
        break;
    case WLY_STATE_Title:
        wly_CurrentDraw2D = &wly_Title_Draw;
        wly_CurrentUpdate = &wly_Title_Update;
        wly_Title_Init();
        break;
    }
}