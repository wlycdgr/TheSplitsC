#include "raylib.h"
#include "../../splitslib/splitslib_a_Audio.h"
#include "../../splitslib/splitslib_d_Display.h"
#include "../../splitslib/splitslib_sf_Splitsfont.h"
#include "../../splitslib/splitslib_si_SemanticInput.h"
#include "../../splitslib/splitslib_ui_UserInterface.h"
#include "GameSettings.h"
#include "HighScoreManager.h"
#include "ModeSelect.h"
#include "SplitCanvas.h"
#include "StateManager.h"
        
#define WLY_SelectedModeHighScoreStringboxStartText "0"
#define WLY_SelectedModeHighScoreStringboxTextSize 0.25
#define WLY_SelectedModeHighScoreStringboxTextWeight 0.075
#define WLY_SelectedModeHighScoreStringboxTextKern 0.1
#define WLY_SelectedModeHighScoreStringboxTextY 0.0

#define WLY_Menu_ModeSelect_X 225.0
#define WLY_Menu_ModeSelect_Hints_FontWeight 0.015
#define WLY_Menu_ModeSelect_Hints_X 250.0
#define WLY_Top_Y 50.0

#define WLY_ControlSchemeExplanation_TextSize 0.025
#define WLY_ControlSchemeExplanation_TextWeight 0.0025
#define WLY_ControlSchemeExplanation_TextKern 0.005
#define WLY_ControlSchemeExplanation_Y 0.1f

static void ActOnMenuSelection();
static void AddControlSchemesOptionList();
static void AddGameModesOptionList();
static void AddNumberOfAvatarsOptionList();
static void AddNumberOfPlaygridsOptionList();
static void AddPlaygridFlipOptionList();
static void AddPlaygridMovementOptionList();
static void DrawMargins();
static void DrawMenuStrings();
static void FreeMenus();
static void FreeStrings();
static void InitMenu_ModeSelect();
static void InitMenuStrings();
static void InitSelectedModeStrings();
static void UpdateMenuStrings();
static void UpdateScoreMultiplier();
static void UpdateSelectedModeStrings();
static void UpdateSettings();

static splitslib_sf_Stringbox stringbox_ControlSchemeHint;
static splitslib_sf_Stringbox stringbox_ControlSchemeKeys;
static splitslib_sf_Stringbox stringbox_GameModeHint;
static splitslib_sf_Stringbox stringbox_ScoreMultiplierInfo;
static splitslib_sf_Stringbox stringbox_SelectedModeHighScore;

char *strings_ControlSchemeHints[4] = {
    "WHEN YOU ARE RIGHT YOU ARE RIGHT",
    "STRONG AND STABLE",
    "WHO NEEDS A MOUSE?",
    "DISTANCE IS AN ILLUSION"
};
char *strings_ControlSchemeKeys[4] = {
    "USE QWERT AND YUIOP",
    "USE QWERT AND YUIOP",
    "USE G AND H AND Q",
    "USE QWERTYUIOP"
};
char *strings_GameModeHints[3] = {
    "COLLECT ENERGY AND AVOID ENEMIES",
    "COLLECT ENERGY",
    "AVOID ENEMIES"
};

static splitslib_ui_Menu menu_ModeSelect;
static int menuItemId_ControlSchemesOptionList;
static int menuItemId_GameModesOptionList;
static int menuItemId_NumberOfAvatarsOptionList;
static int menuItemId_NumberOfPlaygridsOptionList;
static int menuItemId_PlaygridMovementOptionList;
static int menuItemId_PlaygridFlipOptionList;
static int menuItemId_GameSpeedOptionList;
static int menuItemId_PlayLink;
static int menuItemId_ExitToTitleLink;
static int menuItemId_ExitToDesktopLink;

wly_GameSettings settings;

void wly_ModeSelect_Init(){
    InitMenu_ModeSelect();
    UpdateScoreMultiplier();
    InitMenuStrings();
    InitSelectedModeStrings();
}

void wly_ModeSelect_Update(){
    if (SplitCanvas_IsTransitioning()){
        SplitCanvas_ContinueTransition();
    }
    else {
        splitslib_ui_UpdateMenu(&menu_ModeSelect);
        UpdateSettings();
        UpdateScoreMultiplier();
        UpdateMenuStrings();
        UpdateSelectedModeStrings();
        if (splitslib_si_IsMenuSelectPressed())
            ActOnMenuSelection();
    }
}

void wly_ModeSelect_Draw(){
    SplitCanvas_BeginTextureMode();
        splitslib_ui_DrawMenu(
            &menu_ModeSelect,
            false
        );
        DrawMenuStrings();
        splitslib_sf_DrawString(
            stringbox_SelectedModeHighScore.id,
            stringbox_SelectedModeHighScore.pos.x,
            stringbox_SelectedModeHighScore.pos.y
        );
        //DrawMargins();
    SplitCanvas_EndTextureMode();
    SplitCanvas_DrawAll();
}

static void DrawMargins(){
    
    // top
    DrawLine(
        0, splitslib_d_screen.height * 0.05,
        splitslib_d_screen.width, splitslib_d_screen.height * 0.05,
        RED
    );    
    // bottom
    DrawLine(
        0, splitslib_d_screen.height * 0.95,
        splitslib_d_screen.width, splitslib_d_screen.height * 0.95,
        RED
    );
    
    // left
    DrawLine(
        splitslib_d_screen.width * 0.05, 0, 
        splitslib_d_screen.width * 0.05, splitslib_d_screen.height,
        RED
    );
    // right
    DrawLine(
        splitslib_d_screen.width * 0.95, 0, 
        splitslib_d_screen.width * 0.95, splitslib_d_screen.height,
        RED
    );
    
    // center line horizontal
    DrawRectangle(
        0, splitslib_d_screen.center.y - 1,
        splitslib_d_screen.width, 3,
        RED
    );
    // center line vertical
    DrawRectangle(
        splitslib_d_screen.center.x - 1, 0,
        3, splitslib_d_screen.height,
        RED
    );

    // center left
    DrawLine(
        splitslib_d_screen.width * 0.45, 0, 
        splitslib_d_screen.width * 0.45, splitslib_d_screen.height,
        RED
    );
    // center right
    DrawLine(
        splitslib_d_screen.width * 0.55, 0, 
        splitslib_d_screen.width * 0.55, splitslib_d_screen.height,
        RED
    );
}

static void ActOnMenuSelection(){
    if (menuItemId_PlayLink == menu_ModeSelect.cursor){
        // store information that will allow the gameplay code
        // to look up and update the right high score
        wly_highScoreIndices[0] = splitslib_ui_GetSelectedOptionListKey(
            &menu_ModeSelect,
            menuItemId_ControlSchemesOptionList
        );
        wly_highScoreIndices[1] = splitslib_ui_GetSelectedOptionListKey(
            &menu_ModeSelect,
            menuItemId_GameModesOptionList
        );
        // info that will allow the gameplay code to activate the right
        // combination of modes
        UpdateSettings();      
        wly_SetGameplayState(settings);       
    }
    else if (menuItemId_ExitToTitleLink == menu_ModeSelect.cursor){
        wly_SetState(WLY_STATE_Title);
    }
    else if (menuItemId_ExitToDesktopLink == menu_ModeSelect.cursor){
        wly_SetState(WLY_STATE_EXIT);
    }
    
    if (
        menuItemId_PlayLink == menu_ModeSelect.cursor ||
        menuItemId_ExitToTitleLink == menu_ModeSelect.cursor ||
        menuItemId_ExitToDesktopLink == menu_ModeSelect.cursor
    ){
        splitslib_a_PlayCommonSound(SPLITSLIB_SOUND_MenuSelect);
        FreeMenus();
        FreeStrings();
        SplitCanvas_StartTransition(WLY_WINDOW_WIDTH / 2.0);
    }
}

static void AddControlSchemesOptionList(){
    menuItemId_ControlSchemesOptionList =
        splitslib_ui_AddOptionList(
            &menu_ModeSelect,
            "CONTROL SCHEME",
            (Vector2){
                WLY_Menu_ModeSelect_X,
                WLY_Top_Y
            }
        );
        
    char *labels[4] = {"CLASSIC", "FIXED", "WEB FORM", "COLUMN SWAP"};
    for (int i = 0; i < 4; i++){
        splitslib_ui_AddOptionListOption(
            &menu_ModeSelect,
            menuItemId_ControlSchemesOptionList,
            labels[i], i
        );
    }
}

static void AddGameModesOptionList(){
    menuItemId_GameModesOptionList = 
        splitslib_ui_AddOptionList(
            &menu_ModeSelect,
            "GAME MODE",
            (Vector2){
                WLY_Menu_ModeSelect_X,
                150.0
            }
        );
        
    char *labels[3] = {"CLASSIC", "PEACEFUL", "STRESSFUL"};
    for (int i = 0; i < 3; i++){
        splitslib_ui_AddOptionListOption(
            &menu_ModeSelect,
            menuItemId_GameModesOptionList,
            labels[i], i
        );
    }
}

static void AddGameSpeedOptionList(){
    menuItemId_GameSpeedOptionList = 
        splitslib_ui_AddOptionList(
            &menu_ModeSelect,
            "GAME SPEED",
            (Vector2){
                WLY_Menu_ModeSelect_X,
                325.0
            }
        );
    
    char *labels[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    for (int i = 1; i < 10; i++){
        splitslib_ui_AddOptionListOption(
            &menu_ModeSelect,
            menuItemId_GameSpeedOptionList,
            labels[i], i
        );
    }
}

static void AddNumberOfAvatarsOptionList(){
    menuItemId_NumberOfAvatarsOptionList = 
        splitslib_ui_AddOptionList(
            &menu_ModeSelect,
            "AVATARS",
            (Vector2){
                WLY_Menu_ModeSelect_X,
                225.0
            }
        );
    
    char *labels[4] = {"2", "3", "4", "5"};
    for (int i = 0; i < 4; i++){
        splitslib_ui_AddOptionListOption(
            &menu_ModeSelect,
            menuItemId_NumberOfAvatarsOptionList,
            labels[i], i+2
        );
    }
}

static void AddNumberOfPlaygridsOptionList(){
    menuItemId_NumberOfPlaygridsOptionList = 
        splitslib_ui_AddOptionList(
            &menu_ModeSelect,
            "PLAYGRIDS",
            (Vector2){
                WLY_Menu_ModeSelect_X,
                250.0
            }
        );
    
    char *labels[4] = {"1", "2", "3", "4"};
    for (int i = 0; i < 4; i++){
        splitslib_ui_AddOptionListOption(
            &menu_ModeSelect,
            menuItemId_NumberOfPlaygridsOptionList,
            labels[i], i+1
        );
    }
}

static void AddPlaygridFlipOptionList(){
    menuItemId_PlaygridFlipOptionList = 
        splitslib_ui_AddOptionList(
            &menu_ModeSelect,
            "GRID FLIPPING",
            (Vector2){
                WLY_Menu_ModeSelect_X,
                300.0
            }
        );
    splitslib_ui_AddOptionListOption(
        &menu_ModeSelect,
        menuItemId_PlaygridFlipOptionList,
        "OFF", 0
    );
    splitslib_ui_AddOptionListOption(
        &menu_ModeSelect,
        menuItemId_PlaygridFlipOptionList,
        "ON", 1
    ); 
}

static void AddPlaygridMovementOptionList(){
    menuItemId_PlaygridMovementOptionList = 
        splitslib_ui_AddOptionList(
            &menu_ModeSelect,
            "GRID MOVEMENT",
            (Vector2){
                WLY_Menu_ModeSelect_X,
                275.0
            }
        );
    splitslib_ui_AddOptionListOption(
        &menu_ModeSelect,
        menuItemId_PlaygridMovementOptionList,
        "OFF", 0
    );
    splitslib_ui_AddOptionListOption(
        &menu_ModeSelect,
        menuItemId_PlaygridMovementOptionList,
        "ON", 1
    );
}

static void DrawMenuStrings(){
    splitslib_sf_DrawString(
        stringbox_ControlSchemeKeys.id,
        stringbox_ControlSchemeKeys.pos.x,
        stringbox_ControlSchemeKeys.pos.y
    );
    splitslib_sf_DrawString(
        stringbox_ControlSchemeHint.id,
        stringbox_ControlSchemeHint.pos.x,
        stringbox_ControlSchemeHint.pos.y
    );
    
    splitslib_sf_DrawString(
        stringbox_GameModeHint.id,
        stringbox_GameModeHint.pos.x,
        stringbox_GameModeHint.pos.y
    );
    
    splitslib_sf_DrawString(
        stringbox_ScoreMultiplierInfo.id,
        stringbox_ScoreMultiplierInfo.pos.x,
        stringbox_ScoreMultiplierInfo.pos.y
    );
}

static void FreeMenus(){
    splitslib_ui_FreeMenu(&menu_ModeSelect);
}

static void FreeStrings(){
    splitslib_sf_FreeString(stringbox_ControlSchemeHint.id);
    splitslib_sf_FreeString(stringbox_ControlSchemeKeys.id);
    splitslib_sf_FreeString(stringbox_GameModeHint.id);
    splitslib_sf_FreeString(stringbox_ScoreMultiplierInfo.id);
    splitslib_sf_FreeString(stringbox_SelectedModeHighScore.id);
}

static void InitMenu_ModeSelect(){
    splitslib_ui_InitMenu(&menu_ModeSelect);
    AddControlSchemesOptionList();
    AddGameModesOptionList();
    AddNumberOfAvatarsOptionList();
    AddNumberOfPlaygridsOptionList();
    AddPlaygridMovementOptionList();
    AddPlaygridFlipOptionList();
    AddGameSpeedOptionList();
    
    menuItemId_PlayLink = 
        splitslib_ui_AddLink(
            &menu_ModeSelect,
            "PLAY",
            SPLITSLIB_UI_LABEL_ALIGNMENT_Right,
            (Vector2){
                WLY_Menu_ModeSelect_X,
                525.0
            }
        );
    menuItemId_ExitToTitleLink =
        splitslib_ui_AddLink(
            &menu_ModeSelect,
            "EXIT TO TITLE",
            SPLITSLIB_UI_LABEL_ALIGNMENT_Right,
            (Vector2){
                WLY_Menu_ModeSelect_X,
                550.0
            }
        );
    menuItemId_ExitToDesktopLink =
        splitslib_ui_AddLink(
            &menu_ModeSelect,
            "EXIT TO DESKTOP",
            SPLITSLIB_UI_LABEL_ALIGNMENT_Right,
            (Vector2){
                WLY_Menu_ModeSelect_X,
                575.0
            }
        );
}

static void InitMenuStrings(){
    splitslib_sf_InitStringbox(
        &stringbox_ControlSchemeHint,
        strings_ControlSchemeHints[0],
        WLY_Menu_ModeSelect_Hints_FontWeight,
        0.0025,
        0.005,
        0.0
    );
    splitslib_sf_SetStringboxPosition(
        &stringbox_ControlSchemeHint,
        (Vector2){WLY_Menu_ModeSelect_Hints_X, WLY_Top_Y + 25.0}
    );
    splitslib_sf_InitStringbox(
        &stringbox_ControlSchemeKeys,
        strings_ControlSchemeKeys[0],
        WLY_Menu_ModeSelect_Hints_FontWeight,
        0.0025,
        0.005,
        0.0
    );
    splitslib_sf_SetStringboxPosition(
        &stringbox_ControlSchemeKeys,
        (Vector2){WLY_Menu_ModeSelect_Hints_X, WLY_Top_Y + 50.0}
    );
    
    splitslib_sf_InitStringbox(
        &stringbox_GameModeHint,
        strings_GameModeHints[0],
        WLY_Menu_ModeSelect_Hints_FontWeight,
        0.0025,
        0.005,
        0
    );
    splitslib_sf_SetStringboxPosition(
        &stringbox_GameModeHint,
        (Vector2){WLY_Menu_ModeSelect_Hints_X, 175.0}
    );
    
    splitslib_sf_InitStringbox(
        &stringbox_ScoreMultiplierInfo,
        FormatText("SCORE MULTIPLIER %d", scoreboard.scoreMultiplier),
        WLY_Menu_ModeSelect_Hints_FontWeight,
        0.0025,
        0.005,
        0
    );
    splitslib_sf_SetStringboxPosition(
        &stringbox_ScoreMultiplierInfo,
        (Vector2){WLY_Menu_ModeSelect_Hints_X, 350.0}
    );
}

static void InitSelectedModeStrings(){
    splitslib_sf_InitStringbox(
        &stringbox_SelectedModeHighScore,
        WLY_SelectedModeHighScoreStringboxStartText,
        WLY_SelectedModeHighScoreStringboxTextSize,
        WLY_SelectedModeHighScoreStringboxTextWeight,
        WLY_SelectedModeHighScoreStringboxTextKern,
        WLY_SelectedModeHighScoreStringboxTextY
    );
    splitslib_sf_SetStringboxPosition(
        &stringbox_SelectedModeHighScore,
        (Vector2){800.0, 275.0}
    );
}

static void UpdateMenuStrings(){
    splitslib_sf_SetText(
        stringbox_ControlSchemeHint.id,
        strings_ControlSchemeHints[
            splitslib_ui_GetSelectedOptionListKey(
                &menu_ModeSelect, menuItemId_ControlSchemesOptionList
            )
        ]
    );
    splitslib_sf_SetText(
        stringbox_ControlSchemeKeys.id,
        strings_ControlSchemeKeys[
            splitslib_ui_GetSelectedOptionListKey(
                &menu_ModeSelect, menuItemId_ControlSchemesOptionList
            )
        ]
    );
    
    splitslib_sf_SetText(
        stringbox_GameModeHint.id,
        strings_GameModeHints[
            splitslib_ui_GetSelectedOptionListKey(
                &menu_ModeSelect, menuItemId_GameModesOptionList
            )
        ]
    );
    
    splitslib_sf_SetText(
        stringbox_ScoreMultiplierInfo.id,
        FormatText("SCORE MULTIPLIER %d", scoreboard.scoreMultiplier)
    );
}

static void UpdateScoreMultiplier(){
    int moveBonus = settings.shouldPlaygridsMove ? 
        settings.playgridCount : 0;
    int flipBonus = settings.shouldPlaygridsFlip ?
        settings.playgridCount : 0;

    scoreboard.scoreMultiplier =
        (settings.avatarCount * settings.playgridCount) +
        moveBonus +
        flipBonus + 
        settings.gameSpeed;
}

static void UpdateSelectedModeStrings(){
    int controlSchemeKey = splitslib_ui_GetSelectedOptionListKey(
        &menu_ModeSelect, menuItemId_ControlSchemesOptionList
    );
    int gameModeKey = splitslib_ui_GetSelectedOptionListKey(
        &menu_ModeSelect, menuItemId_GameModesOptionList
    );

    splitslib_sf_SetText(
        stringbox_SelectedModeHighScore.id,
        FormatText(
            "%d", 
            wly_highScores
                [controlSchemeKey]
                [gameModeKey]
        )
    );
}

static void UpdateSettings(){
    settings.controlScheme = splitslib_ui_GetSelectedOptionListValue(
        &menu_ModeSelect,
        menuItemId_ControlSchemesOptionList
    );
    settings.mode = splitslib_ui_GetSelectedOptionListValue(
        &menu_ModeSelect,
        menuItemId_GameModesOptionList
    );
    settings.avatarCount = splitslib_ui_GetSelectedOptionListValue(
        &menu_ModeSelect,
        menuItemId_NumberOfAvatarsOptionList
    );
    settings.playgridCount = splitslib_ui_GetSelectedOptionListValue(
        &menu_ModeSelect,
        menuItemId_NumberOfPlaygridsOptionList
    );
    settings.shouldPlaygridsMove = 
        (1 == splitslib_ui_GetSelectedOptionListValue(
            &menu_ModeSelect,
            menuItemId_PlaygridMovementOptionList
        )) ? true : false;
    settings.shouldPlaygridsFlip =
        (1 == splitslib_ui_GetSelectedOptionListValue(
            &menu_ModeSelect,
            menuItemId_PlaygridFlipOptionList
        )) ? true : false;
    settings.gameSpeed = splitslib_ui_GetSelectedOptionListValue(
        &menu_ModeSelect,
        menuItemId_GameSpeedOptionList
    );
}