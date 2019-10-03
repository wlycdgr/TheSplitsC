#include <stdlib.h>
#include "raylib.h"
#include "splitslib_tw_Tween.h"
#include "splitslib_pv_ParamValidation.h"

#define WLY_MaxTweens 32

typedef enum WLY_TWEENPHASE {
    WLY_TWEENPHASE_Done = 0,
    WLY_TWEENPHASE_HoldingAtTarget,
    WLY_TWEENPHASE_Inactive,
    WLY_TWEENPHASE_MovingToSource,
    WLY_TWEENPHASE_MovingToTarget
} WLY_TWEENPHASE;

typedef struct wly_TweenParams{
    splitslib_anim_TweenCurveType curveType;
    bool doLoop;
    bool isRoundtrip;
    float secondsAtTarget;
    float secondsToSource;
    float secondsToTarget;
    float sourceValue;
    float targetValue;
    float totalSeconds;
} wly_TweenParams;

typedef struct wly_Tween{
    float changePerSecond;
    bool isActive;
    int loopCount;
    wly_TweenParams params;
    WLY_TWEENPHASE phase;
    float phaseTimer;
    float secondsSinceLastLoop;
    float value;
} wly_Tween;

static wly_Tween wly_tweens[WLY_MaxTweens];
static int wly_numberOfActiveTweens = 0;

static int wly_GetFirstInactiveTween();
static bool wly_TweenIdIsValid(int id);

// PUBLIC API FUNCTIONS
void splitslib_anim_Destroy(int id){
    if (!wly_TweenIdIsValid(id)){
        return;
    }
    
    wly_tweens[id].changePerSecond = 0.0f;
    wly_tweens[id].isActive = false;
    wly_tweens[id].loopCount = 0;
    wly_tweens[id].phase = WLY_TWEENPHASE_Inactive;
    wly_tweens[id].secondsSinceLastLoop = 0.0f;
    wly_tweens[id].value = 0.0f;
    
    wly_numberOfActiveTweens--;
}

float splitslib_anim_GetValue(int id){
    if (wly_TweenIdIsValid(id)){
        return wly_tweens[id].value;
    }
    else {
        return -1.0f;
    }
}

bool splitslib_anim_HasLooped(int id){
    if (wly_TweenIdIsValid(id)){
        return wly_tweens[id].loopCount > 0;
    }
    else {
        return false;
    }
}

int splitslib_anim_Init(
    float sourceValue,
    float targetValue,
    float secondsToTarget,
    float secondsAtTarget,
    float secondsToSource,
    bool doLoop,
    bool isRoundtrip
){ 
    if (WLY_MaxTweens <= wly_numberOfActiveTweens){
        return -1;
    }
    
    int id;
    if (-1 == (id = wly_GetFirstInactiveTween()))
        return -1;
    
    wly_tweens[id].isActive = true;
    wly_tweens[id].secondsSinceLastLoop = 0.0f;
    wly_tweens[id].loopCount = 0;
    wly_tweens[id].phase = WLY_TWEENPHASE_MovingToTarget;
    wly_tweens[id].phaseTimer = 0.0f;
    wly_tweens[id].changePerSecond = 
        (targetValue - sourceValue) / secondsToSource;
    wly_tweens[id].value = sourceValue;
    
    wly_tweens[id].params.sourceValue = sourceValue;
    wly_tweens[id].params.targetValue = targetValue;
    wly_tweens[id].params.secondsToTarget = 
        secondsToTarget > SPLITSLIB_ANIM_MIN_SECONDS_TO_TARGET ?
        secondsToTarget : SPLITSLIB_ANIM_MIN_SECONDS_TO_TARGET;
    wly_tweens[id].params.secondsAtTarget =
        secondsAtTarget > SPLITSLIB_ANIM_MIN_SECONDS_AT_TARGET ?
        secondsAtTarget : SPLITSLIB_ANIM_MIN_SECONDS_AT_TARGET;
    wly_tweens[id].params.secondsToSource =
        secondsToSource > SPLITSLIB_ANIM_MIN_SECONDS_TO_SOURCE ?
        secondsToSource : SPLITSLIB_ANIM_MIN_SECONDS_TO_SOURCE;
    wly_tweens[id].params.totalSeconds = isRoundtrip ?
        wly_tweens[id].params.secondsToTarget +
        wly_tweens[id].params.secondsAtTarget +
        wly_tweens[id].params.secondsToSource :
        wly_tweens[id].params.secondsToTarget;
    wly_tweens[id].params.doLoop = doLoop;
    wly_tweens[id].params.isRoundtrip = isRoundtrip;
    wly_tweens[id].params.curveType = SPLITSLIB_ANIM_DEFAULT_TWEEN_CURVE_TYPE;

    wly_numberOfActiveTweens++;
    
    return id; 
}

void splitslib_anim_Tween(int id){
    if (!wly_TweenIdIsValid(id)){
        return;
    }
    
    wly_Tween *tPtr = &(wly_tweens[id]);
    
    float frameTime = GetFrameTime();    
    float changeThisFrame;
    switch(tPtr->phase){
    case WLY_TWEENPHASE_MovingToTarget:
        tPtr->secondsSinceLastLoop += frameTime;
        changeThisFrame = tPtr->changePerSecond * frameTime;
        if (
            abs(changeThisFrame) >= 
            abs(tPtr->params.targetValue - tPtr->value)
        ){
            tPtr->value = tPtr->params.targetValue;
            tPtr->phaseTimer = 0.0f;
            if (tPtr->params.isRoundtrip){
                tPtr->phase = WLY_TWEENPHASE_HoldingAtTarget;
            }
            else {
                tPtr->phase = WLY_TWEENPHASE_Done;
            }
        }
        else {
            tPtr->value += changeThisFrame;
        }
        break;
    
    case WLY_TWEENPHASE_HoldingAtTarget:
        tPtr->secondsSinceLastLoop += frameTime;
        tPtr->phaseTimer += frameTime;
        if (tPtr->phaseTimer >= tPtr->params.secondsAtTarget){
            tPtr->changePerSecond = 
                (tPtr->params.sourceValue - tPtr->value) /
                tPtr->params.secondsToSource;
            tPtr->phaseTimer = 0.0f;
            tPtr->phase = WLY_TWEENPHASE_MovingToSource;
        }
        break;
        
    case WLY_TWEENPHASE_MovingToSource:
        tPtr->secondsSinceLastLoop += frameTime;
        changeThisFrame = tPtr->changePerSecond * frameTime;
        if (
            abs(changeThisFrame) >= 
            abs(tPtr->params.sourceValue - tPtr->value)
        ){
            tPtr->value = tPtr->params.sourceValue;
            if (tPtr->params.doLoop){
                tPtr->changePerSecond =
                    (tPtr->params.targetValue - tPtr->value) / 
                    tPtr->params.secondsToTarget;
                tPtr->loopCount++;
                tPtr->secondsSinceLastLoop = 0.0f;
                tPtr->phase = WLY_TWEENPHASE_MovingToTarget;
            }
            else {
                tPtr->phase = WLY_TWEENPHASE_Done;
            }
        }
        else {
            tPtr->value += changeThisFrame;
        }
        break;
    }
}

// PRIVATE MODULE FUNCTIONS
static int wly_GetFirstInactiveTween(){
    for (int i = 0; i < WLY_MaxTweens; i++){
        if (!wly_tweens[i].isActive)
            return i;
    }
    return -1;
}

static bool wly_TweenIdIsValid(int id){
    return
       (splitslib_pv_IsArrayIndexInBounds(id, WLY_MaxTweens) &&
       wly_tweens[id].isActive);
}