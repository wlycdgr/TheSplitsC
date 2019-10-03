#ifndef wly_Gameplay_EnergyPelletManager_H
#define wly_Gameplay_EnergyPelletManager_H

#include "raylib.h"
#include "../../splitslib/splitslib_ps_PulseSystem.h"
#include "GameplayMixed.h"

#define WLY_PELLETMANAGER_POOLSIZE 32

#define WLY_ENERGYPELLET_ENERGYVALUE 90
#define WLY_ENERGYPELLET_COLOR DARKGRAY
#define WLY_ENERGYPELLET_PULSECOLOR DARKGRAY
#define WLY_ENERGYPELLET_VSPEED_PERSEC 150.0f
#define WLY_ENERGYPELLET_SIZE_MIN 24
#define WLY_ENERGYPELLET_SIZE_MAX 24
#define WLY_ENERGYPELLET_SPAWNDELAY_SEC 1.0f //2.0F

#define WLY_ENEMYPELLET_COLOR MAGENTA
#define WLY_ENEMYPELLET_PULSECOLOR MAGENTA
#define WLY_ENEMYPELLET_VSPEED_PERSEC  200.0F
#define WLY_ENEMYPELLET_SIZE_MIN 16
#define WLY_ENEMYPELLET_SIZE_MAX 16
//#define WLY_CLASSIC_ColumnCounts {6, 7, 8, 9}
#define WLY_CLASSIC_SecondsBetweenEnemySpawns 1.0f
#define WLY_STRESSFUL_SecondsBetweenEnemySpawns 0.2f

typedef enum WLY_PELLET_TYPE{
    WLY_PELLET_TYPE_Energy = 0,
    WLY_PELLET_TYPE_Enemy
} WLY_PELLET_TYPE;

typedef enum WLY_PELLET_STATE {
    WLY_PELLET_STATE_BeingEaten = 0,
    WLY_PELLET_STATE_Dying,
    WLY_PELLET_STATE_JustFinishedBeingEaten,
    WLY_PELLET_STATE_Killing,
    WLY_PELLET_STATE_Inactive,
    WLY_PELLET_STATE_Moving,
} WLY_PELLET_STATE;

typedef struct wly_Pellet {
    int columnIndex;
    WLY_PELLET_STATE state;
    splitslib_ps_PulseSquare ps;
} wly_Pellet;

typedef struct wly_PelletManager{
    wly_Pellet pelletPool[WLY_PELLETMANAGER_POOLSIZE];
    float spawnTimer;
    float spawnDelay;
    float vSpeed;
    WLY_PELLET_TYPE type;
} wly_PelletManager;

void PelletManager_DeactivatePellet(wly_Pellet *p);
void PelletManager_Draw(wly_PelletManager *pm, wly_Playgrid *pgPtr);
void PelletManager_Init(wly_PelletManager *pm, WLY_PELLET_TYPE type);
void PelletManager_Reset(wly_PelletManager *pm);
void PelletManager_Update(wly_PelletManager *pm);

#endif