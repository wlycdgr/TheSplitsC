#include "raylib.h"
#include "../../splitslib/splitslib_a_Audio.h"
#include "SplitCanvas.h"

typedef struct wly_RenderTextures{
    RenderTexture2D mainFlipped;
    RenderTexture2D main;
    RenderTexture2D leftPaneFlipped;
    RenderTexture2D leftPane;
    RenderTexture2D rightPaneFlipped;
    RenderTexture2D rightPane;
    float width;
} wly_RenderTextures;

typedef struct wly_Transition{
    bool isTransitioning;
    float splitX;
    float leftPaneX;
    float rightPaneX;
    float leftDistance;
    float rightDistance;
} wly_Transition;

static wly_Transition transition;
static wly_RenderTextures rtextures;

void SplitCanvas_BeginTextureMode(){
    BeginTextureMode(rtextures.mainFlipped);
}

void SplitCanvas_ContinueTransition(){
    float frameTime = GetFrameTime();
    transition.leftPaneX -= 
        (transition.leftDistance * frameTime) / 
        (WLY_TRANSITION_DURATION_IN_SECS);
    transition.rightPaneX += 
        (transition.rightDistance * frameTime) /
        (WLY_TRANSITION_DURATION_IN_SECS);
        
    if (
        transition.leftPaneX < -rtextures.width &&
        transition.rightPaneX > rtextures.width
    ){
        transition.isTransitioning = false;
    }
}

void SplitCanvas_DrawAll(){
    SplitCanvas_DrawMain();
    
    if (transition.isTransitioning){
        DrawTexture(
            rtextures.leftPane.texture,
            transition.leftPaneX, 0,
            WHITE);
        DrawTexture(
            rtextures.rightPane.texture,
            transition.rightPaneX, 0,
            WHITE);
    }
}

void SplitCanvas_DrawMain(){
    DrawTexture(rtextures.main.texture, 0, 0, WHITE);
}

void SplitCanvas_EndTextureMode(){
    EndTextureMode();
    BeginTextureMode(rtextures.main);
        DrawTexture(rtextures.mainFlipped.texture, 0, 0, WHITE);
    EndTextureMode();
}

void SplitCanvas_Init(float width, float height){
    rtextures.mainFlipped = LoadRenderTexture(width, height);
    rtextures.main = LoadRenderTexture(width, height);
    rtextures.leftPaneFlipped = LoadRenderTexture(width, height);
    rtextures.leftPane = LoadRenderTexture(width, height);
    rtextures.rightPaneFlipped = LoadRenderTexture(width, height);
    rtextures.rightPane = LoadRenderTexture(width, height);
    rtextures.width = width;
    
    transition.isTransitioning = false;
    transition.splitX = 0;
    transition.leftPaneX = 0;
    transition.rightPaneX = 0;
}

bool SplitCanvas_IsTransitioning(){
    return transition.isTransitioning;
}

void SplitCanvas_StartTransition(float splitX){
    splitslib_a_PlayCommonSound(SPLITSLIB_SOUND_SplitTransition);
    
    transition.splitX = splitX;
    transition.leftPaneX = splitX - rtextures.width;
    transition.rightPaneX = splitX;
    transition.leftDistance = splitX;
    transition.rightDistance = rtextures.width - splitX;
    
    BeginTextureMode(rtextures.leftPaneFlipped);
        DrawTexture(rtextures.main.texture, rtextures.width - splitX, 0, WHITE);
    EndTextureMode();
    BeginTextureMode(rtextures.leftPane);
        DrawTexture(rtextures.leftPaneFlipped.texture, 0, 0, WHITE);
    EndTextureMode();
    
    BeginTextureMode(rtextures.rightPaneFlipped);
        DrawTexture(rtextures.main.texture, -splitX, 0, WHITE);
    EndTextureMode();
    BeginTextureMode(rtextures.rightPane);
        DrawTexture(rtextures.rightPaneFlipped.texture, 0, 0, WHITE);
    EndTextureMode();
    
    transition.isTransitioning = true;
}