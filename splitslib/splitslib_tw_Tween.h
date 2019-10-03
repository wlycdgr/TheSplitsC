#ifndef SPLITSLIB_ANIM_Animation_H
#define SPLITSLIB_ANIM_Animation_H

// enforced as lower bounds by Init
#define SPLITSLIB_ANIM_MIN_SECONDS_AT_TARGET 0.0f
#define SPLITSLIB_ANIM_MIN_SECONDS_TO_SOURCE 0.01f
#define SPLITSLIB_ANIM_MIN_SECONDS_TO_TARGET 0.01f

// if we add more tween types,
// modify init function or add setter
typedef enum splitslib_anim_TweenCurveType{
    SPLITSLIB_ANIM_Linear = 0
    // add others as needed
    // e.g. splitslib_anim_SCurve, etc
} splitslib_anim_TweenCurveType;
#define SPLITSLIB_ANIM_DEFAULT_TWEEN_CURVE_TYPE SPLITSLIB_ANIM_Linear

void splitslib_anim_Destroy(int id);
float splitslib_anim_GetValue(int id);
bool splitslib_anim_HasLooped(int id);
int splitslib_anim_Init(
    float sourceValue,
    float targetValue,
    float secondsToTarget,
    float secondsAtTarget,
    float secondsToSource,
    bool doLoop,
    bool isRoundtrip
);
void splitslib_anim_Tween(int id);

#endif