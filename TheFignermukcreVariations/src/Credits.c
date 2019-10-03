#include "../../splitslib/splitslib_d_Display.h"
#include "../../splitslib/splitslib_si_SemanticInput.h"
#include "../../splitslib/splitslib_sf_Splitsfont.h"
#include "../../splitslib/splitslib_ui_UserInterface.h"
#include "Credits.h"
#include "SplitCanvas.h"
#include "StateManager.h"

#define WLY_ASplitsGame_Text "A THE SPLITS GAME"
#define WLY_ASplitsGame_TextSize 0.025
#define WLY_ASplitsGame_TextWeight 0.0025
#define WLY_ASplitsGame_TextKern 0.01
#define WLY_ASplitsGame_StartY 1.1f

#define WLY_Title_Text "THE FIGNERMUKCRE VARIATIONS"
#define WLY_Title_TextSize 0.075
#define WLY_Title_TextWeight 0.0075 //0.005
#define WLY_Title_TextKern 0.01 //0.02
#define WLY_Title_StartY 1.175f

#define WLY_MainCredit_Text "BY ILYA ZAREMBSKY"
#define WLY_MainCredit_TextSize 0.025
#define WLY_MainCredit_TextWeight 0.001
#define WLY_MainCredit_TextKern 0.02
#define WLY_MainCredit_StartY 1.3f

#define WLY_MainCredit2_Text "WITH SOUND BY MARLEY HALL"
#define WLY_MainCredit2_TextSize 0.025
#define WLY_MainCredit2_TextWeight 0.001
#define WLY_MainCredit2_TextKern 0.02
#define WLY_MainCredit2_StartY 1.35f

#define WLY_KSThx1_Text "VERY SPECIAL THANKS TO"
#define WLY_KSThx1_TextSize 0.025
#define WLY_KSThx1_TextWeight 0.005
#define WLY_KSThx1_TextKern 0.01
#define WLY_KSThx1_StartY 1.6f

#define WLY_KSThx2_Text "ALL OF THE GENEROUS AND PATIENT KICKSTARTER BACKERS"
#define WLY_KSThx2_TextSize 0.025
#define WLY_KSThx2_TextWeight 0.0025
#define WLY_KSThx2_TextKern 0.01
#define WLY_KSThx2_StartY 1.65f

#define WLY_KSThx3_Text "FULL LIST AT FB DOT COM SLASH LETSPLAYTHESPLITS"
#define WLY_KSThx3_TextSize 0.025
#define WLY_KSThx3_TextWeight 0.0025
#define WLY_KSThx3_TextKern 0.01
#define WLY_KSThx3_StartY 1.7f

#define WLY_MoreThxHeader_Text "MUCH THANKS ALSO TO"
#define WLY_MoreThxHeader_TextSize 0.025
#define WLY_MoreThxHeader_TextWeight 0.005
#define WLY_MoreThxHeader_TextKern 0.01
#define WLY_MoreThxHeader_StartY 1.9f

#define WLY_MoreThx1_Text "NATALIE VAYKUS AND SERGE ZAREMBSKY"
#define WLY_MoreThx1_TextSize 0.025
#define WLY_MoreThx1_TextWeight 0.0025
#define WLY_MoreThx1_TextKern 0.01
#define WLY_MoreThx1_StartY 1.95f

#define WLY_MoreThx2_Text "DAN TOROP"
#define WLY_MoreThx2_TextSize 0.025
#define WLY_MoreThx2_TextWeight 0.0025
#define WLY_MoreThx2_TextKern 0.01
#define WLY_MoreThx2_StartY 2.0f

#define WLY_MoreThx3_Text "CHARLES PRATT"
#define WLY_MoreThx3_TextSize 0.025
#define WLY_MoreThx3_TextWeight 0.0025
#define WLY_MoreThx3_TextKern 0.01
#define WLY_MoreThx3_StartY 2.05f

#define WLY_MoreThx4_Text "DYLAN MCKENZIE"
#define WLY_MoreThx4_TextSize 0.025
#define WLY_MoreThx4_TextWeight 0.0025
#define WLY_MoreThx4_TextKern 0.01
#define WLY_MoreThx4_StartY 2.1f

#define WLY_MoreThx5_Text "HAPPYFUN HIDEAWAY"
#define WLY_MoreThx5_TextSize 0.025
#define WLY_MoreThx5_TextWeight 0.0025
#define WLY_MoreThx5_TextKern 0.01
#define WLY_MoreThx5_StartY 2.15f

#define WLY_MoreThx6_Text "BABYCASTLES"
#define WLY_MoreThx6_TextSize 0.025
#define WLY_MoreThx6_TextWeight 0.0025
#define WLY_MoreThx6_TextKern 0.01
#define WLY_MoreThx6_StartY 2.2f

#define WLY_MoreThx7_Text "RAY RAYLIB SAN"
#define WLY_MoreThx7_TextSize 0.025
#define WLY_MoreThx7_TextWeight 0.0025
#define WLY_MoreThx7_TextKern 0.01
#define WLY_MoreThx7_StartY 2.25f

#define WLY_MoreThx8_Text "BENNETT FODDY"
#define WLY_MoreThx8_TextSize 0.025
#define WLY_MoreThx8_TextWeight 0.0025
#define WLY_MoreThx8_TextKern 0.01
#define WLY_MoreThx8_StartY 2.3f

#define WLY_SpecialDedication1_Text "FOR FRANK"
#define WLY_SpecialDedication1_TextSize 0.05
#define WLY_SpecialDedication1_TextWeight 0.005
#define WLY_SpecialDedication1_TextKern 0.01
#define WLY_SpecialDedication1_StartY 2.6f

#define WLY_SpecialDedication2_Text "HERE IS SOME CONTENT"
#define WLY_SpecialDedication2_TextSize 0.05
#define WLY_SpecialDedication2_TextWeight 0.005
#define WLY_SpecialDedication2_TextKern 0.01
#define WLY_SpecialDedication2_StartY 2.7f

#define WLY_ReturnPrompt_Text "SPACEBAR TO RETURN"
#define WLY_ReturnPrompt_TextSize 0.01f
#define WLY_ReturnPrompt_TextWeight 0.0005f
#define WLY_ReturnPrompt_TextKern 0.01f
#define WLY_ReturnPrompt_Y 0.95f

#define WLY_ScrollPerSecond 100.0f

static void FreeStrings();
static void InitStrings();
static void ScrollCredits();
static bool WeHaveReachedTheEndOfTheCredits();

static splitslib_sf_Stringbox stringbox_ASplitsGame;
static splitslib_sf_Stringbox stringbox_Title;
static splitslib_sf_Stringbox stringbox_MainCredit;
static splitslib_sf_Stringbox stringbox_MainCredit2;

static splitslib_sf_Stringbox stringbox_KSThx1;
static splitslib_sf_Stringbox stringbox_KSThx2;
static splitslib_sf_Stringbox stringbox_KSThx3;

static splitslib_sf_Stringbox stringbox_MoreThxHeader;
static splitslib_sf_Stringbox stringbox_MoreThx1;
static splitslib_sf_Stringbox stringbox_MoreThx2;
static splitslib_sf_Stringbox stringbox_MoreThx3;
static splitslib_sf_Stringbox stringbox_MoreThx4;
static splitslib_sf_Stringbox stringbox_MoreThx5;
static splitslib_sf_Stringbox stringbox_MoreThx6;
static splitslib_sf_Stringbox stringbox_MoreThx7;
static splitslib_sf_Stringbox stringbox_MoreThx8;

static splitslib_sf_Stringbox stringbox_SpecialDedication1;
static splitslib_sf_Stringbox stringbox_SpecialDedication2;

void wly_Credits_Init()
{
    InitStrings();
}

void wly_Credits_Update()
{
    if (SplitCanvas_IsTransitioning()){
        SplitCanvas_ContinueTransition();
    }
    else {
        ScrollCredits();
        
        if (
            IsKeyPressed(KEY_SPACE) ||
            WeHaveReachedTheEndOfTheCredits()
        ){
            FreeStrings();
            wly_SetState(WLY_STATE_Title);
            SplitCanvas_StartTransition(WLY_WINDOW_WIDTH / 2.0);
        }
    }
}

void wly_Credits_Draw()
{
    SplitCanvas_BeginTextureMode();
        splitslib_sf_DrawString(
            stringbox_ASplitsGame.id,
            stringbox_ASplitsGame.pos.x, stringbox_ASplitsGame.pos.y
        );    
        splitslib_sf_DrawString(
            stringbox_Title.id,
            stringbox_Title.pos.x, stringbox_Title.pos.y
        );
        splitslib_sf_DrawString(
            stringbox_MainCredit.id,
            stringbox_MainCredit.pos.x, stringbox_MainCredit.pos.y
        );
        splitslib_sf_DrawString(
            stringbox_MainCredit2.id,
            stringbox_MainCredit2.pos.x, stringbox_MainCredit2.pos.y
        );
        
        splitslib_sf_DrawString(
            stringbox_KSThx1.id,
            stringbox_KSThx1.pos.x, stringbox_KSThx1.pos.y
        );
        splitslib_sf_DrawString(
            stringbox_KSThx2.id,
            stringbox_KSThx2.pos.x, stringbox_KSThx2.pos.y
        );
        splitslib_sf_DrawString(
            stringbox_KSThx3.id,
            stringbox_KSThx3.pos.x, stringbox_KSThx3.pos.y
        );
            
        splitslib_sf_DrawString(
            stringbox_MoreThxHeader.id,
            stringbox_MoreThxHeader.pos.x, stringbox_MoreThxHeader.pos.y);
        splitslib_sf_DrawString(
            stringbox_MoreThx1.id,
            stringbox_MoreThx1.pos.x, stringbox_MoreThx1.pos.y);
        splitslib_sf_DrawString(
            stringbox_MoreThx2.id,
            stringbox_MoreThx2.pos.x, stringbox_MoreThx2.pos.y);
        splitslib_sf_DrawString(
            stringbox_MoreThx3.id,
            stringbox_MoreThx3.pos.x, stringbox_MoreThx3.pos.y);
        splitslib_sf_DrawString(
            stringbox_MoreThx4.id,
            stringbox_MoreThx4.pos.x, stringbox_MoreThx4.pos.y);
        splitslib_sf_DrawString(
            stringbox_MoreThx5.id,
            stringbox_MoreThx5.pos.x, stringbox_MoreThx5.pos.y);
        splitslib_sf_DrawString(
            stringbox_MoreThx6.id,
            stringbox_MoreThx6.pos.x, stringbox_MoreThx6.pos.y);
        splitslib_sf_DrawString(
            stringbox_MoreThx7.id,
            stringbox_MoreThx7.pos.x, stringbox_MoreThx7.pos.y);
        splitslib_sf_DrawString(
            stringbox_MoreThx8.id,
            stringbox_MoreThx8.pos.x, stringbox_MoreThx8.pos.y);
            
        splitslib_sf_DrawString(
            stringbox_SpecialDedication1.id,
            stringbox_SpecialDedication1.pos.x, 
            stringbox_SpecialDedication1.pos.y
        );
        splitslib_sf_DrawString(
            stringbox_SpecialDedication2.id,
            stringbox_SpecialDedication2.pos.x, 
            stringbox_SpecialDedication2.pos.y
        );        
        
        splitslib_ui_DrawReturnPrompt();
    SplitCanvas_EndTextureMode();
    
    SplitCanvas_DrawAll();
}

static void FreeStrings(){
    splitslib_sf_FreeString(stringbox_ASplitsGame.id);
    splitslib_sf_FreeString(stringbox_Title.id);
    splitslib_sf_FreeString(stringbox_MainCredit.id);
    splitslib_sf_FreeString(stringbox_MainCredit2.id);
    
    splitslib_sf_FreeString(stringbox_KSThx1.id);
    splitslib_sf_FreeString(stringbox_KSThx2.id);
    splitslib_sf_FreeString(stringbox_KSThx3.id);
    
    splitslib_sf_FreeString(stringbox_MoreThxHeader.id);
    splitslib_sf_FreeString(stringbox_MoreThx1.id);
    splitslib_sf_FreeString(stringbox_MoreThx2.id);   
    splitslib_sf_FreeString(stringbox_MoreThx3.id);
    splitslib_sf_FreeString(stringbox_MoreThx4.id);
    splitslib_sf_FreeString(stringbox_MoreThx5.id);
    splitslib_sf_FreeString(stringbox_MoreThx6.id);
    splitslib_sf_FreeString(stringbox_MoreThx7.id);
    splitslib_sf_FreeString(stringbox_MoreThx8.id);
    
    splitslib_sf_FreeString(stringbox_SpecialDedication1.id);
    splitslib_sf_FreeString(stringbox_SpecialDedication2.id);
}

static void InitStrings()
{
    splitslib_sf_InitStringbox(
        &stringbox_ASplitsGame,
        WLY_ASplitsGame_Text,
        WLY_ASplitsGame_TextSize,
        WLY_ASplitsGame_TextWeight,
        WLY_ASplitsGame_TextKern,
        WLY_ASplitsGame_StartY);
    splitslib_sf_InitStringbox(
        &stringbox_Title,
        WLY_Title_Text,
        WLY_Title_TextSize,
        WLY_Title_TextWeight,
        WLY_Title_TextKern,
        WLY_Title_StartY);
    splitslib_sf_InitStringbox(
        &stringbox_MainCredit,
        WLY_MainCredit_Text,
        WLY_MainCredit_TextSize,
        WLY_MainCredit_TextWeight,
        WLY_MainCredit_TextKern,
        WLY_MainCredit_StartY);
    splitslib_sf_InitStringbox(
        &stringbox_MainCredit2,
        WLY_MainCredit2_Text,
        WLY_MainCredit2_TextSize,
        WLY_MainCredit2_TextWeight,
        WLY_MainCredit2_TextKern,
        WLY_MainCredit2_StartY);
        
    splitslib_sf_InitStringbox(
        &stringbox_KSThx1,
        WLY_KSThx1_Text,
        WLY_KSThx1_TextSize,
        WLY_KSThx1_TextWeight,
        WLY_KSThx1_TextKern,
        WLY_KSThx1_StartY);
    splitslib_sf_InitStringbox(
        &stringbox_KSThx2,
        WLY_KSThx2_Text,
        WLY_KSThx2_TextSize,
        WLY_KSThx2_TextWeight,
        WLY_KSThx2_TextKern,
        WLY_KSThx2_StartY);
    splitslib_sf_InitStringbox(
        &stringbox_KSThx3,
        WLY_KSThx3_Text,
        WLY_KSThx3_TextSize,
        WLY_KSThx3_TextWeight,
        WLY_KSThx3_TextKern,
        WLY_KSThx3_StartY);
        
    splitslib_sf_InitStringbox(
        &stringbox_MoreThxHeader,
        WLY_MoreThxHeader_Text,
        WLY_MoreThxHeader_TextSize,
        WLY_MoreThxHeader_TextWeight,
        WLY_MoreThxHeader_TextKern,
        WLY_MoreThxHeader_StartY);
    splitslib_sf_InitStringbox(
        &stringbox_MoreThx1,
        WLY_MoreThx1_Text,
        WLY_MoreThx1_TextSize,
        WLY_MoreThx1_TextWeight,
        WLY_MoreThx1_TextKern,
        WLY_MoreThx1_StartY);
    splitslib_sf_InitStringbox(
        &stringbox_MoreThx2,
        WLY_MoreThx2_Text,
        WLY_MoreThx2_TextSize,
        WLY_MoreThx2_TextWeight,
        WLY_MoreThx2_TextKern,
        WLY_MoreThx2_StartY);
    splitslib_sf_InitStringbox(
        &stringbox_MoreThx3,
        WLY_MoreThx3_Text,
        WLY_MoreThx3_TextSize,
        WLY_MoreThx3_TextWeight,
        WLY_MoreThx3_TextKern,
        WLY_MoreThx3_StartY);
    splitslib_sf_InitStringbox(
        &stringbox_MoreThx4,
        WLY_MoreThx4_Text,
        WLY_MoreThx4_TextSize,
        WLY_MoreThx4_TextWeight,
        WLY_MoreThx4_TextKern,
        WLY_MoreThx4_StartY);
    splitslib_sf_InitStringbox(
        &stringbox_MoreThx5,
        WLY_MoreThx5_Text,
        WLY_MoreThx5_TextSize,
        WLY_MoreThx5_TextWeight,
        WLY_MoreThx5_TextKern,
        WLY_MoreThx5_StartY);
    splitslib_sf_InitStringbox(
        &stringbox_MoreThx6,
        WLY_MoreThx6_Text,
        WLY_MoreThx6_TextSize,
        WLY_MoreThx6_TextWeight,
        WLY_MoreThx6_TextKern,
        WLY_MoreThx6_StartY);
    splitslib_sf_InitStringbox(
        &stringbox_MoreThx7,
        WLY_MoreThx7_Text,
        WLY_MoreThx7_TextSize,
        WLY_MoreThx7_TextWeight,
        WLY_MoreThx7_TextKern,
        WLY_MoreThx7_StartY);
    splitslib_sf_InitStringbox(
        &stringbox_MoreThx8,
        WLY_MoreThx8_Text,
        WLY_MoreThx8_TextSize,
        WLY_MoreThx8_TextWeight,
        WLY_MoreThx8_TextKern,
        WLY_MoreThx8_StartY);
        
    splitslib_sf_InitStringbox(
        &stringbox_SpecialDedication1,
        WLY_SpecialDedication1_Text,
        WLY_SpecialDedication1_TextSize,
        WLY_SpecialDedication1_TextWeight,
        WLY_SpecialDedication1_TextKern,
        WLY_SpecialDedication1_StartY);
    splitslib_sf_InitStringbox(
        &stringbox_SpecialDedication2,
        WLY_SpecialDedication2_Text,
        WLY_SpecialDedication2_TextSize,
        WLY_SpecialDedication2_TextWeight,
        WLY_SpecialDedication2_TextKern,
        WLY_SpecialDedication2_StartY);
}

static void ScrollCredits(){
    float scrollAmount = WLY_ScrollPerSecond * GetFrameTime();
    
    stringbox_ASplitsGame.pos.y -= scrollAmount;
    stringbox_Title.pos.y -= scrollAmount;
    stringbox_MainCredit.pos.y -= scrollAmount;
    stringbox_MainCredit2.pos.y -= scrollAmount;
    
    stringbox_KSThx1.pos.y -= scrollAmount;
    stringbox_KSThx2.pos.y -= scrollAmount;
    stringbox_KSThx3.pos.y -= scrollAmount;
    
    stringbox_MoreThxHeader.pos.y -= scrollAmount;
    stringbox_MoreThx1.pos.y -= scrollAmount;
    stringbox_MoreThx2.pos.y -= scrollAmount;
    stringbox_MoreThx3.pos.y -= scrollAmount;
    stringbox_MoreThx4.pos.y -= scrollAmount;
    stringbox_MoreThx5.pos.y -= scrollAmount;
    stringbox_MoreThx6.pos.y -= scrollAmount;
    stringbox_MoreThx7.pos.y -= scrollAmount;
    stringbox_MoreThx8.pos.y -= scrollAmount;

    stringbox_SpecialDedication1.pos.y -= scrollAmount;
    stringbox_SpecialDedication2.pos.y -= scrollAmount;
}

static bool WeHaveReachedTheEndOfTheCredits(){
    return stringbox_SpecialDedication2.pos.y < -100.0f;
}