#include "raylib.h"
#include "splitslib_d_Display.h"
#include "splitslib_util_Utilities.h"
#include "splitslib_ns_NiceShapes.h"

void splitslib_ns_NiceLines_Animate(splitslib_ns_NiceLines *nlPtr){
    if (!nlPtr->isReady) return;
    
    for (int i = 0; i < nlPtr->count; i++){
        nlPtr->xPositions[i] += nlPtr->xSpeeds[i];
        if (nlPtr->xPositions[i] > splitslib_d_screen.width){
            nlPtr->xPositions[i] = -nlPtr->widths[i];
        }
        else if (nlPtr->xPositions[i] < -nlPtr->widths[i]){
            nlPtr->xPositions[i] = splitslib_d_screen.width;
        } 
        
        
        /*
        nlPtr->yPositions[i] += 4 * nlPtr->xSpeeds[i];
        if (nlPtr->yPositions[i] > splitslib_d_screen.height)
            nlPtr->yPositions[i] = -splitslib_d_screen.height;
        */
    }
}

void splitslib_ns_NiceLines_Build(splitslib_ns_NiceLines *nlPtr){
    for (int i = 0; i < nlPtr->count; i++){
        nlPtr->xPositions[i] = splitslib_util_Random(splitslib_d_screen.width);
        nlPtr->yPositions[i] = 0;
        nlPtr->xSpeeds[i] = 
            nlPtr->minSpeed +
            splitslib_util_Random(nlPtr->maxSpeed - nlPtr->minSpeed);
        nlPtr->widths[i] = 
            nlPtr->minWidth +
            splitslib_util_Random(nlPtr->maxWidth - nlPtr->minWidth);
    }
    
    nlPtr->isReady = true;
}

void splitslib_ns_NiceLines_Draw(splitslib_ns_NiceLines *nlPtr){
    if (!nlPtr->isReady) return;
    
    for (int i = 0; i < nlPtr->count; i++){
        DrawRectangle(
            nlPtr->xPositions[i], nlPtr->yPositions[i],
            nlPtr->widths[i], splitslib_d_screen.height,
            nlPtr->color);
    }
}

// initializes the nice lines system to default values
// adjust values by accessing properties directly
// i trust you
void splitslib_ns_NiceLines_Init(splitslib_ns_NiceLines *nlPtr){
    nlPtr->color = SPLITSLIB_NS_NICELINES_DEFAULT_COLOR;
    nlPtr->count = SPLITSLIB_NS_NICELINES_DEFAULT_COUNT;
    nlPtr->maxSpeed = SPLITSLIB_NS_NICELINES_DEFAULT_SPEED_MAX;
    nlPtr->minSpeed = SPLITSLIB_NS_NICELINES_DEFAULT_SPEED_MIN;
    nlPtr->maxWidth = SPLITSLIB_NS_NICELINES_DEFAULT_WIDTH_MAX;
    nlPtr->minWidth = SPLITSLIB_NS_NICELINES_DEFAULT_WIDTH_MIN;
    
    nlPtr->isReady = false;
}

void splitslib_ns_NiceLines_SetColor(splitslib_ns_NiceLines *nlPtr, Color color){
    nlPtr->color = color;
}

void splitslib_ns_NiceLines_SetCount(splitslib_ns_NiceLines *nlPtr, int count){
    nlPtr->count = splitslib_util_Clamp(
        count, SPLITSLIB_NS_NICELINES_LINES_MIN, SPLITSLIB_NS_NICELINES_LINES_MAX);
        
    nlPtr->isReady = false;
}

void splitslib_ns_NiceLines_SetMaxSpeed(splitslib_ns_NiceLines *nlPtr, float maxSpeed){
    nlPtr->maxSpeed = maxSpeed;
    
    /*
    if (nlPtr->minSpeed > nlPtr->maxSpeed)
        nlPtr->maxSpeed = nlPtr->minSpeed;
    */
    nlPtr->isReady = false;
}

void splitslib_ns_NiceLines_SetMinSpeed(splitslib_ns_NiceLines *nlPtr, float minSpeed){
    nlPtr->minSpeed = minSpeed;
    /*
    nlPtr->minSpeed = splitslib_util_Clamp(
        minSpeed, SPLITSLIB_NS_NICELINES_SPEED_MIN, nlPtr->maxSpeed);
      */  
    nlPtr->isReady = false;
}

void splitslib_ns_NiceLines_SetMaxWidth(splitslib_ns_NiceLines *nlPtr, float maxWidth){
    nlPtr->maxWidth = maxWidth;
    if (nlPtr->minWidth > nlPtr->maxWidth)
        nlPtr->maxWidth = nlPtr->minWidth;
    
    nlPtr->isReady = false;
}

void splitslib_ns_NiceLines_SetMinWidth(splitslib_ns_NiceLines *nlPtr, float minWidth){
    nlPtr->minWidth = splitslib_util_Clamp(
        minWidth, SPLITSLIB_NS_NICELINES_WIDTH_MIN, nlPtr->maxWidth);
        
    nlPtr->isReady = false;
}