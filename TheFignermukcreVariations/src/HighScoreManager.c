#include <stdio.h>
#include <stdlib.h>
#include "../../splitslib/splitslib_a_Audio.h"
#include "HighScoreManager.h"
#include "ProjectAudio.h"

#define WLY_HighScoresFilepath "../data/highscores.txt"

static void LoadDefaultHighScores();
static void LoadNextHighScore(FILE *fPtr);

void wly_LoadHighScores(){    
    LoadDefaultHighScores();
    
    FILE *fPtr = fopen(WLY_HighScoresFilepath, "r+");
    if (NULL == fPtr) {
        wly_SaveHighScores();
        return;
    }

    char c;
    int hackDefenseCounter = 0;
    int hackDefenseMaxLen = 
        WLY_ControlSchemes * 
        WLY_GameModes;
    while ((EOF != (c = fgetc(fPtr))) && hackDefenseCounter < hackDefenseMaxLen){
        LoadNextHighScore(fPtr);
        hackDefenseCounter++;
    }
    
    fclose(fPtr);
    
    for (int i = 0; i < 2; i++){
        wly_highScoreIndices[i] = 0;
    }
}

void wly_SaveHighScores(){
    FILE *fPtr = fopen(WLY_HighScoresFilepath, "w");
    if (NULL == fPtr) return; // TODO -- handle failure more loudly
    
    for (int i = 0; i < WLY_ControlSchemes; i++){
        for (int j = 0; j < WLY_GameModes; j++){
            fprintf(
                fPtr,
                " %d,%d %d\n",
                i, j,
                wly_highScores[i][j]
            );
        }
    }
    
    fclose(fPtr); 
}

void wly_UpdateScoreboard(int amtToAddToScore){
    if (scoreboard.current < WLY_HighScoreCap){
        scoreboard.current += 
            amtToAddToScore * 
            scoreboard.scoreMultiplier;
        if (scoreboard.current > WLY_HighScoreCap)
            scoreboard.current = WLY_HighScoreCap;
    }
    
    splitslib_sf_SetText(
        scoreboard.view_current.id,
        FormatText("%d", scoreboard.current)
    );
    
    // TODO add some kind of JUICE to this probs
    if (scoreboard.current > scoreboard.high){
        splitslib_a_PlayCustomSound(
            WLY_PROJECTAUDIO_SFX_HighScore
        );
        scoreboard.high = scoreboard.current;
        splitslib_sf_SetText(
            scoreboard.view_high.id,
            FormatText("HS: %d", scoreboard.high)
        );
    }
}

static void LoadDefaultHighScores(){
    for (int i = 0; i < WLY_ControlSchemes; i++){
        for (int j = 0; j < WLY_GameModes; j++){
            wly_highScores[i][j] = 0;
        }
    }
}

static void LoadNextHighScore(FILE *fPtr){
    char c;
    int counter = 0;
    int hackDefenseCounter = 0;
    int hackDefenseMaxLen = 32;
    int indices[2];
    int tempMaxLen = 10;
    char temp[tempMaxLen];
    for (int i = 0; i < tempMaxLen; i++){
        temp[i] = '\0';
    }
    int tempCounter = 0;
    
    while (('\n' != (c = fgetc(fPtr))) && hackDefenseCounter < hackDefenseMaxLen){
        hackDefenseCounter++;
        
        if (tempCounter > tempMaxLen - 1) continue;
        
        if (',' == c || ' ' == c){
            if (counter >= 2) continue;
            
            temp[tempCounter] = '\0';
            indices[counter] = atoi(temp);
            for (int i = 0; i < tempMaxLen; i++){
                temp[i] = '\0';
            }
            tempCounter = 0;
            counter++;
        }
        else if (tempCounter < tempMaxLen -1){
            temp[tempCounter] = c;
            tempCounter++;
        }
    }
    
    temp[tempCounter] = '\0';
    wly_highScores[indices[0]][indices[1]] = atoi(temp);
}