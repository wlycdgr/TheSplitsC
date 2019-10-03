#ifndef wly_GameSettings_H
#define wly_GameSettings_H

#define WLY_SPEED_MULTIPLIER_LEVEL_1  1.0
#define WLY_SPEED_MULTIPLIER_LEVEL_2  1.2
#define WLY_SPEED_MULTIPLIER_LEVEL_3  1.44
#define WLY_SPEED_MULTIPLIER_LEVEL_4  1.728
#define WLY_SPEED_MULTIPLIER_LEVEL_5  2.0736
#define WLY_SPEED_MULTIPLIER_LEVEL_6  2.48832
#define WLY_SPEED_MULTIPLIER_LEVEL_7  2.985984
#define WLY_SPEED_MULTIPLIER_LEVEL_8  3.5831808
#define WLY_SPEED_MULTIPLIER_LEVEL_9  4.29981696

float wly_speedMultipliers[9] = {
    WLY_SPEED_MULTIPLIER_LEVEL_1,
    WLY_SPEED_MULTIPLIER_LEVEL_2,
    WLY_SPEED_MULTIPLIER_LEVEL_3,
    WLY_SPEED_MULTIPLIER_LEVEL_4,
    WLY_SPEED_MULTIPLIER_LEVEL_5,
    WLY_SPEED_MULTIPLIER_LEVEL_6,
    WLY_SPEED_MULTIPLIER_LEVEL_7,
    WLY_SPEED_MULTIPLIER_LEVEL_8,
    WLY_SPEED_MULTIPLIER_LEVEL_9
};

typedef enum WLY_GAME_CONTROL_SCHEME {
    WLY_GAME_CONTROL_SCHEME_Classic = 0,
    WLY_GAME_CONTROL_SCHEME_Fixed,
    WLY_GAME_CONTROL_SCHEME_WebForm,
    WLY_GAME_CONTROL_SCHEME_ColumnSwap
} WLY_GAME_CONTROL_SCHEME;

typedef enum WLY_GAME_MODE {
    WLY_GAME_MODE_Classic = 0,
    WLY_GAME_MODE_Peaceful, // energies only, fast shrink
    WLY_GAME_MODE_Stressful, // reds only, no grow/shrink
} WLY_GAME_MODE;

typedef struct wly_GameSettings {
    int avatarCount;
    int columnCount;
    WLY_GAME_CONTROL_SCHEME controlScheme;
    float energyPelletValue;
    int gameSpeed;
    WLY_GAME_MODE mode;
    int playgridCount;
    int playgridWidth;
    float scale;
    float scaledAvatarWidth;
    float scaledPlaygridBorderWidth;
    float scaledPlaygridColumnDividerWidth;
    float scaledPlaygridColumnWidth;
    float scaledPlaygridWidth;
    float scaledPlaygridActiveIndicatorVisibleWidth;
    float scaledPlaygridActiveIndicatorWidth;
    bool shouldPlaygridsMove;
    bool shouldPlaygridsFlip;
    float rateEnemySpawn;
    float rateEnergySpawn;
    float speedAvatarDecayPerSecond;
    float speedAvatarGrowPerSecond;
    float speedAvatarMovePerFrame;
    float speedEnemy;
    float speedEnergy;
    float speedMultiplier;
    float speedPlaygridMove;
} wly_GameSettings;
wly_GameSettings gameSettings;

#endif