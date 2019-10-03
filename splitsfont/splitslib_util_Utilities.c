#include <stdlib.h>
#include "splitslib_util_Utilities.h"

float splitslib_util_Clamp(float value, float min, float max){
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

int splitslib_util_Random(int max){
    return rand() % max;
}

int splitslib_util_Wrap(int value, int min, int max){
    if (value < min) return max;
    if (value > max) return min;
    return value;
}