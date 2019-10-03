#include "raylib.h"
#include "../splitslib/splitslib_d_Display.h"
#include "../splitslib/splitslib_sf_Splitsfont.h"
#include "../splitslib/splitslib_ui_UserInterface.h"
#include "Scoreboard.h"
#include "SplitCanvas.h"
#include "StateManager.h"

#define WLY_Header_Text "SCOREBOARD"

static void FreeStrings();

static splitslib_sf_Stringbox stringbox_Header;

static splitslib_sf_Stringbox stringbox_ReturnPrompt;

void wly_Scoreboard_Init(){
    splitslib_ui_InitHeaderString(&stringbox_Header, WLY_Header_Text);
}

void wly_Scoreboard_Update(){
    if (SplitCanvas_IsTransitioning()){
        SplitCanvas_ContinueTransition();
    }
    else if (IsKeyPressed(KEY_SPACE)){
        FreeStrings();
        wly_SetState(WLY_STATE_Title);
        SplitCanvas_StartTransition(WLY_WINDOW_WIDTH / 2.0);
    }
}

void wly_Scoreboard_Draw(){
    SplitCanvas_BeginTextureMode();
        splitslib_ui_DrawHeader(&stringbox_Header);
        splitslib_ui_DrawReturnPrompt();
    SplitCanvas_EndTextureMode();
    
    SplitCanvas_DrawAll();
}

static void FreeStrings(){
    splitslib_sf_FreeString(stringbox_Header.id);
}