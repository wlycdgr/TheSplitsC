#include <stdio.h>
#include <stdlib.h>
#include "../splitslib/splitslib_d_Display.h"
#include "../splitslib/splitslib_si_SemanticInput.h"
#include "../splitslib/splitslib_sf_Splitsfont.h"
#include "../splitslib/splitslib_ui_UserInterface.h"
#include "../splitslib/splitslib_util_Utilities.h"
#include "Options.h"
#include "SplitCanvas.h"
#include "StateManager.h"

#define WLY_Header_Text "OPTIONS"

#define WLY_MenuItem_SFXVolume_Text "SFX VOLUME"
#define WLY_MenuItem_SFXVolume_Y 0.5f
#define WLY_MenuItem_SFXVolume_SliderY (WLY_MenuItem_SFXVolume_Y * splitslib_d_screen.height) + 8

#define WLY_MenuItem_MusicVolume_Text "MUSIC VOLUME" 
#define WLY_MenuItem_MusicVolume_Y 0.55f
#define WLY_MenuItem_MusicVolume_SliderY (WLY_MenuItem_MusicVolume_Y * splitslib_d_screen.height) + 8

#define WLY_MenuItem_SaveAndExit_Text "SAVE AND EXIT"
#define WLY_MenuItem_SaveAndExit_Y 0.9f

#define WLY_OptionsFilepath "../files/options.txt"

typedef enum wly_OptionCodes {
    WLY_OptionCode_SFXVolume = 0,
    WLY_OptionCode_MusicVolume = 1
} wly_OptionCodes;

static void DecreaseSelectedValue();
static void DoTheSelectedThing();
static void DrawMenu();
static void DrawMenuItemMusicVolume();
static void DrawMenuItemSFXVolume();
static void FreeStrings();
static void IncreaseSelectedValue();
static void InitMenu();
static void LoadOptions();
static void LoadOption(char *k, char *v);
static void SaveOptions();

static splitslib_sf_Stringbox stringbox_Header;

static splitslib_ui_Menu menu;

static splitslib_sf_Stringbox stringbox_MenuItem_SFXVolume;
static splitslib_sf_Stringbox stringbox_MenuItem_MusicVolume;
static splitslib_sf_Stringbox stringbox_MenuItem_SaveAndExit;

void wly_Options_Init()
{
    splitslib_ui_InitHeaderString(&stringbox_Header, WLY_Header_Text);
    InitMenu();
}

void wly_Options_Update()
{
    if (SplitCanvas_IsTransitioning()){
        SplitCanvas_ContinueTransition();
    }
    else {
        splitslib_ui_ScrollMenu(&menu);
        if (splitslib_si_IsMenuSelectPressed()){
            DoTheSelectedThing();
        }
        
        if (splitslib_si_IsMenuIncreasePressed()){
            IncreaseSelectedValue();
        }
        
        if (splitslib_si_IsMenuDecreasePressed()){
            DecreaseSelectedValue();
        }
    }
}

void wly_Options_Draw()
{
    SplitCanvas_BeginTextureMode();
        splitslib_ui_DrawHeader(&stringbox_Header);
        DrawMenu();
    SplitCanvas_EndTextureMode();
    
    SplitCanvas_DrawAll();
}

void wly_LoadOptions()
{
    wly_optionValues.sfxVolume = WLY_DefaultSFXVolume;
    wly_optionValues.musicVolume = WLY_DefaultMusicVolume;
    
    FILE *fp;
    fp = fopen(WLY_OptionsFilepath, "r+");
    if (NULL == fp) {
        SaveOptions();
        return;
    }

    int maxL = 10;
    char key[maxL];
    char value[maxL];
    bool isKey = true;
    bool isValue = false;
    bool cursor = 0;
    char c;
    while (EOF != (c = fgetc(fp))){
        if ('\n' == c){
            LoadOption(key, value);
            for (int i =0; i < maxL; i++){
               key[i] = '\0';
               value[i] = '\0';
            } 
            isKey = true;
            isValue = false;
            cursor = 0;
        }
        else if (',' == c){
           isKey = false;
           isValue = true;
           cursor = 0;
        }
        else if (cursor >= maxL){
           continue;
        }
        else if (isKey){
           key[cursor] = c;
           cursor++;
        }
        else if (isValue){
           value[cursor] = c;
           cursor++;
        }
    }
   
    fclose(fp);
}

static void DecreaseSelectedValue(){
   switch(menu.cursor){
    case 0: // SFX VOLUME
        wly_optionValues.sfxVolume = splitslib_util_Clamp(
            wly_optionValues.sfxVolume - WLY_MenuSlider_Increment,
            WLY_MenuSlider_Min, WLY_MenuSlider_Max);
        break;
    case 1: // MUSIC VOLUME
        wly_optionValues.musicVolume = splitslib_util_Clamp(
            wly_optionValues.musicVolume - WLY_MenuSlider_Increment,
            WLY_MenuSlider_Min, WLY_MenuSlider_Max);
        break;
    } 
}

static void DoTheSelectedThing(){
    switch(menu.cursor){
    case 2: // SAVE AND EXIT 
        SaveOptions();
        FreeStrings();
        wly_SetState(WLY_STATE_Title);
        SplitCanvas_StartTransition(WLY_WINDOW_WIDTH / 2.0);
        break;
    }
}

static void DrawMenu(){
    DrawMenuItemSFXVolume();
    DrawMenuItemMusicVolume();
        
    splitslib_sf_DrawString(
        stringbox_MenuItem_SaveAndExit.id,
        stringbox_MenuItem_SaveAndExit.pos.x, stringbox_MenuItem_SaveAndExit.pos.y);
}

static void DrawMenuItemMusicVolume(){
    splitslib_sf_DrawString(
        stringbox_MenuItem_MusicVolume.id,
        stringbox_MenuItem_MusicVolume.pos.x, stringbox_MenuItem_MusicVolume.pos.y);
    
    DrawLine(
        WLY_MenuSlider_LeftEdge, WLY_MenuItem_MusicVolume_SliderY, 
        WLY_MenuSlider_RightEdge, WLY_MenuItem_MusicVolume_SliderY,
        BLACK);
        
    DrawCircle(
        WLY_MenuSlider_LeftEdge + (WLY_MenuSlider_Width * (wly_optionValues.musicVolume / 100.0)), 
        WLY_MenuItem_MusicVolume_SliderY,
        WLY_MenuSlider_IndicatorRadius, BLACK);
}

static void DrawMenuItemSFXVolume(){
    splitslib_sf_DrawString(
        stringbox_MenuItem_SFXVolume.id,
        stringbox_MenuItem_SFXVolume.pos.x, stringbox_MenuItem_SFXVolume.pos.y); 
        
    DrawLine(
        WLY_MenuSlider_LeftEdge, WLY_MenuItem_SFXVolume_SliderY, 
        WLY_MenuSlider_RightEdge, WLY_MenuItem_SFXVolume_SliderY,
        BLACK);
        
    DrawCircle(
        WLY_MenuSlider_LeftEdge + (WLY_MenuSlider_Width * (wly_optionValues.sfxVolume / 100.0)), 
        WLY_MenuItem_SFXVolume_SliderY,
        WLY_MenuSlider_IndicatorRadius, BLACK);
}

static void FreeStrings(){
    splitslib_sf_FreeString(stringbox_Header.id);
    
    splitslib_sf_FreeString(stringbox_MenuItem_SFXVolume.id);
    splitslib_sf_FreeString(stringbox_MenuItem_MusicVolume.id);
    splitslib_sf_FreeString(stringbox_MenuItem_SaveAndExit.id);
}

static void IncreaseSelectedValue(){
    switch(menu.cursor){
    case 0: // SFX VOLUME
        wly_optionValues.sfxVolume = splitslib_util_Clamp(
            wly_optionValues.sfxVolume + WLY_MenuSlider_Increment,
            WLY_MenuSlider_Min, WLY_MenuSlider_Max);
        break;
    case 1: // MUSIC VOLUME
        wly_optionValues.musicVolume = splitslib_util_Clamp(
            wly_optionValues.musicVolume + WLY_MenuSlider_Increment,
            WLY_MenuSlider_Min, WLY_MenuSlider_Max);
        break;
    }
}

static void InitMenu()
{
    splitslib_sf_Result result;
    
    splitslib_sf_InitStringbox(
        &stringbox_MenuItem_SFXVolume,
        WLY_MenuItem_SFXVolume_Text,
        WLY_MenuItem_TextSize,
        WLY_MenuItem_TextWeight,
        WLY_MenuItem_TextKern,
        WLY_MenuItem_SFXVolume_Y);
    result = splitslib_sf_GetWidth(stringbox_MenuItem_SFXVolume.id);
    stringbox_MenuItem_SFXVolume.pos.x = splitslib_d_screen.width * 0.485 - result.floatValue;
        
    splitslib_sf_InitStringbox(
        &stringbox_MenuItem_MusicVolume,
        WLY_MenuItem_MusicVolume_Text,
        WLY_MenuItem_TextSize,
        WLY_MenuItem_TextWeight,
        WLY_MenuItem_TextKern,
        WLY_MenuItem_MusicVolume_Y);
    result = splitslib_sf_GetWidth(stringbox_MenuItem_MusicVolume.id);
    stringbox_MenuItem_MusicVolume.pos.x = splitslib_d_screen.width * 0.485 - result.floatValue;
        
    splitslib_sf_InitStringbox(
        &stringbox_MenuItem_SaveAndExit,
        WLY_MenuItem_SaveAndExit_Text,
        WLY_MenuItem_TextSize,
        WLY_MenuItem_TextWeight,
        WLY_MenuItem_TextKern,
        WLY_MenuItem_SaveAndExit_Y);
        
    menu.cursor = 0;
    menu.items[0] = &stringbox_MenuItem_SFXVolume;
    menu.items[1] = &stringbox_MenuItem_MusicVolume;
    menu.items[2] = &stringbox_MenuItem_SaveAndExit;
    menu.itemCount = 3;
    menu.scrollDir = 0;
    
    splitslib_ui_ResetScrollTimer(&menu);
    splitslib_ui_UpdateMenuItemColors(&menu);
}

static void LoadOption(char *k, char *v)
{
    int val = atoi(v);
    switch(atoi(k)){
    case WLY_OptionCode_MusicVolume:
        wly_optionValues.musicVolume = splitslib_util_Clamp(val, 0, 100);
        break;
    case WLY_OptionCode_SFXVolume:
        wly_optionValues.sfxVolume = splitslib_util_Clamp(val, 0, 100);
        break;
    default:
        break;
    }
}

static void SaveOptions()
{
    FILE *fp;
    fp = fopen(WLY_OptionsFilepath, "w");
    if (NULL == fp) return; // TODO -- handle failure more loudly
    
    fprintf(fp, "%d,%d\n", WLY_OptionCode_SFXVolume, wly_optionValues.sfxVolume);
    fprintf(fp, "%d,%d\n", WLY_OptionCode_MusicVolume, wly_optionValues.musicVolume);
    
    fclose(fp); 
}