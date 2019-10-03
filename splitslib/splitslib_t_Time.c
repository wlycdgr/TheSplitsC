#include "raylib.h" // needed for GetFrameTime(), which helpfully abstracts away the platform
#include "splitslib_pv_ParamValidation.h"
#include "splitslib_t_Time.h"

#define WLY_MaxTimers 32

typedef struct wly_Timer {
    float elapsedTimeInSecs;
    bool isActive;
    float targetTimeInSecs;
} wly_Timer;

static wly_Timer wly_timers[WLY_MaxTimers];
static int wly_numberOfActiveTimers = 0;

static bool wly_TimerIdIsValid(int id);

// PUBLIC API FUNCTIONS
void splitslib_t_AdvanceTimer(int id){
    if (!wly_TimerIdIsValid(id)){
        return;
    }
    
    if (wly_timers[id].elapsedTimeInSecs <= wly_timers[id].targetTimeInSecs){
        wly_timers[id].elapsedTimeInSecs += GetFrameTime();
    }
}

void splitslib_t_FreeTimer(int id){
    if (!wly_TimerIdIsValid(id)){
        return;
    }
    
    wly_timers[id].isActive = false;
    wly_timers[id].elapsedTimeInSecs = 0.0f;
    wly_timers[id].targetTimeInSecs = 0.0f;
    
    wly_numberOfActiveTimers--;
}

bool splitslib_t_HasTimerGoneOff(int id){
    if (!wly_TimerIdIsValid(id)){
        return false;
    }
    
    return wly_timers[id].elapsedTimeInSecs >= wly_timers[id].targetTimeInSecs;
}

int splitslib_t_InitTimer(float targetTimeInSecs){ 
    if (WLY_MaxTimers <= wly_numberOfActiveTimers){
        return -1;
    }
    
    int id = -1;
    for (int i = 0; i < WLY_MaxTimers; i++){
        if (!wly_timers[i].isActive){
            id = i;
            break;
        }
    }
    if (-1 == id){
        return -1;
    }
    
    wly_timers[id].elapsedTimeInSecs = 0.0f;
    wly_timers[id].isActive = true;
    wly_timers[id].targetTimeInSecs = targetTimeInSecs;
    
    wly_numberOfActiveTimers++;

    return id;
}

void splitslib_t_ResetTimer(int id){
    if (!wly_TimerIdIsValid(id)){
        return;
    }
    
    wly_timers[id].elapsedTimeInSecs = 0.0f;
}

static bool wly_TimerIdIsValid(int id){
    return 
        (splitslib_pv_IsArrayIndexInBounds(id, WLY_MaxTimers) ||
        wly_timers[id].isActive);
}