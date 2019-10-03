/*
FLOW is

IN 
    -> LogoSequence

    IN -> 
LogoSequence 
   -> Title

    LogoSequence | Gameplay Mode Menu | Pause | Game Over ->
Title
    -> OUT
    -> Credits
    -> Options
    -> Gameplay
    
    Title -> 
Credits
    -> Title
    
    Title -> 
Options
    -> Title
    
    Title ->
Gameplay (substate)
    -> Pause
    -> Game Over

    Gameplay ->    
Pause (substate)
    -> Gameplay
    -> Title

    Gameplay ->
Game Over (substate)
    -> Gameplay
    -> Title
*/
    
#include "raylib.h"
#include "../splitslib/splitslib_ca_CommonAudio.h"
#include "../splitslib/splitslib_d_Display.h"
#include "../splitslib/splitslib_sf_Splitsfont.h"
#include "../splitslib/splitslib_ui_UserInterface.h"
#include "Options.h"
#include "SplitCanvas.h"
#include "StateManager.h"

#define WLY_TargetFPS 60
#define WLY_WindowTitle "The Splits"
#define WLY_IsFullscreen false

#define WLY_DEBUGMODE_ON true
#define WLY_DEBUGMODE_KEY_EXIT KEY_Z
    
int main()
{
    SetTargetFPS(WLY_TargetFPS);
    
    splitslib_d_InitDisplay(
        WLY_WindowTitle,
        WLY_WINDOW_WIDTH, WLY_WINDOW_HEIGHT,
        WLY_IsFullscreen);
    
    SplitCanvas_Init(WLY_WINDOW_WIDTH, WLY_WINDOW_HEIGHT);
        
    splitslib_sf_Init();
    
    splitslib_ui_InitReturnPrompt();
    
    wly_LoadOptions();
    
    InitAudioDevice();
    splitslib_ca_LoadCommonSounds();
    
    wly_SetState(WLY_STATE_Intro);
    
    wly_doMainLoop = true;
    while (wly_doMainLoop){        
        wly_CurrentUpdate();

        BeginDrawing();
            ClearBackground(WHITE);
            wly_CurrentDraw2D();     
        EndDrawing();
        
        if (WLY_DEBUGMODE_ON){
            if (IsKeyPressed(WLY_DEBUGMODE_KEY_EXIT))
                wly_doMainLoop = false;
        }
    }
    
    splitslib_ca_UnloadCommonSounds();
    CloseAudioDevice();
    
    CloseWindow();
    
    return 0;
}