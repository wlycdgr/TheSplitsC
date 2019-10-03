#include "splitslib_a_Audio.h"
#include "splitslib_d_Display.h"
#include "splitslib_sf_Splitsfont.h"
#include "splitslib_si_SemanticInput.h"
#include "splitslib_ui_UserInterface.h"
#include "splitslib_util_Utilities.h"

static void AddStringbox(splitslib_ui_Menu *mPtr, char *label);
static void AdvanceMainScrollTimer(splitslib_ui_Menu *mPtr);
static void AdvanceOptionListScrollTimer(
splitslib_ui_MenuItem_OptionList *olPtr);
static void AdvanceSliderScrollTimer(splitslib_ui_Menu *mPtr);
static bool MainScrollTimerDinged(splitslib_ui_Menu *mPtr);
static bool OptionListScrollTimerDinged(
splitslib_ui_MenuItem_OptionList *olPtr);
static void ResetMainScrollTimer(splitslib_ui_Menu *mPtr);
static void ResetOptionListScrollTimer(splitslib_ui_MenuItem_OptionList *olPtr);
static void ResetSliderScrollTimer(splitslib_ui_Menu *mPtr);
static void ScrollMainOnce(splitslib_ui_Menu *mPtr);
static void ScrollOptionListOnce(splitslib_ui_MenuItem_OptionList *olPtr);
static void ScrollSliderOnce(splitslib_ui_Menu *mPtr);
static void SetAlignmentAdjustedLabelPosition(
splitslib_sf_Stringbox *sbPtr, SPLITSLIB_UI_LABEL_ALIGNMENT alignment, Vector2 pos);
static void SetMainScrollDirectionToDown(splitslib_ui_Menu *mPtr);
static void SetMainScrollDirectionToNone(splitslib_ui_Menu *mPtr);
static void SetMainScrollDirectionToUp(splitslib_ui_Menu *mPtr);
void SetMenuItemMainLabelPosition(
splitslib_ui_Menu *mPtr, int itemIndex, Vector2 pos);
static void SetOptionListScrollDirectionToLeft(
splitslib_ui_MenuItem_OptionList *olPtr);
static void SetOptionListScrollDirectionToNone(
splitslib_ui_MenuItem_OptionList *olPtr);
static void SetOptionListScrollDirectionToRight(
splitslib_ui_MenuItem_OptionList *olPtr);
static void SetSliderScrollDirectionToLeft(splitslib_ui_Menu *mPtr);
static void SetSliderScrollDirectionToNone(splitslib_ui_Menu *mPtr);
static void SetSliderScrollDirectionToRight(splitslib_ui_Menu *mPtr);
static bool SliderScrollTimerDinged(splitslib_ui_Menu *mPtr);
static bool ThereIsAMainScrollDirection(splitslib_ui_Menu *mPtr);
static bool ThereIsAnOptionListScrollDirection(
splitslib_ui_MenuItem_OptionList *olPtr);
static bool ThereIsASliderScrollDirection(splitslib_ui_Menu *mPtr);
static void UpdateLabelColors(splitslib_ui_Menu *mPtr);

static splitslib_sf_Stringbox stringbox_ReturnPrompt;

void splitslib_ui_AddHeader(
splitslib_ui_Menu *mPtr, char *label, 
SPLITSLIB_UI_LABEL_ALIGNMENT alignment, Vector2 pos){
    splitslib_sf_InitStringbox(
        &(mPtr->headerStringbox),
        label,
        WLY_Header_TextSize,
        WLY_Header_TextWeight,
        WLY_Header_TextKern,
        WLY_Header_Y
    );
    
    SetAlignmentAdjustedLabelPosition(
        &(mPtr->headerStringbox), alignment, pos
    );
    
    mPtr->hasHeader = true;
}

int splitslib_ui_AddLink(
splitslib_ui_Menu *mPtr, char *label, 
SPLITSLIB_UI_LABEL_ALIGNMENT alignment, Vector2 pos){
    if (mPtr->size >= WLY_Menu_MaxItems) return -1;
    
    AddStringbox(mPtr, label);
    
    SetAlignmentAdjustedLabelPosition(
        &(mPtr->itemMainStringboxes[mPtr->size]), alignment, pos
    );
    
    mPtr->itemTypes[mPtr->size] = SPLITSLIB_UI_MENU_ITEM_TYPE_Link;
    
    int index = mPtr->size;
    mPtr->size++;
    return index;
}

int splitslib_ui_AddOptionList(
splitslib_ui_Menu *mPtr, char *label, Vector2 pos){
    if (mPtr->size >= WLY_Menu_MaxItems) return -1;
    
    AddStringbox(mPtr, label);
    mPtr->itemTypes[mPtr->size] = SPLITSLIB_UI_MENU_ITEM_TYPE_OptionList;
    
    SetAlignmentAdjustedLabelPosition(
        &(mPtr->itemMainStringboxes[mPtr->size]), 
        SPLITSLIB_UI_LABEL_ALIGNMENT_Right, 
        pos
    );
    
    splitslib_ui_MenuItem_OptionList *olPtr =
        &(mPtr->itemOptionLists[mPtr->size]);
    olPtr->cursor = 0;
    olPtr->scrollDir = 0;
    olPtr->scrollTimer = 0.0f;
    olPtr->size = 0;
    for (int i = 0; i < WLY_OptionList_MaxItems; i++){
        olPtr->values[i] = 0;
    }
    
    int index = mPtr->size;
    mPtr->size++;
    return index;
}

void splitslib_ui_AddOptionListOption(
splitslib_ui_Menu *mPtr, int optionListId, char *label, int value){
    splitslib_ui_MenuItem_OptionList *olPtr =
        &(mPtr->itemOptionLists[optionListId]);
        
    if (olPtr->size >= WLY_OptionList_MaxItems) return;
    
    splitslib_sf_InitStringbox(
        &(olPtr->labels[olPtr->size]),
        label,
        WLY_OptionListItemLabel_TextSize,
        WLY_OptionListItemLabel_TextWeight,
        WLY_OptionListItemLabel_TextKern,
        WLY_OptionListItemLabel_Y
    );
    
    float yPos = mPtr->itemMainStringboxes[optionListId].pos.y;
    float xPos = mPtr->itemMainStringboxes[optionListId].pos.x;
    // We assume the main label is right-aligned
    splitslib_sf_Result result = splitslib_sf_GetWidth(
        mPtr->itemMainStringboxes[optionListId].id);
    xPos += result.floatValue;
    xPos += 24; // DIVIDER VALUE
    xPos += 12 * olPtr->size; // margin * number of labels to the left
    for (int i = 0; i < olPtr->size; i++){
        result = splitslib_sf_GetWidth(olPtr->labels[i].id);
        xPos += result.floatValue;
    }
    
    splitslib_sf_SetStringboxPosition(
        &(olPtr->labels[olPtr->size]),
        (Vector2){xPos, yPos}
    );
    
    olPtr->values[olPtr->size] = value;
    
    olPtr->size++;
}
    
int splitslib_ui_AddSlider(
splitslib_ui_Menu *mPtr, char *label, float value, Vector2 pos){
    if (mPtr->size >= WLY_Menu_MaxItems) return -1;
    
    AddStringbox(mPtr, label);
    
    SetAlignmentAdjustedLabelPosition(
        &(mPtr->itemMainStringboxes[mPtr->size]), 
        SPLITSLIB_UI_LABEL_ALIGNMENT_Right, 
        pos
    );
    
    mPtr->itemTypes[mPtr->size] = SPLITSLIB_UI_MENU_ITEM_TYPE_Slider;
    
    mPtr->itemSliderValues[mPtr->size] = value / 100;

    int index = mPtr->size;
    mPtr->size++;
    return index;
}

void splitslib_ui_DrawHeader(splitslib_sf_Stringbox *sbox){
    splitslib_sf_DrawString(
        sbox->id,
        sbox->pos.x, sbox->pos.y
    );
}

void splitslib_ui_DrawMenu(splitslib_ui_Menu *mPtr, bool isSelected){
    if (mPtr->hasHeader){
        if (isSelected){
            splitslib_sf_SetColor(
                mPtr->headerStringbox.id, 
                WLY_Header_Color_Selected
            );
        }
        else {
            splitslib_sf_SetColor(
                mPtr->headerStringbox.id,
                WLY_Header_Color_Unselected
            );
        }
        
        splitslib_sf_DrawString(
            mPtr->headerStringbox.id,
            mPtr->headerStringbox.pos.x,
            mPtr->headerStringbox.pos.y
        );
    }
    
    for (int i = 0; i < mPtr->size; i++){
        splitslib_sf_DrawString(
            mPtr->itemMainStringboxes[i].id,
            mPtr->itemMainStringboxes[i].pos.x,
            mPtr->itemMainStringboxes[i].pos.y
        );
        
        if (
            SPLITSLIB_UI_MENU_ITEM_TYPE_OptionList ==
            mPtr->itemTypes[i]
        ){
            splitslib_ui_MenuItem_OptionList *olPtr = 
                &(mPtr->itemOptionLists[i]);
            for (int j = 0; j < olPtr->size; j++){
                splitslib_sf_DrawString(
                    olPtr->labels[j].id,
                    olPtr->labels[j].pos.x,
                    olPtr->labels[j].pos.y
                );
            }
        }
        else if (
            SPLITSLIB_UI_MENU_ITEM_TYPE_Slider == 
            mPtr->itemTypes[i]
        ){
            DrawLine(
                WLY_MenuSlider_LeftEdge,
                mPtr->itemMainStringboxes[i].pos.y,
                WLY_MenuSlider_RightEdge,
                mPtr->itemMainStringboxes[i].pos.y,
                BLACK
            );
              
            DrawCircle(
                WLY_MenuSlider_LeftEdge + 
                WLY_MenuSlider_Width * mPtr->itemSliderValues[i],
                mPtr->itemMainStringboxes[i].pos.y,
                WLY_MenuSlider_IndicatorRadius,
                ORANGE
            );
                
        }
    }
}

void splitslib_ui_DrawReturnPrompt(){
    splitslib_sf_DrawString(
        stringbox_ReturnPrompt.id,
        splitslib_d_screen.width * WLY_ReturnPrompt_X, 
        stringbox_ReturnPrompt.pos.y
    );
}

void splitslib_ui_FreeMenu(splitslib_ui_Menu *mPtr){
    if (mPtr->hasHeader)
        splitslib_sf_FreeString(mPtr->headerStringbox.id);
    
    for (int i = 0; i < mPtr->size; i++){
        splitslib_sf_FreeString(mPtr->itemMainStringboxes[i].id);
        if (
            SPLITSLIB_UI_MENU_ITEM_TYPE_OptionList ==
            mPtr->itemTypes[i]
        ){
            splitslib_ui_MenuItem_OptionList *olPtr =
                &(mPtr->itemOptionLists[i]);
            for (int j = 0; j < olPtr->size; j++){
                splitslib_sf_FreeString(olPtr->labels[j].id);
            }
        }
    }
}

int splitslib_ui_GetSelectedOptionListKey(
splitslib_ui_Menu *mPtr, int olIndex)
{
    splitslib_ui_MenuItem_OptionList *olPtr =
        &(mPtr->itemOptionLists[olIndex]);
        
    return olPtr->cursor;
}

int splitslib_ui_GetSelectedOptionListValue(
splitslib_ui_Menu *mPtr, int olIndex)
{
    splitslib_ui_MenuItem_OptionList *olPtr =
        &(mPtr->itemOptionLists[olIndex]);
        
    return olPtr->values[olPtr->cursor];
}

float splitslib_ui_GetSliderValue
(splitslib_ui_Menu *mPtr, int sIndex)
{
    if (sIndex < 0 || sIndex > WLY_Menu_MaxItems - 1)
        return 0.0;
    
    if (
        SPLITSLIB_UI_MENU_ITEM_TYPE_Slider !=
        mPtr->itemTypes[sIndex]
    )
        return 0.0;
        
    return mPtr->itemSliderValues[sIndex];
}

void splitslib_ui_InitHeaderString
(splitslib_sf_Stringbox *sbox, char *text)
{
    splitslib_sf_InitStringbox(
        sbox,
        text,
        WLY_Header_TextSize,
        WLY_Header_TextWeight,
        WLY_Header_TextKern,
        WLY_Header_Y
    );
}

// TODO flesh this out
void splitslib_ui_InitMenu(splitslib_ui_Menu *mPtr){
    mPtr->cursor = 0;
    mPtr->hasHeader = false;
    mPtr->scrollDir_SliderItem = 0;
    mPtr->scrollDir_Main = 0;
    mPtr->scrollTimer_SliderItem = 0.0f;
    mPtr->scrollTimer_Main = 0.0f;
    mPtr->size = 0;
    
    for (int i = 0; i < WLY_Menu_MaxItems; i++){
        mPtr->itemSliderValues[i] = 0.0f;
        mPtr->itemTypes[i] = 
            SPLITSLIB_UI_MENU_ITEM_TYPE_Undefined;
    }
}

void splitslib_ui_InitReturnPrompt(){
    splitslib_sf_InitStringbox(
        &stringbox_ReturnPrompt,
        WLY_ReturnPrompt_Text,
        WLY_ReturnPrompt_TextSize,
        WLY_ReturnPrompt_TextWeight,
        WLY_ReturnPrompt_TextKern,
        WLY_ReturnPrompt_Y
    );
}

void splitslib_ui_UpdateMenu(splitslib_ui_Menu *mPtr){
    if (
        SPLITSLIB_UI_MENU_ITEM_TYPE_Slider == 
        mPtr->itemTypes[mPtr->cursor]
    ){
        if (splitslib_si_IsMenuIncreasePressed()){
            SetSliderScrollDirectionToRight(mPtr);
            ScrollSliderOnce(mPtr);
            ResetSliderScrollTimer(mPtr);
        }
        else if (splitslib_si_IsMenuDecreasePressed()){
            SetSliderScrollDirectionToLeft(mPtr);
            ScrollSliderOnce(mPtr);
            ResetSliderScrollTimer(mPtr);
        }
        
        if (
            splitslib_si_IsMenuIncreaseReleased() ||
            splitslib_si_IsMenuDecreaseReleased()
        ){
            SetSliderScrollDirectionToNone(mPtr);
            ResetSliderScrollTimer(mPtr);
        }
        
        if (ThereIsASliderScrollDirection(mPtr)){
            AdvanceSliderScrollTimer(mPtr);
            if (SliderScrollTimerDinged(mPtr)){
                ScrollSliderOnce(mPtr);
                ResetSliderScrollTimer(mPtr);
            }
        }
    }
    else if (
        SPLITSLIB_UI_MENU_ITEM_TYPE_OptionList == 
        mPtr->itemTypes[mPtr->cursor]
    ){
        splitslib_ui_MenuItem_OptionList *olPtr = 
            &(mPtr->itemOptionLists[mPtr->cursor]);
        
        if (splitslib_si_IsMenuIncreasePressed()){
            SetOptionListScrollDirectionToRight(olPtr); 
            ScrollOptionListOnce(olPtr); 
            ResetOptionListScrollTimer(olPtr); 
        }
        else if (splitslib_si_IsMenuDecreasePressed()){
            SetOptionListScrollDirectionToLeft(olPtr); 
            ScrollOptionListOnce(olPtr); 
            ResetOptionListScrollTimer(olPtr); 
        }
        
        if (
            splitslib_si_IsMenuIncreaseReleased() ||
            splitslib_si_IsMenuDecreaseReleased()
        ){
            SetOptionListScrollDirectionToNone(olPtr); 
            ResetOptionListScrollTimer(olPtr); 
        }
        
        if (ThereIsAnOptionListScrollDirection(olPtr)){ 
            AdvanceOptionListScrollTimer(olPtr); 
            if (OptionListScrollTimerDinged(olPtr)){ 
                ScrollOptionListOnce(olPtr);
                ResetOptionListScrollTimer(olPtr);
            }
        }
    }
    
    if (splitslib_si_IsMenuScrollDownPressed()){
        SetMainScrollDirectionToDown(mPtr);
        ScrollMainOnce(mPtr);
        ResetMainScrollTimer(mPtr);
    }
    else if (splitslib_si_IsMenuScrollUpPressed()){
        SetMainScrollDirectionToUp(mPtr);
        ScrollMainOnce(mPtr);
        ResetMainScrollTimer(mPtr);
    }
    
    if (
        splitslib_si_IsMenuScrollDownReleased() ||
        splitslib_si_IsMenuScrollUpReleased()
    ){
        SetMainScrollDirectionToNone(mPtr);
        ResetMainScrollTimer(mPtr);
    }

    if (ThereIsAMainScrollDirection(mPtr)){
        AdvanceMainScrollTimer(mPtr);
        if (MainScrollTimerDinged(mPtr)){
            ScrollMainOnce(mPtr);
            ResetMainScrollTimer(mPtr);
        }
    }
    
    UpdateLabelColors(mPtr);
}

static void AddStringbox(splitslib_ui_Menu *mPtr, char *label){
    splitslib_sf_InitStringbox(
        &(mPtr->itemMainStringboxes[mPtr->size]),
        label,
        WLY_MenuItem_TextSize,
        WLY_MenuItem_TextWeight,
        WLY_MenuItem_TextKern,
        WLY_MenuItem_Y
    );
}

static void AdvanceMainScrollTimer(splitslib_ui_Menu *mPtr){
    mPtr->scrollTimer_Main += GetFrameTime();
}

static void AdvanceOptionListScrollTimer(
splitslib_ui_MenuItem_OptionList *olPtr){
    olPtr->scrollTimer += GetFrameTime();
}

static void AdvanceSliderScrollTimer(splitslib_ui_Menu *mPtr){
    mPtr->scrollTimer_SliderItem += GetFrameTime();
}

static bool MainScrollTimerDinged(splitslib_ui_Menu *mPtr){
    return mPtr->scrollTimer_Main > WLY_MainScrollTimer_Delay;
}

static bool OptionListScrollTimerDinged(
splitslib_ui_MenuItem_OptionList *olPtr){
    return olPtr->scrollTimer >= WLY_OptionListScrollTimer_Delay;
}

static void ResetMainScrollTimer(splitslib_ui_Menu *mPtr){
    mPtr->scrollTimer_Main = 0.0f;
}

static void ResetOptionListScrollTimer(splitslib_ui_MenuItem_OptionList *olPtr){
    olPtr->scrollTimer = 0.0f;
}

static void ResetSliderScrollTimer(splitslib_ui_Menu *mPtr){
    mPtr->scrollTimer_SliderItem = 0.0f;
}

static void ScrollMainOnce(splitslib_ui_Menu *mPtr){
    splitslib_a_PlayCommonSound(SPLITSLIB_SOUND_MenuMainScroll);
    
    mPtr->cursor += mPtr->scrollDir_Main;
    mPtr->cursor = splitslib_util_Wrap(mPtr->cursor, 0, mPtr->size - 1);
}

static void ScrollOptionListOnce(splitslib_ui_MenuItem_OptionList *olPtr){
    splitslib_a_PlayCommonSound(SPLITSLIB_SOUND_MenuOptionScroll);
    
    olPtr->cursor += olPtr->scrollDir;
    olPtr->cursor = splitslib_util_Wrap(olPtr->cursor, 0, olPtr->size - 1);
}

static void ScrollSliderOnce(splitslib_ui_Menu *mPtr){
    float newSliderValue = splitslib_util_Clamp(
        mPtr->itemSliderValues[mPtr->cursor] + 
        (mPtr->scrollDir_SliderItem * 0.02),
        0.0,
        1.0
    );
    
    mPtr->itemSliderValues[mPtr->cursor] = newSliderValue;
}

static void SetAlignmentAdjustedLabelPosition(
splitslib_sf_Stringbox *sbPtr, SPLITSLIB_UI_LABEL_ALIGNMENT alignment, Vector2 pos){
    float x = pos.x;
    splitslib_sf_Result result;
    
    if (SPLITSLIB_UI_LABEL_ALIGNMENT_Center == alignment){
        result = splitslib_sf_GetWidth(sbPtr->id);
        x = pos.x - result.floatValue / 2.0;
    }
    else if (SPLITSLIB_UI_LABEL_ALIGNMENT_Right == alignment){
        result = splitslib_sf_GetWidth(sbPtr->id);
        x = pos.x - result.floatValue;
    }
    
    splitslib_sf_SetStringboxPosition(sbPtr, (Vector2){x, pos.y});
}

static void SetMainScrollDirectionToDown(splitslib_ui_Menu *mPtr){
    mPtr->scrollDir_Main = 1;
}

static void SetMainScrollDirectionToNone(splitslib_ui_Menu *mPtr){
    mPtr->scrollDir_Main = 0;
}

static void SetMainScrollDirectionToUp(splitslib_ui_Menu *mPtr){
    mPtr->scrollDir_Main = -1;
}

static void SetOptionListScrollDirectionToLeft(
splitslib_ui_MenuItem_OptionList *olPtr){
    olPtr->scrollDir = -1;
}

static void SetOptionListScrollDirectionToNone(
splitslib_ui_MenuItem_OptionList *olPtr){
    olPtr->scrollDir = 0;
}

static void SetOptionListScrollDirectionToRight(
splitslib_ui_MenuItem_OptionList *olPtr){
    olPtr->scrollDir  = 1;
}

static void SetSliderScrollDirectionToLeft(splitslib_ui_Menu *mPtr){
    mPtr->scrollDir_SliderItem = -1;
}

static void SetSliderScrollDirectionToNone(splitslib_ui_Menu *mPtr){
    mPtr->scrollDir_SliderItem = 0;
}

static void SetSliderScrollDirectionToRight(splitslib_ui_Menu *mPtr){
    mPtr->scrollDir_SliderItem = 1;
}

static bool SliderScrollTimerDinged(splitslib_ui_Menu *mPtr){
    return mPtr->scrollTimer_SliderItem >= WLY_SliderScrollTimer_Delay;
}

static bool ThereIsAMainScrollDirection(splitslib_ui_Menu *mPtr){
    return mPtr->scrollDir_Main != 0;
}

static bool ThereIsAnOptionListScrollDirection(
splitslib_ui_MenuItem_OptionList *olPtr){
    return 0 != olPtr->scrollDir;
}

static bool ThereIsASliderScrollDirection(splitslib_ui_Menu *mPtr){
    return 0 != mPtr->scrollDir_SliderItem;
}

static void UpdateLabelColors(splitslib_ui_Menu *mPtr){
    // main labels
    for (int i = 0; i < mPtr->size; i++){
        splitslib_sf_SetColor(
            mPtr->itemMainStringboxes[i].id, 
            WLY_MenuItem_UnselectedColor
        );
    }
    splitslib_sf_SetColor(
        mPtr->itemMainStringboxes[mPtr->cursor].id,
        WLY_MenuItem_SelectedColor
    );
    
    // option list item labels
    for (int i = 0; i < mPtr->size; i++){
        if (
            SPLITSLIB_UI_MENU_ITEM_TYPE_OptionList !=
            mPtr->itemTypes[i]
        ){
            continue;
        }
        
        splitslib_ui_MenuItem_OptionList *olPtr = 
            &(mPtr->itemOptionLists[i]);
            
        for (int j = 0; j < olPtr->size; j++){
            splitslib_sf_SetColor(
                olPtr->labels[j].id,
                WLY_OptionListItemLabel_UnselectedColor
            );
        }
      
        splitslib_sf_SetColor(
            olPtr->labels[olPtr->cursor].id,
            WLY_OptionListItemLabel_SelectedColor
        );
    }
}