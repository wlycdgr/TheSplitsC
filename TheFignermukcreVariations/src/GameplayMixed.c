#include <stdlib.h>
#include "raylib.h"
#include "../../splitslib/splitslib_a_Audio.h"
#include "../../splitslib/splitslib_d_Display.h"
#include "../../splitslib/splitslib_ps_PulseSystem.h"
#include "../../splitslib/splitslib_sf_Splitsfont.h"
#include "../../splitslib/splitslib_si_SemanticInput.h"
#include "../../splitslib/splitslib_tw_Tween.h"
#include "../../splitslib/splitslib_ui_UserInterface.h"
#include "../../splitslib/splitslib_util_Utilities.h"
#include "GameplayMixed.h"
#include "Gameplay_PelletManager.h"
#include "GameSettings.h"
#include "HighScoreManager.h"
#include "ProjectAudio.h"
#include "SplitCanvas.h"
#include "StateManager.h"

#define WLY_Menu_GameOver_HeaderText "GAME OVER"
#define WLY_Menu_GameOver_LabelText_PlayAgain "PLAY AGAIN"
#define WLY_Menu_GameOver_LabelText_ExitToModeSelect "EXIT TO MODE SELECT"
#define WLY_Menu_GameOver_LabelText_ExitToTitle "EXIT TO TITLE"
#define WLY_Menu_GameOver_LabelText_ExitToDesktop "EXIT TO DESKTOP"

#define WLY_Menu_Pause_HeaderText "PAUSED"
#define WLY_Menu_Pause_LabelText_Continue "CONTINUE"
#define WLY_Menu_Pause_LabelText_ExitToModeSelect "EXIT TO MODE SELECT"
#define WLY_Menu_Pause_LabelText_ExitToTitle "EXIT TO TITLE"
#define WLY_Menu_Pause_LabelText_ExitToDesktop "EXIT TO DESKTOP"

#define WLY_MenuHeader_Y 40
#define WLY_MenuItem_1_Y 400
#define WLY_MenuItem_2_Y 440

#define WLY_MenuRibbon_BorderWidth 2
#define WLY_MenuRibbon_Bottom_Y 478
#define WLY_MenuRibbon_Top_Y 0

#define WLY_MENURIBBON_HEIGHT 480

static bool AnyAvatarHasJustBeenHitByAnEnemyPellet();
static bool AnyAvatarHasJustRunOutOfLife();
static void CleanUpAndExitTo(WLY_STATE newState);
static bool ColumnTargeted(int i, wly_Playgrid *playgrid);
static void ContinueAnimatingTheMove(wly_Playgrid *pgPtr);
static void DetectAndHandleCollisionsBetweenAvatarsAndPellets(wly_PelletManager *pmPtr, int playgridIndex);
static void DoTheSelectedGameOverMenuThing();
static void DoTheSelectedPauseMenuThing();
static void DrawAvatars(wly_Playgrid *pgPtr);
static void DrawDangerIndicators(wly_Playgrid *pgPtr);
static void DrawGameOverMenu();
static void DrawMenuRibbonBorders();
static void DrawPauseMenu();
static void DrawPlayfields();
static void DrawPlaygrid(wly_Playgrid *pgPtr, int pgIndex);
static void DrawScoreboard();
static void FinalizeSourceToTargetMove
(wly_Playgrid *pgPtr, wly_PlayerMove *mPtr);
static void FreeMenus();
static void FreeStrings();
static void HandleACollisionBetweenAnAvatarAndAPellet
(wly_PlayerAvatar *paPtr, wly_Pellet *pPtr, WLY_PELLET_TYPE type);
static void HandleMoveInput();
static void HandleSwitchActivePlaygridInput();
static void HandleSwitchPlaygridMoveDirectionInput();
static void InitBgAnimationSystem();
static void InitEnemyPellets();
static void InitEnergyPellets();
static void InitGameOverMenu();
static void InitGameSettings(wly_GameSettings settings);
static void InitGameSettings_SetAvatarAndColumnCounts(
int avatarCount, int a2, int c2, int a3, int c3, int a4, int c4, int a5, int c5);
static void InitGameSettings_SetSpeed(int speed);
static void InitInputMappings();
static void InitPauseMenu();
static void InitScoreboard();
static bool KillAnimationHasFinished();
static void LoadRenderTextures();
static void MovePlaygrid(wly_Playgrid *pgPtr);
static void NoteDownStartingColumnAndPosition(wly_Playgrid *playgrid);
static void PositionAvatars(wly_Playgrid *playgrid);
static void PositionColumns(wly_Playgrid *playgrid);
static void PositionGrid(wly_Playgrid *playgrid, int playgridIndex);
static void ResetAvatarIndexArrays(wly_Playgrid *pgPtr);
static void ResetAvatars(wly_Playgrid *playgrid);
static void ResetColumns(wly_Playgrid *playgrid);
static void ResetGame();
static void ResetMove(wly_Playgrid *playgrid);
static void ResetMoveTimer(wly_PlayerMove *move);
static void ResetPlaygrid(wly_Playgrid *pgPtr, int pgIndex);
static void ResetScoreboard();
static void RotatePlaygrid(wly_Playgrid *pgPtr);
static void SaveNewHighScoreIfApplicable();
static void SetTargetColumn(wly_Playgrid *pgPtr, int tcIndex);
static void SetupStartPosition(wly_Playgrid *playgrid);
static void TargetThisColumnToTheLeft(wly_Playgrid *playgrid);
static void TargetThisColumnToTheRight(wly_Playgrid *playgrid);
static bool TheAnimationIsDone(wly_Playgrid *pgPtr);
static bool ThereIsACollisionBetweenAnAvatarAndAPellet(wly_PlayerAvatar *paPtr, wly_Pellet *pPtr);
static bool ThereIsAnEmptyColumnToTheLeft(wly_Playgrid *playgrid);
static bool ThereIsAnEmptyColumnToTheRight(wly_Playgrid *playgrid);
static void TryToStartMovingLeft(wly_PlayerMove *movePtr, wly_Playgrid *pgPtr);
static void TryToStartMovingRight(wly_PlayerMove *movePtr, wly_Playgrid *pgPtr);
static void UnloadRenderTextures();
static void UpdateAvatar(wly_PlayerAvatar *paPtr);
static void UpdateAvatars(wly_Playgrid *pgPtr);
static void UpdateAvatarIndexArray_LeftToRight(wly_Playgrid *pgPtr);
static void UpdatePlaygrid(wly_Playgrid *pgPtr, int pgIndex);
static void UpdatePlaygrids();
static void UpdateScoreboardViewToGameplayMode();
static void UpdateScoreboardViewToMenuMode();
static void Utils_SwapSort(int *arr, int len, int *matchingArr);
static bool Utils_SwapSort_IsNotSorted(int *arr, int len);

wly_Playgrid playgrids[WLY_GAME_MaxPlaygrids];
wly_PelletManager energyPMs[WLY_GAME_MaxPlaygrids];
wly_PelletManager enemyPMs[WLY_GAME_MaxPlaygrids];
int activePlaygridIndex; 

float frameTime;

char input_MoveLeft[WLY_PLAYGRID_MaxAvatars];
char input_MoveRight[WLY_PLAYGRID_MaxAvatars];
char input_ColumnSwap_Columns[WLY_PLAYGRID_MaxColumns];
char input_WebForm_MoveLeft;
char input_WebForm_MoveRight;
char input_WebForm_SelectNext;

WLY_GAMEPLAY_STATE gameplayState;

// Pause and Game Over menu UI 
static splitslib_ui_Menu menu_GameOver;
int menuItem_GameOver_PlayAgainLink_Id;
int menuItem_GameOver_ExitToDesktopLink_Id;
int menuItem_GameOver_ExitToModeSelectLink_Id;
int menuItem_GameOver_ExitToTitleLink_Id;

static splitslib_ui_Menu menu_Pause;
int menuItem_Pause_ContinueLink_Id;
int menuItem_Pause_ExitToModeSelectLink_Id;
int menuItem_Pause_ExitToTitleLink_Id;
int menuItem_Pause_ExitToDesktopLink_Id;

RenderTexture2D rtexture_menuRibbon;
RenderTexture2D rtexture_menuRibbonFlipped;

void wly_GameplayMixed_Init(wly_GameSettings settings){
    InitGameSettings(settings);
    
    InitScoreboard();
    UpdateScoreboardViewToGameplayMode();

    LoadRenderTextures();

    InitGameOverMenu();
    InitPauseMenu();

    InitInputMappings();
    
    for (int i = 0; i < gameSettings.playgridCount; i++){
        ResetPlaygrid(&playgrids[i], i);
    }
    
    InitEnemyPellets();
    InitEnergyPellets();

    activePlaygridIndex = 0;
    scoreboard.current = 0;
    gameplayState = WLY_GAMEPLAY_STATE_Play;
}

void wly_GameplayMixed_Update(){    
    if (SplitCanvas_IsTransitioning()){
        SplitCanvas_ContinueTransition();
        return;
    }
    
    wly_PlayerAvatar *paPtr;
    
    switch(gameplayState){
        
    case WLY_GAMEPLAY_STATE_GameOver:
        splitslib_ui_UpdateMenu(&menu_GameOver);
        if (splitslib_si_IsMenuSelectPressed()){
            splitslib_a_PlayCommonSound(SPLITSLIB_SOUND_MenuSelect);  
            DoTheSelectedGameOverMenuThing();
            SplitCanvas_StartTransition(WLY_WINDOW_WIDTH / 2.0);
        }
        break;

    case WLY_GAMEPLAY_STATE_Pause:
        splitslib_ui_UpdateMenu(&menu_Pause);
        if (splitslib_si_IsMenuSelectPressed()){
            splitslib_a_PlayCommonSound(SPLITSLIB_SOUND_MenuSelect);
            DoTheSelectedPauseMenuThing();
            SplitCanvas_StartTransition(WLY_WINDOW_WIDTH / 2.0);
        }   
        break;
        
    case WLY_GAMEPLAY_STATE_Play:
        if (splitslib_si_IsPausePressed()){
            splitslib_a_PlayCommonSound(SPLITSLIB_SOUND_PauseGame);
            gameplayState = WLY_GAMEPLAY_STATE_Pause;
            SplitCanvas_StartTransition(WLY_WINDOW_WIDTH / 2.0);
            break;
        }
        frameTime = GetFrameTime();
        HandleSwitchActivePlaygridInput();
        HandleSwitchPlaygridMoveDirectionInput();
        HandleMoveInput();
        UpdatePlaygrids();
        if(
            AnyAvatarHasJustBeenHitByAnEnemyPellet() ||
            AnyAvatarHasJustRunOutOfLife()
        ){
            gameplayState = WLY_GAMEPLAY_STATE_GameOver;
            UpdateScoreboardViewToMenuMode();
            SaveNewHighScoreIfApplicable();
            SplitCanvas_StartTransition(WLY_WINDOW_WIDTH / 2.0);
        }
        break;
    }
}

void wly_GameplayMixed_Draw(){
    switch(gameplayState){
    case WLY_GAMEPLAY_STATE_GameOver:
        DrawGameOverMenu();
        SplitCanvas_BeginTextureMode();
            DrawPlayfields();
            DrawTexture(rtexture_menuRibbon.texture, 0, 120, WHITE);
        SplitCanvas_EndTextureMode();        
        break;
    
    case WLY_GAMEPLAY_STATE_Pause:
        DrawPauseMenu();
        SplitCanvas_BeginTextureMode();
            DrawPlayfields();
            DrawTexture(rtexture_menuRibbon.texture, 0, 120, WHITE);
        SplitCanvas_EndTextureMode();
        break;
    
    case WLY_GAMEPLAY_STATE_Dying:
    case WLY_GAMEPLAY_STATE_Play:
        SplitCanvas_BeginTextureMode();
            DrawPlayfields();
            DrawScoreboard();
        SplitCanvas_EndTextureMode();
        break;
    }
    
    SplitCanvas_DrawAll();
}

static bool AnyAvatarHasJustBeenHitByAnEnemyPellet(){
    for (int i = 0; i < gameSettings.playgridCount; i++){
        for (int j = 0; j < gameSettings.avatarCount; j++){
            if (
                WLY_AVATARSTATE_JustGotHitByEnemy == 
                playgrids[i].avatars[j].state
            ){
                return true;
            }
        }
    }
    
    return false;
}

static bool AnyAvatarHasJustRunOutOfLife(){
    for (int i = 0; i < gameSettings.playgridCount; i++){
        for (int j = 0; j < gameSettings.avatarCount; j++){
            if (
                WLY_AVATARSTATE_OutOfLife == 
                playgrids[i].avatars[j].state
            ){
                return true;
            }
        }
    }
    
    return false;  
}

static void CleanUpAndExitTo(WLY_STATE newState){
    FreeMenus();
    FreeStrings();
    UnloadRenderTextures();
    wly_SetState(newState);
}

static bool ColumnTargeted(int i, wly_Playgrid *playgrid){
    if (playgrid->columns[i].isActive && playgrid->columns[i].isEmpty){
        playgrid->move.targetColumnIndex = i;
        playgrid->move.targetPos = (Vector2){
            playgrid->columns[playgrid->move.targetColumnIndex].centerX - 
            gameSettings.scale * (WLY_AVATAR_Width / 2.0),
            playgrid->move.sourcePos.y
        };
        return true;
    }
    return false;
}

// TODO make this frame-rate independent / otherwise improve?
static void ContinueAnimatingTheMove(wly_Playgrid *pgPtr){
    wly_PlayerAvatar *paPtr = &(pgPtr->avatars[pgPtr->move.sourceAvatarIndex]);
    
    paPtr->pos.x += 
        gameSettings.speedAvatarMovePerFrame * 
        (pgPtr->move.targetPos.x - pgPtr->move.sourcePos.x);
}

static void DetectAndHandleCollisionsBetweenAvatarsAndPellets
(wly_PelletManager *pmPtr, int playgridIndex)
{
    wly_Pellet *pPtr;
    wly_PlayerAvatar *paPtr;
    
    for (int i = 0; i < gameSettings.avatarCount; i++){
        paPtr = &(playgrids[playgridIndex].avatars[i]);
        
        for (int j = 0; j < WLY_PELLETMANAGER_POOLSIZE; j++){
            pPtr = &(pmPtr->pelletPool[j]);
            
            if (WLY_PELLET_STATE_Inactive == pPtr->state) continue;
            
            if (ThereIsACollisionBetweenAnAvatarAndAPellet(paPtr, pPtr)){
                HandleACollisionBetweenAnAvatarAndAPellet(paPtr, pPtr, pmPtr->type);
            }
        }
    }    
}

static void DoTheSelectedGameOverMenuThing(){
    if (
        menuItem_GameOver_PlayAgainLink_Id ==
        menu_GameOver.cursor
    ){
        ResetGame();
        gameplayState = WLY_GAMEPLAY_STATE_Play;
    }
    else if (
        menuItem_GameOver_ExitToModeSelectLink_Id ==
        menu_GameOver.cursor
    ){
        CleanUpAndExitTo(WLY_STATE_ModeSelect);
    }
    else if (
        menuItem_GameOver_ExitToTitleLink_Id ==
        menu_GameOver.cursor
    ){
        CleanUpAndExitTo(WLY_STATE_Title);
    }
    else if (
        menuItem_GameOver_ExitToDesktopLink_Id ==
        menu_GameOver.cursor
    ){
        CleanUpAndExitTo(WLY_STATE_EXIT);
    }
}

static void DoTheSelectedPauseMenuThing(){
    if (
        menuItem_Pause_ContinueLink_Id ==
        menu_Pause.cursor
    ){
        gameplayState = WLY_GAMEPLAY_STATE_Play;
    }
    else if (
        menuItem_Pause_ExitToModeSelectLink_Id ==
        menu_Pause.cursor
    ){
        CleanUpAndExitTo(WLY_STATE_ModeSelect);
    }
    else if (
        menuItem_Pause_ExitToTitleLink_Id ==
        menu_Pause.cursor
    ){
        CleanUpAndExitTo(WLY_STATE_Title);
    }
    else if (
        menuItem_Pause_ExitToDesktopLink_Id ==
        menu_Pause.cursor
    ){
        CleanUpAndExitTo(WLY_STATE_EXIT);
    }
}

static void DrawAvatars(wly_Playgrid *pgPtr){
    float topY;
    float avatarX;
    int index;
    wly_PlayerAvatar *paPtr;

    for (int i = 0; i < gameSettings.avatarCount; i++){
        paPtr = &(pgPtr->avatars[i]);
        
        if (1 == pgPtr->pgFlip_State){
            topY = 
                pgPtr->pos.y + 
                paPtr->pos.y - 
                paPtr->height;
            avatarX = 
                pgPtr->pos.x + 
                paPtr->pos.x;
        }
        else if (-1 == pgPtr->pgFlip_State){ // upside down
            topY = pgPtr->pos.y;
            avatarX =
                pgPtr->pos.x +
                gameSettings.scaledPlaygridWidth - 
                paPtr->pos.x - 
                paPtr->width;
        }

        DrawRectangle(
            avatarX,
            topY,
            paPtr->width, paPtr->height,
            (Color){
                paPtr->rgbValue, 
                paPtr->rgbValue,
                paPtr->rgbValue,
                255
            }
        );
        
        // TODO revise this drawing of the "kill"
        
        DrawRectangle(
            pgPtr->pos.x + 
            pgPtr->avatars[i].pos.x + 
            WLY_AVATAR_Width * 0.5 - WLY_ENEMYPELLET_SIZE_MIN / 2,
            topY,
            WLY_ENEMYPELLET_SIZE_MIN, pgPtr->avatars[i].offset,
            WLY_BG_Color);
        
    }
    
    switch(gameSettings.controlScheme){
    case WLY_GAME_CONTROL_SCHEME_Classic:
        // nothing special
        break;
    case WLY_GAME_CONTROL_SCHEME_ColumnSwap:
        if (!pgPtr->controlScheme_ColumnSwap_IsSourceColumnSet) break;
        DrawRectangle(
            pgPtr->pos.x + 
            pgPtr->avatars[pgPtr->move.sourceAvatarIndex].pos.x,
            pgPtr->pos.y + 
            pgPtr->avatars[pgPtr->move.sourceAvatarIndex].pos.y - 32,
            32, 32,
            RED);
        break;

    case WLY_GAME_CONTROL_SCHEME_Fixed:
        for (int i = 0; i < gameSettings.avatarCount; i++){
            topY = 
                pgPtr->pos. y +
                pgPtr->avatars[i].pos.y - 
                pgPtr->avatars[i].height;
            for (int j = 0; j < i + 1; j++){
                DrawCircle(
                    pgPtr->pos.x + 
                    pgPtr->avatars[i].pos.x + WLY_AVATAR_Width / 2,
                    topY - j * 24,
                    8,
                    ORANGE);
            }
        }
        break;
        
    case WLY_GAME_CONTROL_SCHEME_WebForm:
        DrawRectangle(
            pgPtr->pos.x + 
            pgPtr->avatars[pgPtr->controlScheme_Fixed_SelectedAvatarIndex].pos.x,
            pgPtr->pos.y +
            pgPtr->avatars[pgPtr->controlScheme_Fixed_SelectedAvatarIndex].pos.y - 32,
            32, 32,
            ORANGE);
        break;
    }
}

static void DrawDangerIndicators(wly_Playgrid *pgPtr){
    float avatarX;
    
    for (int i = 0; i < gameSettings.avatarCount; i++){
        if (1 == pgPtr->pgFlip_State){
            avatarX = 
                pgPtr->pos.x + 
                pgPtr->avatars[i].pos.x;
        }
        else if (-1 == pgPtr->pgFlip_State){ // upside down
            avatarX =
                pgPtr->pos.x +
                gameSettings.scaledPlaygridWidth - 
                pgPtr->avatars[i].pos.x - 
                pgPtr->avatars[i].width;
        }
        
        if (
            pgPtr->avatars[i].height < 
            WLY_AVATAR_DangerIndicatorThreshold
        ){
            float propDeficit =
                (WLY_AVATAR_DangerIndicatorThreshold - 
                pgPtr->avatars[i].height) /
                WLY_AVATAR_DangerIndicatorThreshold;

            DrawRectangle(
                avatarX, 
                pgPtr->pos.y,
                pgPtr->avatars[i].width,
                propDeficit * WLY_PLAYGRID_Height,
                WLY_AVATAR_DangerIndicatorColor
            );
        }
    }
}

static void DrawGameOverMenu(){
    BeginTextureMode(rtexture_menuRibbonFlipped);
        DrawMenuRibbonBorders();
        splitslib_ui_DrawMenu(&menu_GameOver, false);
DrawLine(
    splitslib_d_screen.center.x, 0,
    splitslib_d_screen.center.x, WLY_MENURIBBON_HEIGHT,
    RED
);
        DrawScoreboard();
    EndTextureMode();
    BeginTextureMode(rtexture_menuRibbon);
        DrawTexture(rtexture_menuRibbonFlipped.texture, 0, 0, WHITE);
    EndTextureMode();
}

static void DrawMenuRibbonBorders(){
    DrawRectangle(0, WLY_MenuRibbon_Top_Y, WLY_WINDOW_WIDTH, WLY_MenuRibbon_BorderWidth, BLACK);
    DrawRectangle(0, WLY_MenuRibbon_Bottom_Y, WLY_WINDOW_WIDTH, WLY_MenuRibbon_BorderWidth, BLACK);
}

static void DrawPauseMenu(){
    BeginTextureMode(rtexture_menuRibbonFlipped);
        DrawMenuRibbonBorders();
        splitslib_ui_DrawMenu(&menu_Pause, false);
    EndTextureMode();
    BeginTextureMode(rtexture_menuRibbon);
        DrawTexture(rtexture_menuRibbonFlipped.texture, 0, 0, WHITE);
    EndTextureMode();
}

static void DrawPlayfields(){
    for (int i = 0; i < gameSettings.playgridCount; i++){
        if (WLY_GAME_MODE_Stressful != gameSettings.mode){
            DrawDangerIndicators(&playgrids[i]);
        }
        DrawPlaygrid(&playgrids[i], i);
        PelletManager_Draw(&energyPMs[i], &playgrids[i]);
        PelletManager_Draw(&enemyPMs[i], &playgrids[i]);
        DrawAvatars(&playgrids[i]);
    }
}

static void DrawPlaygrid(wly_Playgrid *pgPtr, int pgIndex){
    // Draw active indicator
    if (
        gameSettings.playgridCount > 1 && 
        pgIndex == activePlaygridIndex
    ){
        DrawRectangle(
            pgPtr->pos.x, pgPtr->pos.y, 
            gameSettings.scaledPlaygridActiveIndicatorVisibleWidth,
            WLY_PLAYGRID_Height,
            WLY_PLAYGRID_ACTIVE_INDICATOR_Color
        );
        DrawRectangle(
            pgPtr->pos.x + 
            gameSettings.scaledPlaygridWidth -
            gameSettings.scaledPlaygridActiveIndicatorVisibleWidth,
            pgPtr->pos.y,
            gameSettings.scaledPlaygridActiveIndicatorVisibleWidth,
            WLY_PLAYGRID_Height,
            WLY_PLAYGRID_ACTIVE_INDICATOR_Color
        );
    }
    
    DrawRectangle(
        pgPtr->pos.x + 
        gameSettings.scaledPlaygridActiveIndicatorWidth,
        pgPtr->pos.y, 
        gameSettings.scaledPlaygridBorderWidth,
        WLY_PLAYGRID_Height, 
        WLY_PLAYGRID_BORDER_Color
    );
    DrawRectangle(
        pgPtr->pos.x + 
        gameSettings.scaledPlaygridWidth -
        gameSettings.scaledPlaygridActiveIndicatorWidth -
        gameSettings.scaledPlaygridBorderWidth,
        pgPtr->pos.y,
        gameSettings.scaledPlaygridBorderWidth, 
        WLY_PLAYGRID_Height,
        WLY_PLAYGRID_BORDER_Color
    );
        
    // TODO don't recalculate this every frame!
    float leftmostColumnLeftX = 
        gameSettings.scaledPlaygridActiveIndicatorWidth +
        gameSettings.scaledPlaygridBorderWidth;
    for (int i = 1; i < gameSettings.columnCount; i++){
        DrawRectangle(
            pgPtr->pos.x + 
            leftmostColumnLeftX + 
            (i * gameSettings.scaledPlaygridColumnWidth) +
            ((i-1) * gameSettings.scaledPlaygridColumnDividerWidth),
            pgPtr->pos.y,
            gameSettings.scaledPlaygridColumnDividerWidth, 
            WLY_PLAYGRID_Height,
            WLY_PLAYGRID_COLUMN_DIVIDER_Color
        );
    }
}

static void DrawScoreboard(){
    splitslib_sf_DrawString(
        scoreboard.view_current.id,
        scoreboard.view_current.pos.x,
        scoreboard.view_current.pos.y
    );
    splitslib_sf_DrawString(
        scoreboard.view_high.id,
        scoreboard.view_high.pos.x,
        scoreboard.view_high.pos.y
    );
    splitslib_sf_DrawString(
        scoreboard.view_multiplier.id,
        scoreboard.view_multiplier.pos.x,
        scoreboard.view_multiplier.pos.y
    );
}

static void FinalizeSourceToTargetMove
(wly_Playgrid *pgPtr, wly_PlayerMove *mPtr){
    pgPtr->columns[mPtr->sourceColumnIndex].isEmpty = true;
    pgPtr->columns[mPtr->sourceColumnIndex].playerAvatarIndex = -1;
    
    pgPtr->columns[mPtr->targetColumnIndex].isEmpty = false;
    pgPtr->columns[mPtr->targetColumnIndex].playerAvatarIndex = 
        mPtr->sourceAvatarIndex;
        
    pgPtr->avatars[mPtr->sourceAvatarIndex].columnIndex =
        mPtr->targetColumnIndex;
        
    pgPtr->avatars[mPtr->sourceAvatarIndex].pos = mPtr->targetPos;
}

static void FreeMenus(){
    splitslib_ui_FreeMenu(&menu_GameOver);
    splitslib_ui_FreeMenu(&menu_Pause);
}

static void FreeStrings(){
    splitslib_sf_FreeString(scoreboard.view_current.id);
    splitslib_sf_FreeString(scoreboard.view_high.id);
}

static void HandleACollisionBetweenAnAvatarAndAPellet
(wly_PlayerAvatar *paPtr, wly_Pellet *pPtr, WLY_PELLET_TYPE type){        
    switch(type){
    case WLY_PELLET_TYPE_Enemy:
        splitslib_a_PlayCustomSound(WLY_PROJECTAUDIO_SFX_EnemyGameOver);
        paPtr->state = WLY_AVATARSTATE_JustGotHitByEnemy;
        PelletManager_DeactivatePellet(pPtr);
        break;
        
    case WLY_PELLET_TYPE_Energy:
        splitslib_a_PlayCustomSound(
            WLY_PROJECTAUDIO_SFX_EnergyEat
        );
        
        splitslib_anim_Destroy(paPtr->eatFlashTweenId);
        paPtr->eatFlashTweenId = splitslib_anim_Init(
            WLY_CLASSIC_AVATAREATFLASH_SourceRGBValue,
            WLY_AVATAREATFLASH_TargetRGBValue,
            WLY_AVATAREATFLASH_SecondsToTarget,
            WLY_AVATAREATFLASH_SecondsAtTarget,
            WLY_AVATAREATFLASH_SecondsToSource,
            false, // do not loop
            true // is a roundtrip, yes
        );
        
        paPtr->state = WLY_AVATARSTATE_Growing;
        paPtr->growTarget += gameSettings.energyPelletValue;
        
        wly_UpdateScoreboard(1);
        
        PelletManager_DeactivatePellet(pPtr);
        break;
    }
}

static void HandleMoveInput(){
    wly_PlayerMove *movePtr = &(playgrids[activePlaygridIndex].move);
    wly_Playgrid *pgPtr = &playgrids[activePlaygridIndex];
    int currentlySelectedAvatarColumnIndex;
    int newlySelectedAvatarIndex;

    switch(movePtr->state){
        
    case WLY_MOVESTATE_ColumnSwapInProgress:
        FinalizeSourceToTargetMove(pgPtr, movePtr);
        if (-1 != movePtr->targetAvatarIndex){
            pgPtr->columns[movePtr->sourceColumnIndex].isEmpty = false;
            pgPtr->columns[movePtr->sourceColumnIndex].playerAvatarIndex =
                movePtr->targetAvatarIndex;
            
            pgPtr->avatars[movePtr->targetAvatarIndex].columnIndex =
                movePtr->sourceColumnIndex;
                
            pgPtr->avatars[movePtr->targetAvatarIndex].pos = movePtr->sourcePos;
        }        
        pgPtr->controlScheme_ColumnSwap_IsSourceColumnSet = false;    
        ResetMove(pgPtr);
        break;
        
    case WLY_MOVESTATE_FailedMoveAttemptInProgress:
        // TODO flesh out this STUB
        ResetMove(pgPtr);
        
        break;
        
    case WLY_MOVESTATE_MoveInProgress:
        ContinueAnimatingTheMove(pgPtr);
        if (TheAnimationIsDone(pgPtr)){
            FinalizeSourceToTargetMove(pgPtr, movePtr);
            switch(gameSettings.controlScheme){
            case WLY_GAME_CONTROL_SCHEME_Classic:
                UpdateAvatarIndexArray_LeftToRight(pgPtr);
                break;
            }
            ResetMove(pgPtr);
        }
        break;
        
    case WLY_MOVESTATE_NotMoving:
        switch(gameSettings.controlScheme){
        case WLY_GAME_CONTROL_SCHEME_Classic:
            for (int i = 0; i < gameSettings.avatarCount; i++){
                if (IsKeyPressed(input_MoveLeft[i])){
                   movePtr->sourceAvatarIndex = pgPtr->avatarIndices_LeftToRight[i];
                   TryToStartMovingLeft(movePtr, pgPtr);
                   RotatePlaygrid(pgPtr);
                   break;
                }
                else if (IsKeyPressed(input_MoveRight[i])){
                    movePtr->sourceAvatarIndex = 
                        pgPtr->avatarIndices_LeftToRight[(gameSettings.avatarCount - 1) - i];
                    TryToStartMovingRight(movePtr, pgPtr);
                    RotatePlaygrid(pgPtr);
                    break;
                }
            }
            break;
        
        case WLY_GAME_CONTROL_SCHEME_ColumnSwap:
            for (int i = 0; i < gameSettings.columnCount; i++){
                if (IsKeyPressed(input_ColumnSwap_Columns[i])){
                    if (pgPtr->controlScheme_ColumnSwap_IsSourceColumnSet){
                        if (!pgPtr->columns[i].isEmpty){
                            splitslib_a_PlayCustomSound(
                                WLY_PROJECTAUDIO_SFX_TargetColumnAvatar
                            );
                            movePtr->targetAvatarIndex = 
                                pgPtr->columns[i].playerAvatarIndex;
                        }
                        else {
                            splitslib_a_PlayCustomSound(
                                WLY_PROJECTAUDIO_SFX_TargetColumnEmpty
                            );
                        }
                        SetTargetColumn(pgPtr, i);
                        movePtr->state = WLY_MOVESTATE_ColumnSwapInProgress;
                    }
                    else {
                        if (pgPtr->columns[i].isEmpty){
                            break; // TODO flesh this out to add a failure sound / animation, etc
                        }
                        splitslib_a_PlayCustomSound(
                            WLY_PROJECTAUDIO_SFX_SuccessfulSourceColumn
                        );
                        movePtr->sourceAvatarIndex = pgPtr->columns[i].playerAvatarIndex;
                        NoteDownStartingColumnAndPosition(pgPtr);
                        pgPtr->controlScheme_ColumnSwap_IsSourceColumnSet = true;
                        break;
                    }
                }
            }
            break;
        
        case WLY_GAME_CONTROL_SCHEME_Fixed:
            for (int i = 0; i < gameSettings.avatarCount; i++){
                if (IsKeyPressed(input_MoveLeft[i])){
                    movePtr->sourceAvatarIndex = i;
                    TryToStartMovingLeft(movePtr, pgPtr);
                    break;
                }
                else if (IsKeyPressed(input_MoveRight[i])){
                    movePtr->sourceAvatarIndex = i;
                    TryToStartMovingRight(movePtr, pgPtr);
                    break;
                }
            }
            break;

        case WLY_GAME_CONTROL_SCHEME_WebForm:
            if (IsKeyPressed(input_WebForm_MoveLeft)){
                movePtr->sourceAvatarIndex = pgPtr->controlScheme_Fixed_SelectedAvatarIndex;
                TryToStartMovingLeft(movePtr, pgPtr);                
            }
            else if (IsKeyPressed(input_WebForm_MoveRight)){
                movePtr->sourceAvatarIndex = pgPtr->controlScheme_Fixed_SelectedAvatarIndex;
                TryToStartMovingRight(movePtr, pgPtr);
            }
            else if (IsKeyPressed(input_WebForm_SelectNext)){
                currentlySelectedAvatarColumnIndex = 
                    pgPtr->avatars[pgPtr->controlScheme_Fixed_SelectedAvatarIndex].columnIndex;
                newlySelectedAvatarIndex = -1;
                
                for (int i = currentlySelectedAvatarColumnIndex + 1; i < gameSettings.columnCount; i++){
                    if (pgPtr->columns[i].isEmpty) continue;    
                    newlySelectedAvatarIndex = pgPtr->columns[i].playerAvatarIndex;
                    break;
                }
                
                if (-1 == newlySelectedAvatarIndex){
                    for (int i = 0; i < currentlySelectedAvatarColumnIndex; i++){
                        if (pgPtr->columns[i].isEmpty) continue;
                        newlySelectedAvatarIndex = pgPtr->columns[i].playerAvatarIndex;
                        break;
                    }
                }
                
                if (
                    -1 < newlySelectedAvatarIndex && 
                    newlySelectedAvatarIndex < gameSettings.avatarCount
                ){
                    pgPtr->controlScheme_Fixed_SelectedAvatarIndex = newlySelectedAvatarIndex;
                }
            }
        }
        
        break;
    }
}

static void HandleSwitchActivePlaygridInput(){
    if (splitslib_si_IsSwitchPlaygridsPressed()){
        splitslib_a_PlayCustomSound(
            WLY_PROJECTAUDIO_SFX_GridSelect
        );
        activePlaygridIndex++;
        if (activePlaygridIndex >= gameSettings.playgridCount)
            activePlaygridIndex = 0;
    }
}

static void HandleSwitchPlaygridMoveDirectionInput(){
    if (splitslib_si_IsSwitchPlaygridMoveDirectionPressed()){
        splitslib_a_PlayCustomSound(
            WLY_PROJECTAUDIO_SFX_GridReversal
        );
        playgrids[activePlaygridIndex].pgMovement_Dir *= -1;
    }
}

static void InitEnemyPellets(){
    for (int i = 0; i < gameSettings.playgridCount; i++){
        PelletManager_Init(
            &enemyPMs[i], 
            WLY_PELLET_TYPE_Enemy
        );
    }
}

static void InitEnergyPellets(){
    for (int i = 0; i < gameSettings.playgridCount; i++){
        PelletManager_Init(
            &energyPMs[i],
            WLY_PELLET_TYPE_Energy
        );
    }   
}

static void InitGameOverMenu()
{
    splitslib_ui_InitMenu(&menu_GameOver);
    splitslib_ui_AddHeader(
        &menu_GameOver,
        WLY_Menu_GameOver_HeaderText,
        SPLITSLIB_UI_LABEL_ALIGNMENT_Center,
        (Vector2){
            splitslib_d_screen.center.x,
            WLY_MENURIBBON_HEIGHT * 0.1
        }
    );
    menuItem_GameOver_PlayAgainLink_Id =
        splitslib_ui_AddLink(
            &menu_GameOver,
            WLY_Menu_GameOver_LabelText_PlayAgain,
            SPLITSLIB_UI_LABEL_ALIGNMENT_Center,
            (Vector2){
                splitslib_d_screen.center.x,
                WLY_MENURIBBON_HEIGHT * 0.75
            }
        );
    menuItem_GameOver_ExitToModeSelectLink_Id =
        splitslib_ui_AddLink(
            &menu_GameOver,
            WLY_Menu_GameOver_LabelText_ExitToModeSelect,
            SPLITSLIB_UI_LABEL_ALIGNMENT_Center,
            (Vector2){
                splitslib_d_screen.center.x,
                WLY_MENURIBBON_HEIGHT * 0.8
            }
        );
    menuItem_GameOver_ExitToTitleLink_Id =
        splitslib_ui_AddLink(
            &menu_GameOver,
            WLY_Menu_GameOver_LabelText_ExitToTitle,
            SPLITSLIB_UI_LABEL_ALIGNMENT_Center,
            (Vector2){
                splitslib_d_screen.center.x,
                WLY_MENURIBBON_HEIGHT * 0.85
            }
        );
    menuItem_GameOver_ExitToDesktopLink_Id =
        splitslib_ui_AddLink(
            &menu_GameOver,
            WLY_Menu_GameOver_LabelText_ExitToDesktop,
            SPLITSLIB_UI_LABEL_ALIGNMENT_Center,
            (Vector2){
                splitslib_d_screen.center.x,
                WLY_MENURIBBON_HEIGHT * 0.9
            }
        );  
}

static void InitGameSettings(wly_GameSettings settings){
    switch(settings.mode){
    case WLY_GAME_MODE_Classic:
        InitGameSettings_SetAvatarAndColumnCounts(
            settings.avatarCount,
            2, 6,
            3, 7,
            4, 8,
            5, 9
           //WLY_CLASSIC_ColumnCounts
        );
        gameSettings.rateEnemySpawn = 
            WLY_CLASSIC_SecondsBetweenEnemySpawns;
        break;
        
    case WLY_GAME_MODE_Peaceful:
        InitGameSettings_SetAvatarAndColumnCounts(
            settings.avatarCount,
            2, 7,
            3, 8,
            4, 9,
            5, 10
        );
        break;
        // 7, 8, 9, 10
        
    case WLY_GAME_MODE_Stressful:
        InitGameSettings_SetAvatarAndColumnCounts(
            settings.avatarCount,
            2, 5,
            3, 6,
            4, 7,
            5, 8
        );
        gameSettings.rateEnemySpawn = 
            WLY_STRESSFUL_SecondsBetweenEnemySpawns;
        break;
    }

    gameSettings.speedAvatarDecayPerSecond = 
        WLY_PLAYGRID_TotalDecayPerSecond / 
        gameSettings.avatarCount;
    gameSettings.controlScheme = settings.controlScheme;
    gameSettings.mode = settings.mode;
    gameSettings.playgridCount = settings.playgridCount;
    gameSettings.shouldPlaygridsMove = settings.shouldPlaygridsMove;
    gameSettings.shouldPlaygridsFlip = settings.shouldPlaygridsFlip;
    
    gameSettings.playgridWidth = 
        (WLY_PLAYGRID_ACTIVE_INDICATOR_Width * 2) +
        (WLY_PLAYGRID_BORDER_Width * 2) +
        (gameSettings.columnCount * WLY_PLAYGRID_COLUMN_Width) + 
        ((gameSettings.columnCount - 1) * WLY_PLAYGRID_COLUMN_DIVIDER_Width);
    
    // scale playgrids if necessary
    gameSettings.scale = 1.0f;
    
    float totalWidth = 
        (gameSettings.playgridWidth * gameSettings.playgridCount) +
        (2 * WLY_GAME_SideBorderWidth) + 
        ((gameSettings.playgridCount - 1) * WLY_GAME_PlaygridDividerWidth);
        
    if (totalWidth <= splitslib_d_screen.width){
        gameSettings.scale = 1.0f;
    }
    else {
        float maxPlaygridWidth =
            splitslib_d_screen.width -
            (2 * WLY_GAME_SideBorderWidth) -
            ((gameSettings.playgridCount - 1) * WLY_GAME_PlaygridDividerWidth);
        maxPlaygridWidth = maxPlaygridWidth / gameSettings.playgridCount;
            
        gameSettings.scale = 
            maxPlaygridWidth / 
            gameSettings.playgridWidth;
    }
    
    // scale everything appropriately
    gameSettings.scaledAvatarWidth = 
        WLY_AVATAR_Width * gameSettings.scale;
    gameSettings.scaledPlaygridBorderWidth = 
        WLY_PLAYGRID_BORDER_Width * gameSettings.scale;
    gameSettings.scaledPlaygridBorderWidth = 
        splitslib_util_Clamp(
            gameSettings.scaledPlaygridBorderWidth,
            2,
            gameSettings.scaledPlaygridBorderWidth
        );
    gameSettings.scaledPlaygridColumnDividerWidth = 
        WLY_PLAYGRID_COLUMN_DIVIDER_Width * gameSettings.scale;
    gameSettings.scaledPlaygridColumnDividerWidth = 
        splitslib_util_Clamp(
            gameSettings.scaledPlaygridColumnDividerWidth,
            1,
            gameSettings.scaledPlaygridColumnDividerWidth
        );
    gameSettings.scaledPlaygridColumnWidth = 
        WLY_PLAYGRID_COLUMN_Width * gameSettings.scale;
    gameSettings.scaledPlaygridWidth = 
        gameSettings.playgridWidth * gameSettings.scale;
    gameSettings.scaledPlaygridActiveIndicatorVisibleWidth = 
        (WLY_PLAYGRID_ACTIVE_INDICATOR_Width / 2) * gameSettings.scale;
    gameSettings.scaledPlaygridActiveIndicatorWidth =
        WLY_PLAYGRID_ACTIVE_INDICATOR_Width * gameSettings.scale;
        
        
    // scale the game speed similiarly
    InitGameSettings_SetSpeed(settings.gameSpeed);
}

static void InitGameSettings_SetAvatarAndColumnCounts(
int avatarCount, int a2, int c2, int a3, int c3, int a4, int c4, int a5, int c5
){
    switch(avatarCount){
    case 2:
        gameSettings.avatarCount = a2;
        gameSettings.columnCount = c2;
        break;
    case 3:
        gameSettings.avatarCount = a3;
        gameSettings.columnCount = c3;
        break;
    case 4:
        gameSettings.avatarCount = a4;
        gameSettings.columnCount = c4;
        break;
    case 5:
        gameSettings.avatarCount = a5;
        gameSettings.columnCount = c5;
        break;
    }
}

static void InitGameSettings_SetSpeed(int speed){
    gameSettings.speedMultiplier = wly_speedMultipliers[speed];
    gameSettings.gameSpeed = speed;
    gameSettings.energyPelletValue = WLY_ENERGYPELLET_ENERGYVALUE;
    // TODO change the enemy spawn rate,
    // but not as much as the energy spawn rate
    gameSettings.rateEnergySpawn = 
        WLY_ENERGYPELLET_SPAWNDELAY_SEC / gameSettings.speedMultiplier;
        
    // value previously initialized based on avatar count
    // we increase the decay value slightly less (proportionally)
    // since movement is not instant
    float decayMultiplier = splitslib_util_Clamp(
        gameSettings.speedMultiplier,
        1.0,
        0.9 * gameSettings.speedMultiplier
    );
    gameSettings.speedAvatarDecayPerSecond *= decayMultiplier;

    gameSettings.speedAvatarGrowPerSecond =
        WLY_AVATAR_GrowPerSecond * gameSettings.speedMultiplier;
    gameSettings.speedAvatarMovePerFrame =
        WLY_AVATAR_MovePerFrame * gameSettings.speedMultiplier;
    gameSettings.speedEnemy = 
        WLY_ENEMYPELLET_VSPEED_PERSEC * gameSettings.speedMultiplier;
    gameSettings.speedEnergy = 
        WLY_ENERGYPELLET_VSPEED_PERSEC * gameSettings.speedMultiplier;
    gameSettings.speedPlaygridMove =
        WLY_PLAYGRID_MovementSpeed_Persec * gameSettings.speedMultiplier;
}

static void InitInputMappings(){
    switch(gameSettings.controlScheme){
    case WLY_GAME_CONTROL_SCHEME_Classic:
    case WLY_GAME_CONTROL_SCHEME_Fixed:
        input_MoveLeft[0] = WLY_INPUT_MOVELEFT_ONE;
        input_MoveLeft[1] = WLY_INPUT_MOVELEFT_TWO;
        input_MoveLeft[2] = WLY_INPUT_MOVELEFT_THREE;
        input_MoveLeft[3] = WLY_INPUT_MOVELEFT_FOUR;
        input_MoveLeft[4] = WLY_INPUT_MOVELEFT_FIVE;
        
        input_MoveRight[0] = WLY_INPUT_MOVERIGHT_ONE;
        input_MoveRight[1] = WLY_INPUT_MOVERIGHT_TWO;
        input_MoveRight[2] = WLY_INPUT_MOVERIGHT_THREE;
        input_MoveRight[3] = WLY_INPUT_MOVERIGHT_FOUR;
        input_MoveRight[4] = WLY_INPUT_MOVERIGHT_FIVE;
        break;
        
    case WLY_GAME_CONTROL_SCHEME_ColumnSwap:
        input_ColumnSwap_Columns[0] = WLY_INPUT_ColumnSwap_LEFTMOST;
        input_ColumnSwap_Columns[1] = WLY_INPUT_ColumnSwap_SECONDFROMLEFT;
        input_ColumnSwap_Columns[2] = WLY_INPUT_ColumnSwap_THIRDFROMLEFT;
        input_ColumnSwap_Columns[3] = WLY_INPUT_ColumnSwap_FOURTHFROMLEFT;
        input_ColumnSwap_Columns[4] = WLY_INPUT_ColumnSwap_FIFTHFROMLEFT;
        input_ColumnSwap_Columns[5] = WLY_INPUT_ColumnSwap_SIXTHFROMLEFT;
        input_ColumnSwap_Columns[6] = WLY_INPUT_ColumnSwap_SEVENTHFROMLEFT;
        input_ColumnSwap_Columns[7] = WLY_INPUT_ColumnSwap_EIGHTHFROMLEFT;
        input_ColumnSwap_Columns[8] = WLY_INPUT_ColumnSwap_NINTHFROMLEFT;
        input_ColumnSwap_Columns[9] = WLY_INPUT_ColumnSwap_TENTHFROMLEFT;
        break;
        
    case WLY_GAME_CONTROL_SCHEME_WebForm:
        input_WebForm_MoveLeft = WLY_INPUT_WEBFORM_MOVELEFT;
        input_WebForm_MoveRight = WLY_INPUT_WEBFORM_MOVERIGHT;
        input_WebForm_SelectNext = WLY_INPUT_WEBFORM_SELECTNEXT;
        break;
    }
}

static void InitPauseMenu(){
    splitslib_ui_InitMenu(&menu_Pause);
    splitslib_ui_AddHeader(
        &menu_Pause,
        WLY_Menu_Pause_HeaderText,
        SPLITSLIB_UI_LABEL_ALIGNMENT_Center,
        (Vector2){
            splitslib_d_screen.center.x,
            WLY_MENURIBBON_HEIGHT * 0.2
        }
    );
    splitslib_sf_SetKern(
        menu_Pause.headerStringbox.id,
        0.05
    );
    splitslib_sf_SetSize(
        menu_Pause.headerStringbox.id,
        0.2
    );
    splitslib_sf_SetStrokeWeight(
        menu_Pause.headerStringbox.id,
        0.02
    );
    
    splitslib_sf_Result result = 
        splitslib_sf_GetWidth(menu_Pause.headerStringbox.id);
    float stringWidth = result.floatValue;
    splitslib_sf_SetStringboxPosition(
        &(menu_Pause.headerStringbox),
        (Vector2){
            splitslib_d_screen.center.x - 
            stringWidth / 2,
            WLY_MENURIBBON_HEIGHT * 0.25
        }
    );
    
    menuItem_Pause_ContinueLink_Id =
        splitslib_ui_AddLink(
            &menu_Pause,
            WLY_Menu_Pause_LabelText_Continue,
            SPLITSLIB_UI_LABEL_ALIGNMENT_Center,
            (Vector2){
                splitslib_d_screen.center.x,
                WLY_MENURIBBON_HEIGHT * 0.75
            }
        );
    menuItem_Pause_ExitToModeSelectLink_Id =
        splitslib_ui_AddLink(
            &menu_Pause,
            WLY_Menu_Pause_LabelText_ExitToModeSelect,
            SPLITSLIB_UI_LABEL_ALIGNMENT_Center,
            (Vector2){
                splitslib_d_screen.center.x,
                WLY_MENURIBBON_HEIGHT * 0.8
            }
        );
    menuItem_Pause_ExitToTitleLink_Id =
        splitslib_ui_AddLink(
            &menu_Pause,
            WLY_Menu_Pause_LabelText_ExitToTitle,
            SPLITSLIB_UI_LABEL_ALIGNMENT_Center,
            (Vector2){
                splitslib_d_screen.center.x,
                WLY_MENURIBBON_HEIGHT * 0.85
            }
        );
    menuItem_Pause_ExitToDesktopLink_Id =
        splitslib_ui_AddLink(
            &menu_Pause,
            WLY_Menu_Pause_LabelText_ExitToDesktop,
            SPLITSLIB_UI_LABEL_ALIGNMENT_Center,
            (Vector2){
                splitslib_d_screen.center.x,
                WLY_MENURIBBON_HEIGHT * 0.9
            }
        );
}

static void InitScoreboard(){
    scoreboard.high = wly_highScores
        [wly_highScoreIndices[0]]
        [wly_highScoreIndices[1]];
    
    splitslib_sf_InitStringbox(
        &scoreboard.view_current,
        WLY_CurrentScoreStringboxStartText,
        WLY_CurrentScoreStringboxTextSize,
        WLY_CurrentScoreStringboxTextWeight,
        WLY_CurrentScoreStringboxTextKern,
        WLY_CurrentScoreStringboxTextY
    );
    
    splitslib_sf_InitStringbox(
        &scoreboard.view_high,
        WLY_HighScoreStringboxStartText,
        WLY_HighScoreStringboxTextSize,
        WLY_HighScoreStringboxTextWeight,
        WLY_HighScoreStringboxTextKern,
        WLY_HighScoreStringboxTextY
    );
    
    splitslib_sf_InitStringbox(
        &scoreboard.view_multiplier,
        WLY_ScoreMultiplierStringboxStartText,
        WLY_ScoreMultiplierStringboxTextSize,
        WLY_ScoreMultiplierStringboxTextWeight,
        WLY_ScoreMultiplierStringboxTextKern,
        WLY_ScoreMultiplierStringboxTextY
    );
    splitslib_sf_SetText(
        scoreboard.view_multiplier.id,
        FormatText("MULTIPLIER %dX", scoreboard.scoreMultiplier)
    );
}

static bool KillAnimationHasFinished(){
    for (int i = 0; i < gameSettings.playgridCount; i++){
        for (int j = 0; j < gameSettings.avatarCount; j++){
            if (
                WLY_AVATARSTATE_Dead == 
                playgrids[i].avatars[j].state
            ){
                return true;
            }
        }
    }
    
    return false;
}

static void LoadRenderTextures(){
    rtexture_menuRibbon = LoadRenderTexture(WLY_WINDOW_WIDTH, WLY_MENURIBBON_HEIGHT);
    rtexture_menuRibbonFlipped = LoadRenderTexture(WLY_WINDOW_WIDTH, WLY_MENURIBBON_HEIGHT);
}

static void MovePlaygrid(wly_Playgrid *pgPtr){
    if (gameSettings.shouldPlaygridsMove){
        pgPtr->pos.x +=
            pgPtr->pgMovement_Dir *
            (pgPtr->pgMovement_Speed * frameTime);
    }
}

static void NoteDownStartingColumnAndPosition(wly_Playgrid *playgrid){
    playgrid->move.sourceColumnIndex = 
        playgrid->avatars[playgrid->move.sourceAvatarIndex].columnIndex;
    playgrid->move.sourcePos =
        playgrid->avatars[playgrid->move.sourceAvatarIndex].pos;
}

static void PositionAvatars(wly_Playgrid *playgrid){
    for (int i = 0; i < gameSettings.avatarCount; i++){
        playgrid->columns[i].isEmpty = false;
        playgrid->columns[i].playerAvatarIndex = i;
        
        playgrid->avatars[i].pos = (Vector2){
            playgrid->columns[i].centerX - playgrids->avatars[i].width / 2.0, 
            WLY_PLAYGRID_Height};
        
        playgrid->avatars[i].columnIndex = i;
    }
}

static void PositionColumns(wly_Playgrid *playgrid){
    for (int i = 0; i < gameSettings.columnCount; i++){
        playgrid->columns[i].pos = (Vector2){
            gameSettings.scaledPlaygridActiveIndicatorWidth +
            gameSettings.scaledPlaygridBorderWidth +
            i * gameSettings.scaledPlaygridColumnDividerWidth +
            i * gameSettings.scaledPlaygridColumnWidth,
            0.0};
        playgrid->columns[i].centerX = 
            playgrid->columns[i].pos.x +
            (gameSettings.scaledPlaygridColumnWidth / 2.0);
    }
}

static void PositionGrid(wly_Playgrid *playgrid, int playgridIndex){
    float xPos = 
        playgridIndex * gameSettings.scaledPlaygridWidth +
        playgridIndex * WLY_GAME_PlaygridDividerWidth;
        
    float widthExcludingSideBorders = 
        gameSettings.scaledPlaygridWidth * gameSettings.playgridCount +
        (gameSettings.playgridCount - 1) * WLY_GAME_PlaygridDividerWidth;
        
    xPos += (splitslib_d_screen.width - widthExcludingSideBorders) / 2.0;
    
    playgrid->pos = (Vector2){
        xPos,
        (splitslib_d_screen.height - WLY_PLAYGRID_Height) / 2.0
    };
}

static void ResetAvatarIndexArrays(wly_Playgrid *pgPtr){
    UpdateAvatarIndexArray_LeftToRight(pgPtr);
}

static void ResetAvatars(wly_Playgrid *playgrid){
    wly_PlayerAvatar *paPtr;
    
    for (int i = 0; i < gameSettings.avatarCount; i++){
        paPtr = &(playgrid->avatars[i]);

        paPtr->columnIndex = -1;
        paPtr->eatFlashTweenId = -1;
        paPtr->growCounter = 0.0f;
        paPtr->growTarget = 0.0f;
        paPtr->index = i;    
        paPtr->isActive = true;
        paPtr->isGrowing = false;
        paPtr->isMoving = false;        
        paPtr->offset = 0;
        paPtr->pos = (Vector2){0.0, 0.0};
        paPtr->state = WLY_AVATARSTATE_Decaying;
        paPtr->width = gameSettings.scaledAvatarWidth;
        
        switch(gameSettings.mode){
        case WLY_GAME_MODE_Classic:
            paPtr->height = WLY_CLASSIC_AVATAR_StartHeight;
            paPtr->rgbValue = 
                WLY_CLASSIC_AVATAREATFLASH_SourceRGBValue;
            break;
        case WLY_GAME_MODE_Peaceful:
            paPtr->height = WLY_PEACEFUL_AVATAR_StartHeight;
            paPtr->rgbValue = 
                WLY_PEACEFUL_AVATAREATFLASH_SourceRGBValue;
            break;
        case WLY_GAME_MODE_Stressful:
            paPtr->height = WLY_STRESSFUL_AVATAR_StartHeight;
            paPtr->rgbValue = 
                WLY_STRESSFUL_AVATAREATFLASH_SourceRGBValue;
            break;
        }
    } 
}

static void ResetColumns(wly_Playgrid *playgrid){
    for (int i = 0; i < gameSettings.columnCount; i++){
        playgrid->columns[i].index = i;
        playgrid->columns[i].pos = (Vector2){0.0, 0.0};
        playgrid->columns[i].centerX = 0.0;
        playgrid->columns[i].isActive = true;
        playgrid->columns[i].isEmpty = true;
        playgrid->columns[i].playerAvatarIndex = -1;
    }
}

static void ResetGame(){
    for (int i = 0; i < gameSettings.playgridCount; i++){
        ResetPlaygrid(&playgrids[i], i);
        PelletManager_Reset(&energyPMs[i]);
        PelletManager_Reset(&enemyPMs[i]);
    }
    
    activePlaygridIndex = 0;
    
    ResetScoreboard();
    UpdateScoreboardViewToGameplayMode();
}

static void ResetMove(wly_Playgrid *playgrid){
    playgrid->move.state = WLY_MOVESTATE_NotMoving;
    playgrid->move.sourceAvatarIndex = -1;
    playgrid->move.targetAvatarIndex = -1;
    playgrid->move.sourceColumnIndex = -1;
    playgrid->move.targetColumnIndex = -1;
    playgrid->move.sourcePos = (Vector2){0.0, 0.0};
    playgrid->move.targetPos = (Vector2){0.0, 0.0};
    playgrid->move.timer = 0.0f;
}

static void ResetMoveTimer(wly_PlayerMove *move){
    move->timer = 0.0f;
}

static void ResetPlaygrid(wly_Playgrid *pgPtr, int pgIndex){
    SetupStartPosition(pgPtr);
    PositionGrid(pgPtr, pgIndex);
    ResetAvatarIndexArrays(pgPtr);
    pgPtr->controlScheme_ColumnSwap_IsSourceColumnSet = false;
    pgPtr->height = WLY_PLAYGRID_Height;
    pgPtr->pgMovement_Dir = (0 == pgIndex % 2) ? 1 : -1;
    pgPtr->pgMovement_Speed = gameSettings.speedPlaygridMove;
    pgPtr->pgFlip_State = 1;
    pgPtr->score = 0;
}

static void ResetScoreboard(){
    scoreboard.current = 0;
}

static void RotatePlaygrid(wly_Playgrid *pgPtr){
    if (gameSettings.shouldPlaygridsFlip){
        splitslib_a_PlayCustomSound(
            WLY_PROJECTAUDIO_SFX_GridFlip
        );
        pgPtr->pgFlip_State *= -1;
    }
}

static void SaveNewHighScoreIfApplicable(){
    if (scoreboard.high == scoreboard.current){
        wly_highScores
            [wly_highScoreIndices[0]]
            [wly_highScoreIndices[1]]
        = scoreboard.high;
        
        wly_SaveHighScores();
    }
}

static void SetTargetColumn(wly_Playgrid *pgPtr, int tcIndex){
    pgPtr->move.targetColumnIndex = tcIndex;
    pgPtr->move.targetPos = (Vector2){
        pgPtr->columns[tcIndex].centerX - WLY_AVATAR_Width / 2,
        pgPtr->move.sourcePos.y};
}

static void SetupStartPosition(wly_Playgrid *playgrid){
    ResetColumns(playgrid);
    ResetAvatars(playgrid);
    ResetMove(playgrid);
    
    PositionColumns(playgrid);
    PositionAvatars(playgrid);
}

static void TargetThisColumnToTheLeft(wly_Playgrid *playgrid){
    for (int i = playgrid->move.sourceColumnIndex -1; i > -1; i--){
        if (ColumnTargeted(i, playgrid)) break;
    }
}

static void TargetThisColumnToTheRight(wly_Playgrid *playgrid){
    for (int i = playgrid->move.sourceColumnIndex + 1; i < gameSettings.columnCount; i++){
        if (ColumnTargeted(i, playgrid)) break;
    }
}

static bool TheAnimationIsDone(wly_Playgrid *pgPtr){
   wly_PlayerAvatar *paPtr = &(pgPtr->avatars[pgPtr->move.sourceAvatarIndex]);
   
    if (
        pgPtr->move.targetPos.x > pgPtr->move.sourcePos.x &&
        paPtr->pos.x > pgPtr->move.targetPos.x
    ){
        return true;
    }
    else if (
        pgPtr->move.targetPos.x < pgPtr->move.sourcePos.x &&
        paPtr->pos.x < pgPtr->move.targetPos.x
    ){
        return true;
    }
    
    return false;
}

static bool ThereIsACollisionBetweenAnAvatarAndAPellet
(wly_PlayerAvatar *paPtr, wly_Pellet *pPtr)
{
    float pa_topY = paPtr->pos.y - paPtr->height;
    float pa_rightX = paPtr->pos.x + WLY_AVATAR_Width;
    
    if (splitslib_ps_PulseSquare_Get_BottomY(&(pPtr->ps)) < pa_topY) 
        return false;
    
    if (splitslib_ps_PulseSquare_Get_RightX(&(pPtr->ps)) < paPtr->pos.x)
        return false;
    
    if (splitslib_ps_PulseSquare_Get_LeftX(&(pPtr->ps)) > pa_rightX)
        return false;
    
    return true;
}

static bool ThereIsAnEmptyColumnToTheLeft(wly_Playgrid *playgrid){
    for (int i = playgrid->move.sourceColumnIndex - 1; i > -1; i--){
        if (playgrid->columns[i].isActive && playgrid->columns[i].isEmpty)
            return true;
    }
    return false;
}

static bool ThereIsAnEmptyColumnToTheRight(wly_Playgrid *playgrid){
    for (int i = playgrid->move.sourceColumnIndex + 1; i < gameSettings.columnCount; i++){
        if (playgrid->columns[i].isActive && playgrid->columns[i].isEmpty)
            return true;
    }
    return false;
}

static void TryToStartMovingLeft(wly_PlayerMove *movePtr, wly_Playgrid *pgPtr){
    NoteDownStartingColumnAndPosition(pgPtr);
    ResetMoveTimer(movePtr);
    if (ThereIsAnEmptyColumnToTheLeft(pgPtr)){
        if (gameSettings.controlScheme == WLY_GAME_CONTROL_SCHEME_Classic){
            splitslib_a_PlayCustomSound(
                WLY_PROJECTAUDIO_SFX_ClassicAvatarSuccessMove
            );
        }
        else if (gameSettings.controlScheme == WLY_GAME_CONTROL_SCHEME_WebForm){
            splitslib_a_PlayCustomSound(
                WLY_PROJECTAUDIO_SFX_WebSuccessMove
            );
        }
        TargetThisColumnToTheLeft(pgPtr);
        movePtr->state = WLY_MOVESTATE_MoveInProgress;
    }
    else {
        if (gameSettings.controlScheme == WLY_GAME_CONTROL_SCHEME_Classic){
            splitslib_a_PlayCustomSound(
                WLY_PROJECTAUDIO_SFX_ClassicAvatarFailedMove
            );
        }
        else if (gameSettings.controlScheme == WLY_GAME_CONTROL_SCHEME_WebForm){
            splitslib_a_PlayCustomSound(
                WLY_PROJECTAUDIO_SFX_WebFailedMove
            );
        }
       movePtr->state = WLY_MOVESTATE_FailedMoveAttemptInProgress;
    }
}

static void TryToStartMovingRight(wly_PlayerMove *movePtr, wly_Playgrid *pgPtr){
    NoteDownStartingColumnAndPosition(pgPtr);
    ResetMoveTimer(movePtr);
    if (ThereIsAnEmptyColumnToTheRight(pgPtr)){
        if (gameSettings.controlScheme == WLY_GAME_CONTROL_SCHEME_Classic){
            splitslib_a_PlayCustomSound(
                WLY_PROJECTAUDIO_SFX_ClassicAvatarSuccessMove
            );
        }
        else if (gameSettings.controlScheme == WLY_GAME_CONTROL_SCHEME_WebForm){
            splitslib_a_PlayCustomSound(
                WLY_PROJECTAUDIO_SFX_WebSuccessMove
            );
        }
        TargetThisColumnToTheRight(pgPtr);
        movePtr->state = WLY_MOVESTATE_MoveInProgress;
    }
    else {
        if (gameSettings.controlScheme == WLY_GAME_CONTROL_SCHEME_Classic){
            splitslib_a_PlayCustomSound(
                WLY_PROJECTAUDIO_SFX_ClassicAvatarFailedMove
            );
        }
        else if (gameSettings.controlScheme == WLY_GAME_CONTROL_SCHEME_WebForm){
            splitslib_a_PlayCustomSound(
                WLY_PROJECTAUDIO_SFX_WebFailedMove
            );
        }
        movePtr->state = WLY_MOVESTATE_FailedMoveAttemptInProgress;
    }
}

static void UnloadRenderTextures(){
    UnloadRenderTexture(rtexture_menuRibbon);
    UnloadRenderTexture(rtexture_menuRibbonFlipped);
}

static void UpdateAvatar(wly_PlayerAvatar *paPtr){
    if (-1 != paPtr->eatFlashTweenId){
        splitslib_anim_Tween(paPtr->eatFlashTweenId);
        paPtr->rgbValue = splitslib_anim_GetValue(paPtr->eatFlashTweenId);
        if (splitslib_anim_HasLooped(paPtr->eatFlashTweenId)){
            splitslib_anim_Destroy(paPtr->eatFlashTweenId);
            paPtr->eatFlashTweenId = -1;
        }
    }
    
    float growAmount;
    
    if (
        gameSettings.mode != WLY_GAME_MODE_Stressful 
       // && WLY_AVATARSTATE_Growing != paPtr->state
    ){
        paPtr->height -= 
            frameTime * 
            gameSettings.speedAvatarDecayPerSecond;
        if (paPtr->height <= 0){
            splitslib_a_PlayCustomSound(WLY_PROJECTAUDIO_SFX_BarDecayGameOver);
            paPtr->state = WLY_AVATARSTATE_OutOfLife;
        }
    }
    
    switch(paPtr->state){
    case WLY_AVATARSTATE_Dead:
        // do nothing...
        break;

    case WLY_AVATARSTATE_Growing:
        growAmount = 
            frameTime * 
            gameSettings.speedAvatarGrowPerSecond;
        paPtr->height += growAmount;
        paPtr->growCounter += growAmount;
        if (paPtr->growCounter >= paPtr->growTarget){
            paPtr->growCounter = 0.0f;
            paPtr->growTarget = 0.0f;
            paPtr->state = WLY_AVATARSTATE_Decaying;
        }
        break;
    }
}

static void UpdateAvatars(wly_Playgrid *pgPtr){
    for (int i = 0; i < gameSettings.avatarCount; i++){
        UpdateAvatar(&(pgPtr->avatars[i]));
    }
}

static void UpdateAvatarIndexArray_LeftToRight(wly_Playgrid *pgPtr){
    int cursor = 0;
    for (int i = 0; i < gameSettings.columnCount; i++){
        if (pgPtr->columns[i].isEmpty) continue;
        
        pgPtr->avatarIndices_LeftToRight[cursor] =
            pgPtr->columns[i].playerAvatarIndex;
        cursor++;
    } 
}

static void UpdatePlaygrid(wly_Playgrid *pgPtr, int pgIndex){
    UpdateAvatars(pgPtr);
    MovePlaygrid(pgPtr);
    switch(gameSettings.mode){
    case WLY_GAME_MODE_Classic:
        PelletManager_Update(&(enemyPMs[pgIndex]));
        DetectAndHandleCollisionsBetweenAvatarsAndPellets(&enemyPMs[pgIndex], pgIndex);
        PelletManager_Update(&(energyPMs[pgIndex]));
        DetectAndHandleCollisionsBetweenAvatarsAndPellets(&energyPMs[pgIndex], pgIndex);
        break;
        
    case WLY_GAME_MODE_Peaceful:
        PelletManager_Update(&(energyPMs[pgIndex]));
        DetectAndHandleCollisionsBetweenAvatarsAndPellets(&energyPMs[pgIndex], pgIndex);
        break;
        
    case WLY_GAME_MODE_Stressful:
        PelletManager_Update(&(enemyPMs[pgIndex]));
        DetectAndHandleCollisionsBetweenAvatarsAndPellets(&enemyPMs[pgIndex], pgIndex);
        break;
    
    default:
        PelletManager_Update(&(enemyPMs[pgIndex]));
        DetectAndHandleCollisionsBetweenAvatarsAndPellets(&enemyPMs[pgIndex], pgIndex);
        PelletManager_Update(&(energyPMs[pgIndex]));
        DetectAndHandleCollisionsBetweenAvatarsAndPellets(&energyPMs[pgIndex], pgIndex);
    }
}

static void UpdatePlaygrids(){
    for (int i = 0; i < gameSettings.playgridCount; i++){
        UpdatePlaygrid(&(playgrids[i]), i);
    }
}

static void UpdateScoreboardViewToGameplayMode(){
    splitslib_sf_SetText(
        scoreboard.view_current.id,
        FormatText("%d", scoreboard.current)
    );
    splitslib_sf_SetSize(
        scoreboard.view_current.id,
        WLY_CurrentScoreStringboxTextSize
    );
    splitslib_sf_SetStrokeWeight(
        scoreboard.view_current.id,
        WLY_CurrentScoreStringboxTextWeight
    );
    splitslib_sf_SetKern(
        scoreboard.view_current.id,
        WLY_CurrentScoreStringboxTextKern
    );
    splitslib_sf_SetStringboxPosition(
        &scoreboard.view_current,
        WLY_CurrentScoreView_GameplayModePos
    );

    splitslib_sf_SetText(
        scoreboard.view_high.id,
        FormatText("HS: %d", scoreboard.high)
    );
    splitslib_sf_SetSize(
        scoreboard.view_high.id,
        WLY_HighScoreStringboxTextSize
    );
    splitslib_sf_SetStrokeWeight(
        scoreboard.view_high.id,
        WLY_HighScoreStringboxTextWeight
    );
    splitslib_sf_SetKern(
        scoreboard.view_high.id,
        WLY_HighScoreStringboxTextKern
    );
    splitslib_sf_SetStringboxPosition(
        &scoreboard.view_high,
        WLY_HighScoreView_GameplayModePos
    );
    
    
    splitslib_sf_SetStringboxPosition(
        &scoreboard.view_multiplier,
        WLY_ScoreMultiplierView_GameplayModePos
    );
}

static void UpdateScoreboardViewToMenuMode(){
    splitslib_sf_Result result;
    float stringWidth;

    splitslib_sf_SetSize(
        scoreboard.view_current.id,
        WLY_MenuView_CurrentScoreStringboxTextSize
    );
    splitslib_sf_SetStrokeWeight(
        scoreboard.view_current.id,
        WLY_MenuView_CurrentScoreStringboxTextWeight
    );
    splitslib_sf_SetKern(
        scoreboard.view_current.id,
        WLY_MenuView_CurrentScoreStringboxTextKern
    );
    result = splitslib_sf_GetWidth(scoreboard.view_current.id);
    stringWidth = result.floatValue;
    splitslib_sf_SetStringboxPosition(
        &scoreboard.view_current,
        (Vector2){
            splitslib_d_screen.center.x - 
            stringWidth / 2.0,
            WLY_MENURIBBON_HEIGHT * 0.3
        }
    );
    
    if (scoreboard.high == scoreboard.current){
        splitslib_sf_SetText(
            scoreboard.view_high.id,
            "NEW HIGH SCORE"
        );
    }
    splitslib_sf_SetSize(
        scoreboard.view_high.id,
        WLY_MenuView_HighScoreStringboxTextSize
    );
    splitslib_sf_SetStrokeWeight(
        scoreboard.view_high.id,
        WLY_MenuView_HighScoreStringboxTextWeight
    );
    splitslib_sf_SetKern(
        scoreboard.view_high.id,
        WLY_MenuView_HighScoreStringboxTextKern
    );
    result = splitslib_sf_GetWidth(scoreboard.view_high.id);
    stringWidth = result.floatValue;
    splitslib_sf_SetStringboxPosition(
        &scoreboard.view_high,
        (Vector2){
            splitslib_d_screen.center.x -
            stringWidth / 2,
            WLY_MENURIBBON_HEIGHT * 0.6
        }
    );
    
    splitslib_sf_SetStringboxPosition(
        &scoreboard.view_multiplier,
        WLY_OFFSCREEN
    );
}

static void Utils_SwapSort(int *arr, int len, int *matchingArr){
    while(Utils_SwapSort_IsNotSorted(arr, len)){
        for (int i = len - 1; i > 0; i--){
            if (arr[i] <= arr[i-1]) continue;
            
            int temp = arr[i-1];
            arr[i-1] = arr[i];
            arr[i] = temp;
            
            temp = matchingArr[i-1];
            matchingArr[i-1] = matchingArr[i];
            matchingArr[i] = temp;
        }
    }
}

static bool Utils_SwapSort_IsNotSorted(int *arr, int len){
    for (int i = len - 1; i > 0; i--){
        if (arr[i] > arr[i-1]) return true;
    }
    return false;
}