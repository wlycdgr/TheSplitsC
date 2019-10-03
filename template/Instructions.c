#include "raylib.h"
#include "../splitslib/splitslib_d_Display.h"
#include "../splitslib/splitslib_sf_Splitsfont.h"
#include "SplitCanvas.h"
#include "StateManager.h"

#define WLY_ReturnPrompt_Text "SPACEBAR TO RETURN"
#define WLY_ReturnPrompt_TextSize 0.01f
#define WLY_ReturnPrompt_TextWeight 0.0005f
#define WLY_ReturnPrompt_TextKern 0.01f
#define WLY_ReturnPrompt_Y 0.95f

static void FreeStrings();
static void InitReturnPromptString();

static splitslib_sf_Stringbox stringbox_ReturnPrompt;

void wly_Instructions_Init(){
    InitReturnPromptString();
}

void wly_Instructions_Update(){
    if (SplitCanvas_IsTransitioning()){
        SplitCanvas_ContinueTransition();
    }
    else {
        if (IsKeyPressed(KEY_SPACE)){
            FreeStrings();
            wly_SetState(WLY_STATE_Title);
            SplitCanvas_StartTransition(WLY_WINDOW_WIDTH / 2.0);
        }
    }
}

void wly_Instructions_Draw(){
    SplitCanvas_BeginTextureMode();
        splitslib_sf_DrawString(
            stringbox_ReturnPrompt.id,
            splitslib_d_screen.width * 0.8, stringbox_ReturnPrompt.pos.y);
    SplitCanvas_EndTextureMode();
    
    SplitCanvas_DrawAll();
}

static void FreeStrings(){
    splitslib_sf_FreeString(stringbox_ReturnPrompt.id);
}

static void InitReturnPromptString(){
    splitslib_sf_InitStringbox(
        &stringbox_ReturnPrompt,
        WLY_ReturnPrompt_Text,
        WLY_ReturnPrompt_TextSize,
        WLY_ReturnPrompt_TextWeight,
        WLY_ReturnPrompt_TextKern,
        WLY_ReturnPrompt_Y);
}