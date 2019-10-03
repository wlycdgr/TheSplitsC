#include "../../splitslib/splitslib_a_Audio.h"
#include "../../splitslib/splitslib_d_Display.h"
#include "../../splitslib/splitslib_si_SemanticInput.h"
#include "../../splitslib/splitslib_sf_Splitsfont.h"
#include "../../splitslib/splitslib_ui_UserInterface.h"
#include "../../splitslib/splitslib_util_Utilities.h"
#include "SplitCanvas.h"
#include "StateManager.h"
#include "Title.h"

#define WLY_HeaderText "A THE SPLITS GAME"
#define WLY_HeaderTextSize 0.02
#define WLY_HeaderTextWeight 0.0025
#define WLY_HeaderTextKern 0.025
#define WLY_HeaderTextY 0.025

#define WLY_TitleTextThe "THE"
#define WLY_TitleTextFignermukcre "FIGNERMUKCRE"
#define WLY_TitleTextVariations "VARIATIONS"
#define WLY_TitleTextTheSize 0.075
#define WLY_TitleTextFignermukcreSize 0.2
#define WLY_TitleTextVariationsSize 0.15
#define WLY_TitleTextTheWeight 0.02
#define WLY_TitleTextFignermukcreWeight 0.0535
#define WLY_TitleTextWeight 0.04
#define WLY_TitleTextKern 0.001//0.01
#define WLY_TitleTextTheY 0.1
#define WLY_TitleTextFignermukcreY 0.25
#define WLY_TitleTextVariationsY 0.55

#define WLY_Menu_Main_HeaderText ""
#define WLY_Menu_Main_LabelText_One "PLAY"
#define WLY_Menu_Main_Size 1

#define WLY_Menu_Main_LinkItem_Play_Label "PLAY"
#define WLY_Menu_Main_LinkItem_Credits_Label "CREDITS"
#define WLY_Menu_Main_LinkItem_Options_Label "OPTIONS"
#define WLY_Menu_Main_LinkItem_Exit_Label "EXIT"

static void DoTheSelectedThing();
static void DrawHeader();
static void DrawTitle();
static void FreeResources();
static void InitMainMenu();
static void InitTitleStrings();

static splitslib_sf_Stringbox stringbox_Header;
static splitslib_sf_Stringbox stringbox_TitleThe;
static splitslib_sf_Stringbox stringbox_TitleFignermukcre;
static splitslib_sf_Stringbox stringbox_TitleVariations;
static splitslib_ui_Menu menu_Main;
int menuItem_Id_PlayLink;
int menuItem_Id_CreditsLink;
int menuItem_Id_OptionsLink;
int menuItem_Id_ExitLink;

void wly_Title_Init()
{
    InitTitleStrings();
    InitMainMenu();
}

void wly_Title_Update()
{
    splitslib_sf_AnimateString(stringbox_TitleThe.id);
    splitslib_sf_AnimateString(stringbox_TitleFignermukcre.id);
    splitslib_sf_AnimateString(stringbox_TitleVariations.id);
    
    if (SplitCanvas_IsTransitioning()){
        SplitCanvas_ContinueTransition();
    }
    else {
        splitslib_ui_UpdateMenu(&menu_Main);
        if (splitslib_si_IsMenuSelectPressed()){
            splitslib_a_PlayCommonSound(SPLITSLIB_SOUND_MenuSelect);
            FreeResources();
            DoTheSelectedThing();
        }
    }
}

void wly_Title_Draw()
{
    SplitCanvas_BeginTextureMode();
        DrawHeader();
        DrawTitle();
        splitslib_ui_DrawMenu(&menu_Main, false);
    SplitCanvas_EndTextureMode();
    
    SplitCanvas_DrawAll();
}

static void DoTheSelectedThing(){
    if (
        menuItem_Id_PlayLink ==
        menu_Main.cursor
    ){
        wly_SetState(WLY_STATE_ModeSelect); 
    }
    else if (
        menuItem_Id_CreditsLink ==
        menu_Main.cursor
    ){
        wly_SetState(WLY_STATE_Credits);
    }
    else if (
        menuItem_Id_OptionsLink ==
        menu_Main.cursor
    ){
        wly_SetState(WLY_STATE_Options);
    }
    else if (
        menuItem_Id_ExitLink ==
        menu_Main.cursor
    ){
        wly_SetState(WLY_STATE_EXIT);
    }
    
    SplitCanvas_StartTransition(WLY_WINDOW_WIDTH / 2.0);
}

static void DrawHeader(){
    splitslib_sf_DrawString(
        stringbox_Header.id,
        stringbox_Header.pos.x, stringbox_Header.pos.y);
}

static void DrawTitle(){
    splitslib_sf_DrawString(
        stringbox_TitleThe.id,
        stringbox_TitleThe.pos.x, 
        stringbox_TitleThe.pos.y
    );
    
    splitslib_sf_DrawString(
        stringbox_TitleFignermukcre.id,
        stringbox_TitleFignermukcre.pos.x, 
        stringbox_TitleFignermukcre.pos.y
    );
    
    splitslib_sf_DrawString(
        stringbox_TitleVariations.id,
        stringbox_TitleVariations.pos.x, 
        stringbox_TitleVariations.pos.y
    );
}

static void FreeResources(){
    splitslib_sf_FreeString(stringbox_Header.id);
    splitslib_sf_FreeString(stringbox_TitleThe.id);
    splitslib_sf_FreeString(stringbox_TitleFignermukcre.id);
    splitslib_sf_FreeString(stringbox_TitleVariations.id);
   
    splitslib_ui_FreeMenu(&menu_Main);
    
}

static void InitMainMenu()
{
    splitslib_ui_InitMenu(&menu_Main);
    menuItem_Id_PlayLink = splitslib_ui_AddLink(
        &menu_Main,
        WLY_Menu_Main_LinkItem_Play_Label,
        SPLITSLIB_UI_LABEL_ALIGNMENT_Center,
        (Vector2){
            splitslib_d_screen.center.x,
            splitslib_d_screen.height * 0.8
        }
    );
    menuItem_Id_CreditsLink = splitslib_ui_AddLink(
        &menu_Main,
        WLY_Menu_Main_LinkItem_Credits_Label,
        SPLITSLIB_UI_LABEL_ALIGNMENT_Center,
        (Vector2){
            splitslib_d_screen.center.x,
            splitslib_d_screen.height * 0.85
        }
    );
    menuItem_Id_OptionsLink = splitslib_ui_AddLink(
        &menu_Main,
        WLY_Menu_Main_LinkItem_Options_Label,
        SPLITSLIB_UI_LABEL_ALIGNMENT_Center,
        (Vector2){
            splitslib_d_screen.center.x,
            splitslib_d_screen.height * 0.9
        }
    );
    menuItem_Id_ExitLink = splitslib_ui_AddLink(
        &menu_Main,
        WLY_Menu_Main_LinkItem_Exit_Label,
        SPLITSLIB_UI_LABEL_ALIGNMENT_Center,
        (Vector2){
            splitslib_d_screen.center.x,
            splitslib_d_screen.height * 0.95
        }
    );
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
        &stringbox_TitleThe,
        WLY_TitleTextThe,
        WLY_TitleTextTheSize,
        WLY_TitleTextTheWeight,
        WLY_TitleTextKern,
        WLY_TitleTextTheY);
    splitslib_sf_SetRotationAlignment(
        stringbox_TitleThe.id, 
        SPLITSLIB_SF_RotationAlignmentArgyle
    );
    splitslib_sf_SetRotationSpeed(
        stringbox_TitleThe.id, 
        360
    );
        
    splitslib_sf_InitStringbox(
        &stringbox_TitleFignermukcre,
        WLY_TitleTextFignermukcre,
        WLY_TitleTextFignermukcreSize,
        WLY_TitleTextFignermukcreWeight,
        WLY_TitleTextKern,
        WLY_TitleTextFignermukcreY);
    splitslib_sf_SetRotationAlignment(
        stringbox_TitleFignermukcre.id, 
        SPLITSLIB_SF_RotationAlignmentArgyle
    );
    splitslib_sf_SetRotationSpeed(
        stringbox_TitleFignermukcre.id, 
        360
    );
    
    splitslib_sf_InitStringbox(
        &stringbox_TitleVariations,
        WLY_TitleTextVariations,
        WLY_TitleTextVariationsSize,
        WLY_TitleTextWeight,
        WLY_TitleTextKern,
        WLY_TitleTextVariationsY);
    splitslib_sf_SetRotationAlignment(
        stringbox_TitleVariations.id, 
        SPLITSLIB_SF_RotationAlignmentArgyle
    );
    splitslib_sf_SetRotationSpeed(
        stringbox_TitleVariations.id, 
        360
    );
}