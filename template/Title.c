#include "../splitslib/splitslib_d_Display.h"
#include "../splitslib/splitslib_si_SemanticInput.h"
#include "../splitslib/splitslib_sf_Splitsfont.h"
#include "../splitslib/splitslib_ui_UserInterface.h"
#include "../splitslib/splitslib_util_Utilities.h"

#include "SplitCanvas.h"
#include "StateManager.h"
#include "Title.h"

#define WLY_HeaderText "A SPLITS SINGLE"
#define WLY_HeaderTextSize 0.02
#define WLY_HeaderTextWeight 0.005
#define WLY_HeaderTextKern 0.025
#define WLY_HeaderTextY 0.1

#define WLY_TitleText "GAME TITLE"
#define WLY_TitleTextSize 0.2
#define WLY_TitleTextWeight 0.01
#define WLY_TitleTextKern 0.025
#define WLY_TitleTextY 0.2

#define WLY_MenuItem_Play_Text "PLAY"
#define WLY_MenuItem_Play_Y 0.6
#define WLY_MenuItem_Scoreboard_Text "SCOREBOARD"
#define WLY_MenuItem_Scoreboard_Y 0.65
#define WLY_MenuItem_Options_Text "OPTIONS"
#define WLY_MenuItem_Options_Y 0.7
#define WLY_MenuItem_Instructions_Text "INSTRUCTIONS"
#define WLY_MenuItem_Instructions_Y 0.75
#define WLY_MenuItem_Credits_Text "CREDITS"
#define WLY_MenuItem_Credits_Y 0.8
#define WLY_MenuItem_Exit_Text "EXIT"
#define WLY_MenuItem_Exit_Y 0.85

static void DoTheSelectedThing();
static void DrawHeader();
static void DrawMenu();
static void DrawTitle();
static void FreeStrings();
static void InitMainMenu();
static void InitTitleStrings();
static void ResetScrollTimer();

static void UpdateMenuItemColors();

static splitslib_sf_Stringbox stringbox_Header;
static splitslib_sf_Stringbox stringbox_Title;

static splitslib_sf_Stringbox stringbox_MenuItem_Play;
static splitslib_sf_Stringbox stringbox_MenuItem_Scoreboard;
static splitslib_sf_Stringbox stringbox_MenuItem_Options;
static splitslib_sf_Stringbox stringbox_MenuItem_Instructions;
static splitslib_sf_Stringbox stringbox_MenuItem_Credits;
static splitslib_sf_Stringbox stringbox_MenuItem_Exit;

static splitslib_ui_Menu mainMenu;

void wly_Title_Init()
{
    InitTitleStrings();
    InitMainMenu();
}

void wly_Title_Update()
{
    splitslib_sf_AnimateString(stringbox_Title.id);
    
    if (SplitCanvas_IsTransitioning()){
        SplitCanvas_ContinueTransition();
    }
    else {
        splitslib_ui_ScrollMenu(&mainMenu);
        if (splitslib_si_IsMenuSelectPressed()){
            FreeStrings();
            DoTheSelectedThing();
        }
    }
}

void wly_Title_Draw()
{
    SplitCanvas_BeginTextureMode();
        DrawHeader();
        DrawTitle();
        DrawMenu();
    SplitCanvas_EndTextureMode();
    
    SplitCanvas_DrawAll();
}

static void DoTheSelectedThing(){
    switch(mainMenu.cursor){
    case 0: // PLAY
        wly_SetState(WLY_STATE_Gameplay);
        break;
    case 1: // SCOREBOARD
        wly_SetState(WLY_STATE_Scoreboard);
        break;
    case 2: // OPTIONS
        wly_SetState(WLY_STATE_Options);
        break;
    case 3: // INSTRUCTIONS
        wly_SetState(WLY_STATE_Instructions);
        break;
    case 4: // CREDITS
        wly_SetState(WLY_STATE_Credits);
        break;
    case 5: // EXIT
        wly_doMainLoop = false;
        break;
    }
    
    if (5 != mainMenu.cursor)
        SplitCanvas_StartTransition(WLY_WINDOW_WIDTH / 2.0);
}

static void DrawHeader(){
    splitslib_sf_DrawString(
        stringbox_Header.id,
        stringbox_Header.pos.x, stringbox_Header.pos.y);
}

static void DrawMenu(){
    splitslib_sf_DrawString(
        stringbox_MenuItem_Play.id,
        stringbox_MenuItem_Play.pos.x, stringbox_MenuItem_Play.pos.y);
        
    splitslib_sf_DrawString(
        stringbox_MenuItem_Scoreboard.id,
        stringbox_MenuItem_Scoreboard.pos.x, stringbox_MenuItem_Scoreboard.pos.y);
        
    splitslib_sf_DrawString(
        stringbox_MenuItem_Options.id,
        stringbox_MenuItem_Options.pos.x, stringbox_MenuItem_Options.pos.y);
        
    splitslib_sf_DrawString(
        stringbox_MenuItem_Instructions.id,
        stringbox_MenuItem_Instructions.pos.x, stringbox_MenuItem_Instructions.pos.y);
        
    splitslib_sf_DrawString(
        stringbox_MenuItem_Credits.id,
        stringbox_MenuItem_Credits.pos.x, stringbox_MenuItem_Credits.pos.y);
        
    splitslib_sf_DrawString(
        stringbox_MenuItem_Exit.id,
        stringbox_MenuItem_Exit.pos.x, stringbox_MenuItem_Exit.pos.y);
}

static void DrawTitle(){
    splitslib_sf_DrawString(
        stringbox_Title.id,
        stringbox_Title.pos.x, stringbox_Title.pos.y);
}

static void FreeStrings(){
    splitslib_sf_FreeString(stringbox_Header.id);
    
    splitslib_sf_FreeString(stringbox_Title.id);
    
    splitslib_sf_FreeString(stringbox_MenuItem_Play.id);
    splitslib_sf_FreeString(stringbox_MenuItem_Scoreboard.id);
    splitslib_sf_FreeString(stringbox_MenuItem_Options.id);
    splitslib_sf_FreeString(stringbox_MenuItem_Instructions.id);
    splitslib_sf_FreeString(stringbox_MenuItem_Credits.id);
    splitslib_sf_FreeString(stringbox_MenuItem_Exit.id);
}

static void InitMainMenu()
{
    splitslib_sf_InitStringbox(
        &stringbox_MenuItem_Play,
        WLY_MenuItem_Play_Text,
        WLY_MenuItem_TextSize,
        WLY_MenuItem_TextWeight,
        WLY_MenuItem_TextKern,
        WLY_MenuItem_Play_Y);
        
    splitslib_sf_InitStringbox(
        &stringbox_MenuItem_Scoreboard,
        WLY_MenuItem_Scoreboard_Text,
        WLY_MenuItem_TextSize,
        WLY_MenuItem_TextWeight,
        WLY_MenuItem_TextKern,
        WLY_MenuItem_Scoreboard_Y);
        
    splitslib_sf_InitStringbox(
        &stringbox_MenuItem_Options,
        WLY_MenuItem_Options_Text,
        WLY_MenuItem_TextSize,
        WLY_MenuItem_TextWeight,
        WLY_MenuItem_TextKern,
        WLY_MenuItem_Options_Y);
        
    splitslib_sf_InitStringbox(
        &stringbox_MenuItem_Instructions,
        WLY_MenuItem_Instructions_Text,
        WLY_MenuItem_TextSize,
        WLY_MenuItem_TextWeight,
        WLY_MenuItem_TextKern,
        WLY_MenuItem_Instructions_Y);
        
    splitslib_sf_InitStringbox(
        &stringbox_MenuItem_Credits,
        WLY_MenuItem_Credits_Text,
        WLY_MenuItem_TextSize,
        WLY_MenuItem_TextWeight,
        WLY_MenuItem_TextKern,
        WLY_MenuItem_Credits_Y);
        
    splitslib_sf_InitStringbox(
        &stringbox_MenuItem_Exit,
        WLY_MenuItem_Exit_Text,
        WLY_MenuItem_TextSize,
        WLY_MenuItem_TextWeight,
        WLY_MenuItem_TextKern,
        WLY_MenuItem_Exit_Y);
        
    mainMenu.cursor = 0;
    mainMenu.items[0] = &stringbox_MenuItem_Play;
    mainMenu.items[1] = &stringbox_MenuItem_Scoreboard;
    mainMenu.items[2] = &stringbox_MenuItem_Options;
    mainMenu.items[3] = &stringbox_MenuItem_Instructions;
    mainMenu.items[4] = &stringbox_MenuItem_Credits;
    mainMenu.items[5] = &stringbox_MenuItem_Exit;
    mainMenu.itemCount = 6;
    mainMenu.scrollDir = 0;
    
    splitslib_ui_ResetScrollTimer(&mainMenu);
    splitslib_ui_UpdateMenuItemColors(&mainMenu);
}

static void InitTitleStrings(){
    splitslib_sf_InitStringbox(
        &stringbox_Header,
        WLY_HeaderText,
        WLY_HeaderTextSize,
        WLY_HeaderTextWeight,
        WLY_HeaderTextKern,
        WLY_HeaderTextY);
        
    splitslib_sf_InitStringbox(
        &stringbox_Title,
        WLY_TitleText,
        WLY_TitleTextSize,
        WLY_TitleTextWeight,
        WLY_TitleTextKern,
        WLY_TitleTextY);
}