#include <stdio.h>
#include <stdlib.h>
#include "../../splitslib/splitslib_a_Audio.h"
#include "../../splitslib/splitslib_d_Display.h"
#include "../../splitslib/splitslib_si_SemanticInput.h"
#include "../../splitslib/splitslib_sf_Splitsfont.h"
#include "../../splitslib/splitslib_ui_UserInterface.h"
#include "../../splitslib/splitslib_util_Utilities.h"
#include "Options.h"
#include "SplitCanvas.h"
#include "StateManager.h"

#define WLY_MenuItem_SFXVolume_Text "SFX VOLUME"
#define WLY_MenuItem_SFXVolume_Y 0.5f
#define WLY_MenuItem_SFXVolume_SliderY (WLY_MenuItem_SFXVolume_Y * splitslib_d_screen.height) + 8

#define WLY_MenuItem_MusicVolume_Text "MUSIC VOLUME" 
#define WLY_MenuItem_MusicVolume_Y 0.55f
#define WLY_MenuItem_MusicVolume_SliderY (WLY_MenuItem_MusicVolume_Y * splitslib_d_screen.height) + 8

#define WLY_MenuItem_SaveAndExit_Text "SAVE AND EXIT"
#define WLY_MenuItem_SaveAndExit_Y 0.9f

#define WLY_OptionsFilepath "../data/options.txt"

#define WLY_Menu_HeaderText "OPTIONS"
#define WLY_Menu_LabelText_SFXVolumeSlider "SFX VOLUME"
#define WLY_Menu_LabelText_MusicVolumeSlider "MUSIC VOLUME"
#define WLY_Menu_LabelText_SaveAndExitLink "SAVE AND EXIT"

typedef enum wly_OptionCodes {
    WLY_OptionCode_SFXVolume = 0,
    WLY_OptionCode_MusicVolume = 1
} wly_OptionCodes;

static void DoTheSelectedThing();
static void DrawMenu();
static void GetOptionValues();
static void InitMenu();
static void LoadOptions();
static void LoadOption(char *k, char *v);
static void SaveOptions();

static splitslib_ui_Menu menu_Options;
int menuItem_SFXVolumeSlider_Id;
int menuItem_MusicVolumeSlider_Id;
int menuItem_SaveAndExitLink_Id;

void wly_Options_Init()
{
    InitMenu();
}

void wly_Options_Update()
{
    if (SplitCanvas_IsTransitioning()){
        SplitCanvas_ContinueTransition();
    }
    else {
        splitslib_ui_UpdateMenu(&menu_Options);
        if (splitslib_si_IsMenuSelectPressed()){
            DoTheSelectedThing();
        }
    }
}

void wly_Options_Draw()
{
    SplitCanvas_BeginTextureMode();
        splitslib_ui_DrawMenu(&menu_Options, false);
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

static void DoTheSelectedThing(){
    if (
        menuItem_SaveAndExitLink_Id ==
        menu_Options.cursor
    ){
        splitslib_a_PlayCommonSound(SPLITSLIB_SOUND_MenuSelect);
        GetOptionValues();
        SaveOptions();
        splitslib_ui_FreeMenu(&menu_Options);
        wly_SetState(WLY_STATE_Title);
        SplitCanvas_StartTransition(WLY_WINDOW_WIDTH / 2.0);
    }
}

static void GetOptionValues(){
    wly_optionValues.musicVolume = 100 * splitslib_ui_GetSliderValue(
        &menu_Options,
        menuItem_MusicVolumeSlider_Id
    );
    
    wly_optionValues.sfxVolume = 100 * splitslib_ui_GetSliderValue(
        &menu_Options,
        menuItem_SFXVolumeSlider_Id
    );
}

static void InitMenu()
{
    splitslib_ui_InitMenu(&menu_Options);
    
    splitslib_ui_AddHeader(
        &menu_Options,
        WLY_Menu_HeaderText,
        SPLITSLIB_UI_LABEL_ALIGNMENT_Center,
        (Vector2){
            splitslib_d_screen.center.x,
            splitslib_d_screen.height * 0.2
        }
    );
    
    menuItem_SFXVolumeSlider_Id = splitslib_ui_AddSlider(
        &menu_Options, 
        WLY_Menu_LabelText_SFXVolumeSlider,
        wly_optionValues.sfxVolume,
        (Vector2){
            splitslib_d_screen.center.x,
            splitslib_d_screen.height * 0.5
        }
    );
    menuItem_MusicVolumeSlider_Id = splitslib_ui_AddSlider(
        &menu_Options,
        WLY_Menu_LabelText_MusicVolumeSlider,
        wly_optionValues.musicVolume,
        (Vector2){
            splitslib_d_screen.center.x,
            splitslib_d_screen.height * 0.55
        }
    );    
    menuItem_SaveAndExitLink_Id = splitslib_ui_AddLink(
        &menu_Options,
        WLY_Menu_LabelText_SaveAndExitLink,
        SPLITSLIB_UI_LABEL_ALIGNMENT_Center,
        (Vector2){
            splitslib_d_screen.center.x,
            splitslib_d_screen.height * 0.85
        }
    );
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