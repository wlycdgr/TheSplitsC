#include <stdlib.h>

#include "raylib.h"
#include "../../splitslib/splitslib_a_Audio.h"
#include "../../splitslib/splitslib_d_Display.h"
#include "GameplayMixed.h"
#include "Gameplay_PelletManager.h"
#include "GameSettings.h"
#include "ProjectAudio.h"

static void AnimateActivePellets(wly_PelletManager *pm);
static void DeactivatePellet(wly_PelletManager *pm, int i);
static void DeactivatePelletsThatHaveGoneOffTheBottom(wly_PelletManager *pm);
static void MoveActivePellets(wly_PelletManager *pm);
static void SpawnNewPellet(wly_PelletManager *pm);

float frameTime;

void PelletManager_DeactivatePellet(wly_Pellet *p){
    p->columnIndex = -1;
    p->state = WLY_PELLET_STATE_Inactive;
}

void PelletManager_Draw(wly_PelletManager *pm, wly_Playgrid *pgPtr){
    float offsetX = pgPtr->pos.x;
    float offsetY = pgPtr->pos.y;
    
    for (int i = 0; i < WLY_PELLETMANAGER_POOLSIZE; i++){
        switch(pm->pelletPool[i].state){
        case WLY_PELLET_STATE_BeingEaten:
        case WLY_PELLET_STATE_Killing:
        case WLY_PELLET_STATE_Moving:
            if (-1 == pgPtr->pgFlip_State){
                offsetX = 
                    pgPtr->pos.x + 
                    gameSettings.scaledPlaygridWidth - 
                    (pm->pelletPool[i].ps.base.pos.x * 2);
                offsetY = 
                    pgPtr->pos.y + 
                    pgPtr->height - (pm->pelletPool[i].ps.base.pos.y * 2); 
            }
            splitslib_ps_PulseSquare_Draw(
                &(pm->pelletPool[i].ps), 
                offsetX,
                offsetY
            );
            break;
        }
    }
}

void PelletManager_Init(wly_PelletManager *pm, WLY_PELLET_TYPE type)
{       
    switch(type){
    case WLY_PELLET_TYPE_Energy:
        pm->spawnDelay = gameSettings.rateEnergySpawn;
        pm->vSpeed = gameSettings.speedEnergy;
        break;
    case WLY_PELLET_TYPE_Enemy:
        pm->spawnDelay = gameSettings.rateEnemySpawn;
        pm->vSpeed = gameSettings.speedEnemy;
        break;
    }
    
    pm->type = type;
    
    for (int i = 0; i < WLY_PELLETMANAGER_POOLSIZE; i++){
        splitslib_ps_PulseSquare *ps = &(pm->pelletPool[i].ps);
        
        splitslib_ps_PulseSquare_Init(ps);
        
        switch(type){
        case WLY_PELLET_TYPE_Energy:
            splitslib_ps_PulseSquare_SetColor(ps, WLY_ENERGYPELLET_COLOR);
            splitslib_ps_PulseSquare_SetPulseColor(ps, WLY_ENERGYPELLET_PULSECOLOR);
            splitslib_ps_PulseSquare_SetSize(
                ps, 
                gameSettings.scale * WLY_ENERGYPELLET_SIZE_MIN, 
                gameSettings.scale * WLY_ENERGYPELLET_SIZE_MAX);
            break;
        case WLY_PELLET_TYPE_Enemy:
            splitslib_ps_PulseSquare_SetColor(ps, WLY_ENEMYPELLET_COLOR);
            splitslib_ps_PulseSquare_SetPulseColor(ps, WLY_ENEMYPELLET_PULSECOLOR);
            splitslib_ps_PulseSquare_SetSize(
                ps, 
                gameSettings.scale * WLY_ENEMYPELLET_SIZE_MIN, 
                gameSettings.scale * WLY_ENEMYPELLET_SIZE_MAX);
            break;
        }
    }
    
    PelletManager_Reset(pm);
}

void PelletManager_Reset(wly_PelletManager *pm){
    pm->spawnTimer = 0.0f;
    for (int i = 0; i < WLY_PELLETMANAGER_POOLSIZE; i++){
        PelletManager_DeactivatePellet(&(pm->pelletPool[i]));
    }
}

void PelletManager_Update(wly_PelletManager *pm){
    frameTime = GetFrameTime();
    
    MoveActivePellets(pm);
    
    AnimateActivePellets(pm);
    
    pm->spawnTimer += frameTime;
    if (pm->spawnTimer > pm->spawnDelay){
        pm->spawnTimer = 0.0f;
        SpawnNewPellet(pm);
        if (WLY_PELLET_TYPE_Energy == pm->type){
            splitslib_a_PlayCustomSound(
                WLY_PROJECTAUDIO_SFX_EnergyAppear
            );
        }
        else if (WLY_PELLET_TYPE_Enemy == pm->type){
            splitslib_a_PlayCustomSound(
                WLY_PROJECTAUDIO_SFX_PinkEnemyAppears
            );
        }
    }
    
    DeactivatePelletsThatHaveGoneOffTheBottom(pm);
}

// PRIVATE FUNCTIONS
static void AnimateActivePellets(wly_PelletManager *pm){
    for (int i = 0; i < WLY_PELLETMANAGER_POOLSIZE; i++){
        if (WLY_PELLET_STATE_Inactive == pm->pelletPool[i].state) continue;        
        splitslib_ps_PulseSquare_Animate(&(pm->pelletPool[i].ps));
    }
}

static void DeactivatePelletsThatHaveGoneOffTheBottom(wly_PelletManager *pm){
    for (int i = 0; i < WLY_PELLETMANAGER_POOLSIZE; i++){
        if (
            WLY_PELLET_STATE_Inactive != pm->pelletPool[i].state && 
            splitslib_ps_PulseSquare_Get_TopY(&(pm->pelletPool[i].ps)) > 
            WLY_PLAYGRID_Height
        ){
            PelletManager_DeactivatePellet(&(pm->pelletPool[i]));
            
            if (gameSettings.mode == WLY_GAME_MODE_Stressful){
                wly_UpdateScoreboard(1);
            }
            
            if (WLY_PELLET_TYPE_Energy == pm->type){
                splitslib_a_PlayCustomSound(
                    WLY_PROJECTAUDIO_SFX_EnergyHitsBottom
                );
            }
            else if (WLY_PELLET_TYPE_Enemy == pm->type){
                if (WLY_GAME_MODE_Stressful == gameSettings.mode){
                    splitslib_a_PlayCustomSound(
                        WLY_PROJECTAUDIO_SFX_ScoreEnemyBottom
                    );
                }
                else {
                    splitslib_a_PlayCustomSound(
                        WLY_PROJECTAUDIO_SFX_NoScoreEnemyBottom
                    );
                }
            }
        }
    }
}

static void MoveActivePellets(wly_PelletManager *pm){
    float moveAmount = frameTime * pm->vSpeed;
    for (int i = 0; i < WLY_PELLETMANAGER_POOLSIZE; i++){
        switch(pm->pelletPool[i].state){
        case WLY_PELLET_STATE_BeingEaten:
        case WLY_PELLET_STATE_Moving:
        case WLY_PELLET_STATE_Killing:
            splitslib_ps_PulseSquare_MoveBy(&(pm->pelletPool[i].ps), 0.0f, moveAmount);
            break;
        }
    }
}

static void SpawnNewPellet(wly_PelletManager *pm){
    int newPelletId = -1;
    for (int i = 0; i < WLY_PELLETMANAGER_POOLSIZE; i++){
        if (WLY_PELLET_STATE_Inactive == pm->pelletPool[i].state){
            newPelletId = i;
            break;
        }
    }
    if (-1 == newPelletId) return;
    
    wly_Pellet *pPtr = 
        &(pm->pelletPool[newPelletId]);
        
    pPtr->state = WLY_PELLET_STATE_Moving;
    pPtr->columnIndex = rand() % gameSettings.columnCount;
    splitslib_ps_PulseSquare_MoveTo(
        &(pPtr->ps),
        gameSettings.scaledPlaygridActiveIndicatorWidth +
        gameSettings.scaledPlaygridBorderWidth + 
        (gameSettings.scaledPlaygridColumnWidth / 2.0) + 
        (pPtr->columnIndex * gameSettings.scaledPlaygridColumnWidth) +
        (pPtr->columnIndex * gameSettings.scaledPlaygridColumnDividerWidth),
        0.0f);
}