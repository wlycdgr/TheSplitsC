#include <assert.h>
#include "../splitslib/splitslib_d_Display.h"
#include "../splitslib/splitslib_si_SemanticInput.h"
#include "../splitslib/splitslib_sf_Splitsfont.h"
#include "SplitCanvas.h"
#include "StateManager.h"

#define WLY_NYUText "SUPPORTED BY THE"
#define WLY_NYUTextSize 0.01
#define WLY_NYUTextWeight 0.001
#define WLY_NYUTextKern 0.025
#define WLY_NYUTextY 0.05

#define WLY_RaylibText "BUILT USING"
#define WLY_RaylibTextSize 0.01
#define WLY_RaylibTextWeight 0.001
#define WLY_RaylibTextKern 0.025
#define WLY_RaylibTextY 0.05

#define WLY_TETMText1 "TROLLCORE ENTERPRISES TM"
#define WLY_TETMText1Size 0.075
#define WLY_TETMText1Weight 0.035
#define WLY_TETMText1Kern 0.0035
#define WLY_TETMText1Y 0.3
#define WLY_TETMText2 "AND"
#define WLY_TETMText2Size 0.01
#define WLY_TETMText2Weight 0.0005
#define WLY_TETMText2Kern 0.01
#define WLY_TETMText2Y 0.45
#define WLY_TETMText3 "THE CONTENT PRODUCTION CORPORATION"
#define WLY_TETMText3Size 0.035
#define WLY_TETMText3Weight 0.002
#define WLY_TETMText3Kern 0.025
#define WLY_TETMText3Y 0.525
#define WLY_TETMText4 "ARE CONTRACTUALLY BOUND TO PRESENT"
#define WLY_TETMText4Size 0.01
#define WLY_TETMText4Weight 0.0005
#define WLY_TETMText4Kern 0.01
#define WLY_TETMText4Y 0.7

#define WLY_NextText "SPACEBAR TO CONTINUE"
#define WLY_NextTextSize 0.01
#define WLY_NextTextWeight 0.0005
#define WLY_NextTextKern 0.01
#define WLY_NextTextY 0.95

#define WLY_Filepath_NYUIncubatorLogo "../img/logo_NYUIncubator_512.png"
#define WLY_Filepath_RaylibLogo "../img/logo_raylib_512.png"

#define WLY_MinTime_NYU 1.0f
#define WLY_NotificationTime_NYU 3.0f
#define WLY_MaxTime_NYU 7.0f
#define WLY_MinTime_Raylib 1.0f
#define WLY_NotificationTime_Raylib 3.0f
#define WLY_MaxTime_Raylib 7.0f
#define WLY_MinTime_TETM 1.0f
#define WLY_NotificationTime_TETM 3.0f
#define WLY_MaxTime_TETM 7.0f

static void AddTimeSinceLastFrameToSubstateTimer();
static void ChangeSubstateTo(WLY_INTROSUBSTATE newSubstate);
static void DrawAdvancePrompt();
static void DrawNYUIncubatorLogo();
static void DrawNYULogoLabel();
static void DrawNYUSubscreen();
static void DrawRaylibLogo();
static void DrawRaylibLogoLabel();
static void DrawRaylibSubscreen();
static void DrawTrollcoreEnterprisesSubscreen();
static void FreeStrings();
static void InitStrings();
static void LoadTextures();
static void ResetSubstateTimer();
static bool TimeIsUpOrPlayerHasPressedNext(float maxTime, float minTime);
static void UnloadTextures();
static bool WeAreFinishedWithThisSubstate();

static WLY_INTROSUBSTATE currentSubstate;
static float currentMinTime;
static float currentMaxTime;
static float substateTimer;

static Texture2D texture_NYUIncubatorLogo;
static Texture2D texture_RaylibLogo;

splitslib_sf_Stringbox stringbox_NYU;
splitslib_sf_Stringbox stringbox_Raylib;
splitslib_sf_Stringbox stringbox_TETM1;
splitslib_sf_Stringbox stringbox_TETM2;
splitslib_sf_Stringbox stringbox_TETM3;
splitslib_sf_Stringbox stringbox_TETM4;
splitslib_sf_Stringbox stringbox_Next;

RenderTexture2D canvas_Left;
RenderTexture2D canvas_Right;

bool isRedrawRequired;
bool isAdvancePromptDrawn;

void wly_Intro_Init()
{    
    InitStrings();
    LoadTextures();
    ChangeSubstateTo(WLY_INTROSUBSTATE_NYU);
    ResetSubstateTimer();
    
    isRedrawRequired = true;
    isAdvancePromptDrawn = false;
}

void wly_Intro_Update()
{
    if (SplitCanvas_IsTransitioning()){
        SplitCanvas_ContinueTransition();
    }
    else {
        AddTimeSinceLastFrameToSubstateTimer();
    }
    
    switch(currentSubstate){
    case WLY_INTROSUBSTATE_NYU:
        if (!isAdvancePromptDrawn && substateTimer > WLY_NotificationTime_NYU){
            isRedrawRequired = true;
            isAdvancePromptDrawn = true;
        }
        if (WeAreFinishedWithThisSubstate()){
            ChangeSubstateTo(WLY_INTROSUBSTATE_Raylib);
            SplitCanvas_StartTransition(WLY_WINDOW_WIDTH / 2.0);
        }
        break;
        
    case WLY_INTROSUBSTATE_Raylib:
        if (!isAdvancePromptDrawn && substateTimer > WLY_NotificationTime_Raylib){
           isRedrawRequired = true;
           isAdvancePromptDrawn = true;
        }
        if (WeAreFinishedWithThisSubstate()){
            ChangeSubstateTo(WLY_INTROSUBSTATE_TrollcoreEnterprises);
            SplitCanvas_StartTransition(WLY_WINDOW_WIDTH / 2.0);
        }
        break;
    case WLY_INTROSUBSTATE_TrollcoreEnterprises:
        if (!isAdvancePromptDrawn && substateTimer > WLY_NotificationTime_TETM){
           isRedrawRequired = true;
           isAdvancePromptDrawn = true;
        }
        if (WeAreFinishedWithThisSubstate()){
            UnloadTextures();
            FreeStrings();
            wly_SetState(WLY_STATE_Title);
            SplitCanvas_StartTransition(WLY_WINDOW_WIDTH / 2.0);
        }
        break;
    }
}

void wly_Intro_Draw()
{
    switch(currentSubstate)
    {
    case WLY_INTROSUBSTATE_NYU:
        if (isRedrawRequired){
            SplitCanvas_BeginTextureMode();
                DrawNYUSubscreen();
            SplitCanvas_EndTextureMode();
            isRedrawRequired = false;
        }
        SplitCanvas_DrawMain();
        break;
        
    case WLY_INTROSUBSTATE_Raylib:
        if (isRedrawRequired){
            SplitCanvas_BeginTextureMode();
                DrawRaylibSubscreen();
            SplitCanvas_EndTextureMode();
            isRedrawRequired = false;
        }
        SplitCanvas_DrawAll();
        break;
        
    case WLY_INTROSUBSTATE_TrollcoreEnterprises:
        if (isRedrawRequired){
            SplitCanvas_BeginTextureMode();
                DrawTrollcoreEnterprisesSubscreen();
            SplitCanvas_EndTextureMode();
            isRedrawRequired = false;
        }
        SplitCanvas_DrawAll();
        break;
    }
}

static void AddTimeSinceLastFrameToSubstateTimer(){
    substateTimer += GetFrameTime();
}

static void ChangeSubstateTo(WLY_INTROSUBSTATE newSubstate){
    currentSubstate = newSubstate;
    isRedrawRequired = true;
    isAdvancePromptDrawn = false;
    ResetSubstateTimer();
}

static void DrawAdvancePrompt(){
    splitslib_sf_DrawString(
        stringbox_Next.id, 
        splitslib_d_screen.width * 0.8, stringbox_Next.pos.y);
}

static void DrawNYUIncubatorLogo(){
    DrawTexture(texture_NYUIncubatorLogo,
        splitslib_d_screen.center.x - texture_NYUIncubatorLogo.width / 2,
        splitslib_d_screen.center.y - texture_NYUIncubatorLogo.height / 2,
        WHITE);
}

static void DrawNYULogoLabel(){
    splitslib_sf_DrawString(
        stringbox_NYU.id, 
        stringbox_NYU.pos.x, stringbox_NYU.pos.y);
}

static void DrawNYUSubscreen(){
    DrawNYUIncubatorLogo();
    DrawNYULogoLabel();
    if (substateTimer > WLY_NotificationTime_NYU)
        DrawAdvancePrompt(); 
}

static void DrawRaylibLogo(){
    DrawTexture(texture_RaylibLogo,
        splitslib_d_screen.center.x - texture_RaylibLogo.width / 2,
        splitslib_d_screen.center.y - texture_RaylibLogo.height / 2,
        WHITE);   
}

static void DrawRaylibLogoLabel(){
    splitslib_sf_DrawString(
        stringbox_Raylib.id, 
        stringbox_Raylib.pos.x, stringbox_Raylib.pos.y);
}

static void DrawRaylibSubscreen(){
    DrawRaylibLogo();
    DrawRaylibLogoLabel();
    if (substateTimer > WLY_NotificationTime_Raylib)
        DrawAdvancePrompt();
}

static void DrawTrollcoreEnterprisesSubscreen(){
    splitslib_sf_DrawString(
        stringbox_TETM1.id,
        stringbox_TETM1.pos.x, stringbox_TETM1.pos.y);
    splitslib_sf_DrawString(
        stringbox_TETM2.id,
        stringbox_TETM2.pos.x, stringbox_TETM2.pos.y);
    splitslib_sf_DrawString(
        stringbox_TETM3.id,
        stringbox_TETM3.pos.x, stringbox_TETM3.pos.y);
    splitslib_sf_DrawString(
        stringbox_TETM4.id,
        stringbox_TETM4.pos.x, stringbox_TETM4.pos.y);
    if (substateTimer > WLY_NotificationTime_TETM)
        DrawAdvancePrompt();
}

static void FreeStrings(){
    splitslib_sf_FreeString(stringbox_NYU.id);
    splitslib_sf_FreeString(stringbox_Raylib.id);
    splitslib_sf_FreeString(stringbox_TETM1.id);
    splitslib_sf_FreeString(stringbox_TETM2.id);
    splitslib_sf_FreeString(stringbox_TETM3.id);
    splitslib_sf_FreeString(stringbox_TETM4.id);
    splitslib_sf_FreeString(stringbox_Next.id);
}

static void InitStrings()
{
    splitslib_sf_InitStringbox(
        &stringbox_NYU, 
        WLY_NYUText, 
        WLY_NYUTextSize, 
        WLY_NYUTextWeight,
        WLY_NYUTextKern,
        WLY_NYUTextY);
        
    splitslib_sf_InitStringbox(
        &stringbox_Raylib,
        WLY_RaylibText,
        WLY_RaylibTextSize,
        WLY_RaylibTextWeight,
        WLY_RaylibTextKern,
        WLY_RaylibTextY);
        
    splitslib_sf_InitStringbox(
        &stringbox_TETM1,
        WLY_TETMText1,
        WLY_TETMText1Size,
        WLY_TETMText1Weight,
        WLY_TETMText1Kern,
        WLY_TETMText1Y);
    splitslib_sf_InitStringbox(
        &stringbox_TETM2,
        WLY_TETMText2,
        WLY_TETMText2Size,
        WLY_TETMText2Weight,
        WLY_TETMText2Kern,
        WLY_TETMText2Y);
    splitslib_sf_InitStringbox(
        &stringbox_TETM3,
        WLY_TETMText3,
        WLY_TETMText3Size,
        WLY_TETMText3Weight,
        WLY_TETMText3Kern,
        WLY_TETMText3Y);
    splitslib_sf_InitStringbox(
        &stringbox_TETM4,
        WLY_TETMText4,
        WLY_TETMText4Size,
        WLY_TETMText4Weight,
        WLY_TETMText4Kern,
        WLY_TETMText4Y);
        
    splitslib_sf_InitStringbox(
        &stringbox_Next,
        WLY_NextText,
        WLY_NextTextSize,
        WLY_NextTextWeight,
        WLY_NextTextKern,
        WLY_NextTextY);
}


static void LoadTextures(){
    Image image = LoadImage(WLY_Filepath_NYUIncubatorLogo);
    texture_NYUIncubatorLogo = LoadTextureFromImage(image);
    UnloadImage(image);
    
    image = LoadImage(WLY_Filepath_RaylibLogo);
    texture_RaylibLogo = LoadTextureFromImage(image);
    UnloadImage(image);
}

static void ResetSubstateTimer(){
    substateTimer = 0.0f;
}

static bool TimeIsUpOrPlayerHasPressedNext(float maxTime, float minTime){
    return (
        (substateTimer > maxTime) ||
        (substateTimer > minTime && splitslib_si_IsNextPressed()));
}

static void UnloadTextures(){
    UnloadTexture(texture_NYUIncubatorLogo);
    UnloadTexture(texture_RaylibLogo);
}

static bool WeAreFinishedWithThisSubstate(){
    switch(currentSubstate){
    case WLY_INTROSUBSTATE_NYU:
        return (TimeIsUpOrPlayerHasPressedNext(
            WLY_MaxTime_NYU, WLY_MinTime_NYU));
    case WLY_INTROSUBSTATE_Raylib:
        return (TimeIsUpOrPlayerHasPressedNext(
            WLY_MaxTime_Raylib, WLY_MinTime_Raylib));
    case WLY_INTROSUBSTATE_TrollcoreEnterprises:
        return (TimeIsUpOrPlayerHasPressedNext(
            WLY_MaxTime_TETM,
            WLY_MinTime_TETM));
    default:
        return false;
    }
}