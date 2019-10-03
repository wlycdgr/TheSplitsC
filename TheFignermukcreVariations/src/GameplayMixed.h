#ifndef wly_Gameplay_Mixed_H
#define wly_Gameplay_Mixed_H

#include "HighScoreManager.h"
#include "StateManager.h"

#define WLY_OFFSCREEN (Vector2){-1000.0f, -1000.0f}

#define WLY_INPUT_MOVELEFT_ONE KEY_Q
#define WLY_INPUT_MOVELEFT_TWO KEY_W
#define WLY_INPUT_MOVELEFT_THREE KEY_E
#define WLY_INPUT_MOVELEFT_FOUR KEY_R
#define WLY_INPUT_MOVELEFT_FIVE KEY_T

#define WLY_INPUT_MOVERIGHT_ONE KEY_P
#define WLY_INPUT_MOVERIGHT_TWO KEY_O
#define WLY_INPUT_MOVERIGHT_THREE KEY_I
#define WLY_INPUT_MOVERIGHT_FOUR KEY_U
#define WLY_INPUT_MOVERIGHT_FIVE KEY_Y

#define WLY_INPUT_ColumnSwap_LEFTMOST KEY_Q
#define WLY_INPUT_ColumnSwap_SECONDFROMLEFT KEY_W
#define WLY_INPUT_ColumnSwap_THIRDFROMLEFT KEY_E
#define WLY_INPUT_ColumnSwap_FOURTHFROMLEFT KEY_R
#define WLY_INPUT_ColumnSwap_FIFTHFROMLEFT KEY_T
#define WLY_INPUT_ColumnSwap_SIXTHFROMLEFT KEY_Y
#define WLY_INPUT_ColumnSwap_SEVENTHFROMLEFT KEY_U
#define WLY_INPUT_ColumnSwap_EIGHTHFROMLEFT KEY_I
#define WLY_INPUT_ColumnSwap_NINTHFROMLEFT KEY_O
#define WLY_INPUT_ColumnSwap_TENTHFROMLEFT KEY_P

#define WLY_INPUT_WEBFORM_MOVELEFT KEY_G
#define WLY_INPUT_WEBFORM_MOVERIGHT KEY_H
#define WLY_INPUT_WEBFORM_SELECTNEXT KEY_Q

#define WLY_AVATAREATFLASH_SourceRGBValue 150
#define WLY_AVATAREATFLASH_TargetRGBValue 50
#define WLY_AVATAREATFLASH_SecondsToTarget 0.05f
#define WLY_AVATAREATFLASH_SecondsAtTarget 0.05f
#define WLY_AVATAREATFLASH_SecondsToSource 2.0f

#define WLY_AVATAR_COLOR CLITERAL{200, 200, 200, 255} // raylib LIGHTGRAY
#define WLY_CLASSIC_AVATAR_StartHeight 240
#define WLY_CLASSIC_AVATAREATFLASH_SourceRGBValue 150

#define WLY_PEACEFUL_AVATAR_StartHeight 320
#define WLY_PEACEFUL_AVATAREATFLASH_SourceRGBValue 150

#define WLY_STRESSFUL_AVATAR_StartHeight 20
#define WLY_STRESSFUL_AVATAREATFLASH_SourceRGBValue 50

#define WLY_AVATAR_Width 32

#define WLY_AVATAR_DangerIndicatorColor CLITERAL{255, 0, 255, 64} // semi-translucent magenta
#define WLY_AVATAR_DangerIndicatorThreshold (0.1 * WLY_PLAYGRID_Height)
#define WLY_AVATAR_DecayPerSecond 30
#define WLY_AVATAR_GrowPerSecond 45 //10 //100 //30
#define WLY_AVATAR_MovePerFrame 0.25//0.1

#define WLY_BG_Color WHITE
#define WLY_GAME_MaxPlaygrids 4
#define WLY_GAME_PlaygridDividerWidth 64
#define WLY_GAME_SideBorderWidth 64
#define WLY_PLAYGRID_Height 600
#define WLY_PLAYGRID_MaxAvatars 5
#define WLY_PLAYGRID_MaxColumns 10
#define WLY_PLAYGRID_MovementSpeed_Persec 100.0f
#define WLY_PLAYGRID_TotalDecayPerSecond 60.0f
#define WLY_PLAYGRID_ACTIVE_INDICATOR_Color ORANGE
#define WLY_PLAYGRID_ACTIVE_INDICATOR_Width 16
#define WLY_PLAYGRID_BORDER_Color BLACK
#define WLY_PLAYGRID_BORDER_Width 4
#define WLY_PLAYGRID_COLUMN_Width 48
#define WLY_PLAYGRID_COLUMN_DIVIDER_Color BLACK
#define WLY_PLAYGRID_COLUMN_DIVIDER_Width 1

typedef enum WLY_AVATARSTATE {
    WLY_AVATARSTATE_BeingKilled = 0,
    WLY_AVATARSTATE_Dead,
    WLY_AVATARSTATE_Decaying,
    WLY_AVATARSTATE_Growing,
    WLY_AVATARSTATE_JustGotHitByEnemy,
    WLY_AVATARSTATE_OutOfLife
} WLY_AVATARSTATE;

typedef enum WLY_MOVESTATE {
    WLY_MOVESTATE_ColumnSwapInProgress = 0,
    WLY_MOVESTATE_NotMoving,
    WLY_MOVESTATE_FailedMoveAttemptInProgress,
    WLY_MOVESTATE_MoveInProgress
} WLY_MOVESTATE;

typedef struct wly_PlayerAvatar {
    int offset;
    int index;
    Vector2 pos;
    int columnIndex;
    bool isActive;
    bool isGrowing;
    bool isMoving;
    float growCounter;
    float growTarget;
    float height;
    float width;
    WLY_AVATARSTATE state;
    int eatFlashTweenId;
    float rgbValue;
} wly_PlayerAvatar;

typedef struct wly_Column{
    int index;
    float centerX;
    Vector2 pos;
    bool isActive;
    bool isEmpty;
    int playerAvatarIndex;
} wly_Column;

typedef struct wly_PlayerMove {
    WLY_MOVESTATE state;
    int sourceAvatarIndex;
    int targetAvatarIndex;
    int sourceColumnIndex;
    int targetColumnIndex;
    Vector2 sourcePos;
    Vector2 targetPos;
    float timer;
} wly_PlayerMove;

typedef struct wly_Playgrid {
    int avatarIndices_LeftToRight[WLY_PLAYGRID_MaxAvatars];
    wly_PlayerAvatar avatars[WLY_PLAYGRID_MaxAvatars];
    bool controlScheme_ColumnSwap_IsSourceColumnSet;
    int controlScheme_Fixed_SelectedAvatarIndex;
    wly_Column columns[WLY_PLAYGRID_MaxColumns];
    float height;
    wly_PlayerMove move;
    int pgMovement_Dir;
    float pgMovement_Speed;
    int pgFlip_State;
    Vector2 pos;
    int score;
} wly_Playgrid;

typedef struct wly_ScaledWidths{
    float activeIndicator;
    float columnDivider;
} wly_ScaledWidths;

void wly_GameplayMixed_Init(wly_GameSettings settings);
void wly_GameplayMixed_Update();
void wly_GameplayMixed_Draw();

#endif