#include "raylib.h"
#include "../splitslib/splitslib_d_Display.h"
#include "../splitslib/splitslib_sf_Splitsfont.h"
#include "../splitslib/splitslib_si_SemanticInput.h"
#include "../splitslib/splitslib_ui_UserInterface.h"
#include "Gameplay.h"
#include "SplitCanvas.h"
#include "StateManager.h"

#define WLY_GameOver_Text "GAME OVER"
#define WLY_GOMenuItem_PlayAgain_Text "PLAY AGAIN"
#define WLY_GOMenuItem_ExitToTitle_Text "EXIT TO TITLE"

#define WLY_Pause_Text "PAUSED"
#define WLY_PauseMenuItem_Continue_Text "CONTINUE"
#define WLY_PauseMenuItem_ExitToTitle_Text "EXIT TO TITLE"

#define WLY_MenuHeader_Y 40
#define WLY_MenuItem_1_Y 400
#define WLY_MenuItem_2_Y 440

#define WLY_MenuRibbon_BorderWidth 2
#define WLY_MenuRibbon_Bottom_Y 478
#define WLY_MenuRibbon_Top_Y 0

#define WLY_MENURIBBON_HEIGHT 480

static void CleanUpAndExitToTitle();
static void DoTheSelectedGOMenuThing();
static void DoTheSelectedPauseMenuThing();
static void DrawGameOverMenu();
static void DrawGameplay();
static void DrawMenuRibbonBorders();
static void DrawPauseMenu();
static void FreeStrings();
static void InitGameOverMenu();
static void InitPauseMenu();
static void LoadRenderTextures();
static void PauseGame();
static void RestartGame();
static void UnloadRenderTextures();

WLY_GAMEPLAYSUBSTATE currentSubstate;

static splitslib_sf_Stringbox stringbox_GameOver;
static splitslib_sf_Stringbox stringbox_GOMenuItem_PlayAgain;
static splitslib_sf_Stringbox stringbox_GOMenuItem_ExitToTitle;
static splitslib_ui_Menu GOMenu;

static splitslib_sf_Stringbox stringbox_Pause;
static splitslib_sf_Stringbox stringbox_PauseMenuItem_Continue;
static splitslib_sf_Stringbox stringbox_PauseMenuItem_ExitToTitle;
static splitslib_ui_Menu PauseMenu;

float frameTime;

RenderTexture2D rtexture_menuRibbon;
RenderTexture2D rtexture_menuRibbonFlipped;

void wly_Gameplay_Init(){
    LoadRenderTextures();
    
    InitGameOverMenu();
    InitPauseMenu();
    
    currentSubstate = WLY_GAMEPLAYSUBSTATE_Play;
}

void wly_Gameplay_Update(){
    if (SplitCanvas_IsTransitioning()){
        SplitCanvas_ContinueTransition();
        return;
    }
    
    switch(currentSubstate){
    case WLY_GAMEPLAYSUBSTATE_GameOver:
        splitslib_ui_ScrollMenu(&GOMenu);
        if (splitslib_si_IsMenuSelectPressed())
            DoTheSelectedGOMenuThing();
        break;
        
    case WLY_GAMEPLAYSUBSTATE_Pause:
        splitslib_ui_ScrollMenu(&PauseMenu);
        if (splitslib_si_IsMenuSelectPressed())
            DoTheSelectedPauseMenuThing();
        break;
        
    case WLY_GAMEPLAYSUBSTATE_Play:
        if (splitslib_si_IsPausePressed())
            PauseGame();
        frameTime = GetFrameTime();
        break;
    }
}

void wly_Gameplay_Draw(){
    switch(currentSubstate){
    case WLY_GAMEPLAYSUBSTATE_GameOver:
        DrawGameOverMenu();
        SplitCanvas_BeginTextureMode();
            DrawGameplay();
            DrawTexture(rtexture_menuRibbon.texture, 0, 120, WHITE);
        SplitCanvas_EndTextureMode();        
        break;
    
    case WLY_GAMEPLAYSUBSTATE_Pause:
        DrawPauseMenu();
        SplitCanvas_BeginTextureMode();
            DrawGameplay();
            DrawTexture(rtexture_menuRibbon.texture, 0, 120, WHITE);
        SplitCanvas_EndTextureMode();
        break;
        
    case WLY_GAMEPLAYSUBSTATE_Play:
        SplitCanvas_BeginTextureMode();
            DrawGameplay();
        SplitCanvas_EndTextureMode();
        break;
    }
    
    SplitCanvas_DrawAll();
}

static void CleanUpAndExitToTitle(){
    FreeStrings();
    wly_SetState(WLY_STATE_Title);
}

static void DoTheSelectedGOMenuThing(){    
    switch(GOMenu.cursor){
    case 0: // PLAY AGAIN
        RestartGame();
        break;
    case 1: // RETURN TO TITLE
        CleanUpAndExitToTitle();
        break;
    }
    
    SplitCanvas_StartTransition(WLY_WINDOW_WIDTH / 2.0);
}

static void DoTheSelectedPauseMenuThing(){
    switch(PauseMenu.cursor){
    case 0: // CONTINUE
        currentSubstate = WLY_GAMEPLAYSUBSTATE_Play;
        break;
    case 1: // RETURN TO TITLE
        CleanUpAndExitToTitle();
        break;
    }
    
    SplitCanvas_StartTransition(WLY_WINDOW_WIDTH / 2.0);
}

static void DrawGameOverMenu(){
    BeginTextureMode(rtexture_menuRibbonFlipped);
        DrawMenuRibbonBorders();
        
        splitslib_sf_DrawString(
            stringbox_GameOver.id,
            stringbox_GameOver.pos.x, WLY_MenuHeader_Y);
            
        splitslib_sf_DrawString(
            stringbox_GOMenuItem_PlayAgain.id,
            stringbox_GOMenuItem_PlayAgain.pos.x, WLY_MenuItem_1_Y);
        splitslib_sf_DrawString(
            stringbox_GOMenuItem_ExitToTitle.id,
            stringbox_GOMenuItem_ExitToTitle.pos.x, WLY_MenuItem_2_Y);
    EndTextureMode();
    BeginTextureMode(rtexture_menuRibbon);
        DrawTexture(rtexture_menuRibbonFlipped.texture, 0, 0, WHITE);
    EndTextureMode();
}

static void DrawGameplay(){
    
}

static void DrawMenuRibbonBorders(){
    DrawRectangle(0, WLY_MenuRibbon_Top_Y, WLY_WINDOW_WIDTH, WLY_MenuRibbon_BorderWidth, BLACK);
    DrawRectangle(0, WLY_MenuRibbon_Bottom_Y, WLY_WINDOW_WIDTH, WLY_MenuRibbon_BorderWidth, BLACK);
}

static void DrawPauseMenu(){
    BeginTextureMode(rtexture_menuRibbonFlipped);
        DrawMenuRibbonBorders();
        
        splitslib_sf_DrawString(
            stringbox_Pause.id,
            stringbox_Pause.pos.x, WLY_MenuHeader_Y);
            
        splitslib_sf_DrawString(
            stringbox_PauseMenuItem_Continue.id,
            stringbox_PauseMenuItem_Continue.pos.x, WLY_MenuItem_1_Y);
        splitslib_sf_DrawString(
            stringbox_PauseMenuItem_ExitToTitle.id,
            stringbox_PauseMenuItem_ExitToTitle.pos.x, WLY_MenuItem_2_Y);
    EndTextureMode();
    BeginTextureMode(rtexture_menuRibbon);
        DrawTexture(rtexture_menuRibbonFlipped.texture, 0, 0, WHITE);
    EndTextureMode();
}

static void FreeStrings(){
    splitslib_sf_FreeString(stringbox_GOMenuItem_PlayAgain.id);
    splitslib_sf_FreeString(stringbox_GOMenuItem_ExitToTitle.id);
    
    splitslib_sf_FreeString(stringbox_PauseMenuItem_Continue.id);
    splitslib_sf_FreeString(stringbox_PauseMenuItem_ExitToTitle.id);
}

static void InitGameOverMenu()
{
    splitslib_sf_InitStringbox(
        &stringbox_GameOver,
        WLY_GameOver_Text,
        WLY_Header_TextSize,
        WLY_Header_TextWeight,
        WLY_Header_TextKern,
        0);
        
    splitslib_sf_InitStringbox(
        &stringbox_GOMenuItem_PlayAgain,
        WLY_GOMenuItem_PlayAgain_Text,
        WLY_MenuItem_TextSize,
        WLY_MenuItem_TextWeight,
        WLY_MenuItem_TextKern,
        0);    
    splitslib_sf_InitStringbox(
        &stringbox_GOMenuItem_ExitToTitle,
        WLY_GOMenuItem_ExitToTitle_Text,
        WLY_MenuItem_TextSize,
        WLY_MenuItem_TextWeight,
        WLY_MenuItem_TextKern,
        0);
        
    GOMenu.cursor = 0;
    GOMenu.items[0] = &stringbox_GOMenuItem_PlayAgain;
    GOMenu.items[1] = &stringbox_GOMenuItem_ExitToTitle;
    GOMenu.itemCount = 2;
    GOMenu.scrollDir = 0;
    splitslib_ui_ResetScrollTimer(&GOMenu);
    splitslib_ui_UpdateMenuItemColors(&GOMenu);
}

static void InitPauseMenu(){
   splitslib_sf_InitStringbox(
        &stringbox_Pause,
        WLY_Pause_Text,
        WLY_Header_TextSize,
        WLY_Header_TextWeight,
        WLY_Header_TextKern,
        0);
        
    splitslib_sf_InitStringbox(
        &stringbox_PauseMenuItem_Continue,
        WLY_PauseMenuItem_Continue_Text,
        WLY_MenuItem_TextSize,
        WLY_MenuItem_TextWeight,
        WLY_MenuItem_TextKern,
        0);    
    splitslib_sf_InitStringbox(
        &stringbox_PauseMenuItem_ExitToTitle,
        WLY_PauseMenuItem_ExitToTitle_Text,
        WLY_MenuItem_TextSize,
        WLY_MenuItem_TextWeight,
        WLY_MenuItem_TextKern,
        0);
        
    PauseMenu.cursor = 0;
    PauseMenu.items[0] = &stringbox_PauseMenuItem_Continue;
    PauseMenu.items[1] = &stringbox_PauseMenuItem_ExitToTitle;
    PauseMenu.itemCount = 2;
    PauseMenu.scrollDir = 0;
    splitslib_ui_ResetScrollTimer(&PauseMenu);
    splitslib_ui_UpdateMenuItemColors(&PauseMenu); 
}

static void LoadRenderTextures(){
    rtexture_menuRibbon = LoadRenderTexture(WLY_WINDOW_WIDTH, WLY_MENURIBBON_HEIGHT);
    rtexture_menuRibbonFlipped = LoadRenderTexture(WLY_WINDOW_WIDTH, WLY_MENURIBBON_HEIGHT);
}

static void PauseGame(){
    currentSubstate = WLY_GAMEPLAYSUBSTATE_Pause;
    SplitCanvas_StartTransition(WLY_WINDOW_WIDTH / 2.0);
}

static void RestartGame(){
    currentSubstate = WLY_GAMEPLAYSUBSTATE_Play;
}

static void UnloadRenderTextures(){
    UnloadRenderTexture(rtexture_menuRibbon);
    UnloadRenderTexture(rtexture_menuRibbonFlipped);
}