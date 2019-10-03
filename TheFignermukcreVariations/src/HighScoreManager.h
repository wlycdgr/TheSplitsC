#ifndef wly_HighScoreManager_H
#define wly_HighScoreManager_H

#include "raylib.h"
#include "../../splitslib/splitslib_sf_Splitsfont.h"

#define WLY_CurrentScoreView_GameplayModePos (Vector2){25.0, 25.0}
#define WLY_ScoreMultiplierView_GameplayModePos (Vector2){25.0, 150.0}
#define WLY_HighScoreView_GameplayModePos (Vector2){25.0, 75.0}

#define WLY_CurrentScoreStringboxStartText "SCORE: 0"
#define WLY_CurrentScoreStringboxTextSize 0.05
#define WLY_CurrentScoreStringboxTextWeight 0.01
#define WLY_CurrentScoreStringboxTextKern 0.0125
#define WLY_CurrentScoreStringboxTextY 0.0
#define WLY_MenuView_CurrentScoreStringboxTextSize 0.15
#define WLY_MenuView_CurrentScoreStringboxTextWeight 0.03
#define WLY_MenuView_CurrentScoreStringboxTextKern 0.1 //0.0375

#define WLY_ScoreMultiplierStringboxStartText "MULTIPLIER 1X"
#define WLY_ScoreMultiplierStringboxTextSize 0.05
#define WLY_ScoreMultiplierStringboxTextWeight 0.01
#define WLY_ScoreMultiplierStringboxTextKern 0.0125
#define WLY_ScoreMultiplierStringboxTextY 0.0

#define WLY_HighScoreStringboxStartText "SCORE: 0"
#define WLY_HighScoreStringboxTextSize 0.02
#define WLY_HighScoreStringboxTextWeight 0.0025
#define WLY_HighScoreStringboxTextKern 0.0125
#define WLY_HighScoreStringboxTextY 0.0
#define WLY_MenuView_HighScoreStringboxTextSize 0.02
#define WLY_MenuView_HighScoreStringboxTextWeight 0.0025
#define WLY_MenuView_HighScoreStringboxTextKern 0.0125

#define WLY_HighScoreCap 9999

#define WLY_ControlSchemes 4
#define WLY_GameModes 3
int wly_highScores
    [WLY_ControlSchemes]
    [WLY_GameModes]; 
int wly_highScoreIndices[2];

typedef struct wly_Scoreboard {
    int current;
    int high;
    int scoreMultiplier;
    splitslib_sf_Stringbox view_current;
    splitslib_sf_Stringbox view_high;
    splitslib_sf_Stringbox view_multiplier;
} wly_Scoreboard;
wly_Scoreboard scoreboard;

void wly_LoadHighScores();
void wly_SaveHighScores();
void wly_UpdateScoreboard(int amtToAddToScore);

#endif