#ifndef SPLITSLIB_UI_UserInterface_H
#define SPLITSLIB_UI_UserInterface_H

#include "splitslib_sf_Splitsfont.h"

#define WLY_Header_Color_Selected ORANGE
#define WLY_Header_Color_Unselected BLACK
#define WLY_Header_TextSize 0.025
#define WLY_Header_TextWeight 0.0025
#define WLY_Header_TextKern 0.005
#define WLY_Header_Y 0.1f

#define WLY_ReturnPrompt_Text "SPACEBAR TO RETURN"
#define WLY_ReturnPrompt_TextSize 0.01f
#define WLY_ReturnPrompt_TextWeight 0.0005f
#define WLY_ReturnPrompt_TextKern 0.01f
#define WLY_ReturnPrompt_Y 0.95f
#define WLY_ReturnPrompt_X 0.8f

#define WLY_MenuItem_TextSize 0.015
#define WLY_MenuItem_TextWeight 0.001
#define WLY_MenuItem_TextKern 0.004
#define WLY_MenuItem_Y 0.1f
#define WLY_MenuItem_UnselectedColor BLACK
#define WLY_MenuItem_SelectedColor ORANGE

#define WLY_OptionListItemLabel_SelectedColor ORANGE
#define WLY_OptionListItemLabel_TextSize 0.015 //0.02 //0.035 //0.0125
#define WLY_OptionListItemLabel_TextWeight 0.001 // 0.0015 //0.001
#define WLY_OptionListItemLabel_TextKern 0.004 //0.0075 //0.005
#define WLY_OptionListItemLabel_UnselectedColor BLACK
#define WLY_OptionListItemLabel_Y 0.1f

#define WLY_MenuSlider_Increment 1
#define WLY_MenuSlider_IndicatorRadius 5
#define WLY_MenuSlider_LeftEdge 660
#define WLY_MenuSlider_Min 0
#define WLY_MenuSlider_Max 100
#define WLY_MenuSlider_RightEdge WLY_MenuSlider_LeftEdge + WLY_MenuSlider_Width 
#define WLY_MenuSlider_Width 200

#define WLY_Menu_MaxItems 64
#define WLY_OptionList_MaxItems 10

#define WLY_MainScrollTimer_Delay 0.2f
#define WLY_OptionListScrollTimer_Delay 0.5F
#define WLY_SliderScrollTimer_Delay 0.1f

typedef enum SPLITSLIB_UI_LABEL_ALIGNMENT {
    SPLITSLIB_UI_LABEL_ALIGNMENT_Center = 0,
    SPLITSLIB_UI_LABEL_ALIGNMENT_Left,
    SPLITSLIB_UI_LABEL_ALIGNMENT_Right
} SPLITSLIB_UI_LABEL_ALIGNMENT;

typedef enum SPLITSLIB_UI_MENUALIGNMENT {
    SPLITSLIB_UI_MENUALIGNMENT_Horizontal = 0,
    SPLITSLIB_UI_MENUALIGNMENT_Vertical
} SPLITSLIB_UI_MENUALIGNMENT;

typedef enum SPLITSLIB_UI_MENU_ITEM_TYPE {
    SPLITSLIB_UI_MENU_ITEM_TYPE_Link = 0,
    SPLITSLIB_UI_MENU_ITEM_TYPE_OptionList,
    SPLITSLIB_UI_MENU_ITEM_TYPE_Slider,
    SPLITSLIB_UI_MENU_ITEM_TYPE_Undefined
} SPLITSLIB_UI_MENU_ITEM_TYPE;

typedef struct splitslib_ui_MenuItem_OptionList{
    int cursor;
    splitslib_sf_Stringbox labels[WLY_OptionList_MaxItems];
    float scrollDir;
    float scrollTimer;
    int size;
    int values[WLY_OptionList_MaxItems];
} splitslib_ui_MenuItem_OptionList;

typedef struct splitslib_ui_Menu {
    int cursor;
    bool hasHeader;
    splitslib_sf_Stringbox headerStringbox;
    splitslib_sf_Stringbox itemMainStringboxes[WLY_Menu_MaxItems];
    float itemSliderValues[WLY_Menu_MaxItems];
    splitslib_ui_MenuItem_OptionList itemOptionLists[WLY_Menu_MaxItems];
    SPLITSLIB_UI_MENU_ITEM_TYPE itemTypes[WLY_Menu_MaxItems];
    int scrollDir_SliderItem;
    int scrollDir_Main;
    float scrollTimer_SliderItem;
    float scrollTimer_Main;
    int size;
} splitslib_ui_Menu;

void splitslib_ui_AddHeader(
splitslib_ui_Menu *mPtr, char *label, 
SPLITSLIB_UI_LABEL_ALIGNMENT alignment, Vector2 pos);
int splitslib_ui_AddLink(
splitslib_ui_Menu *mPtr, char *label, 
SPLITSLIB_UI_LABEL_ALIGNMENT alignment, Vector2 pos);
int splitslib_ui_AddOptionList(
splitslib_ui_Menu *mPtr, char *label, Vector2 pos);
void splitslib_ui_AddOptionListOption(
splitslib_ui_Menu *mPtr, int optionListId, char *label, int value);
int splitslib_ui_AddSlider(
splitslib_ui_Menu *mPtr, char *label, float value, Vector2 pos);
void splitslib_ui_DrawHeader(splitslib_sf_Stringbox *sbox);
void splitslib_ui_DrawMenu(splitslib_ui_Menu *mPtr, bool isSelected);
void splitslib_ui_DrawReturnPrompt();
void splitslib_ui_FreeMenu(splitslib_ui_Menu *mPtr);
int splitslib_ui_GetSelectedOptionListKey(splitslib_ui_Menu *mPtr, int olIndex);
int splitslib_ui_GetSelectedOptionListValue(splitslib_ui_Menu *mPtr, int olIndex);
float splitslib_ui_GetSliderValue(splitslib_ui_Menu *mPtr, int sIndex);
void splitslib_ui_InitHeaderString(splitslib_sf_Stringbox *sbox, char *text);
void splitslib_ui_InitMenu(splitslib_ui_Menu *mPtr);
void splitslib_ui_InitReturnPrompt();
void splitslib_ui_UpdateMenu(splitslib_ui_Menu *mPtr);

#endif