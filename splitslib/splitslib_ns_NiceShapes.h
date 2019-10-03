#ifndef SPLITSLIB_NS_NiceShapes_H
#define SPLITSLIB_NS_NiceShapes_H

#include "raylib.h"

#define SPLITSLIB_NS_NICELINES_DEFAULT_COLOR BLACK
#define SPLITSLIB_NS_NICELINES_DEFAULT_COUNT 32
#define SPLITSLIB_NS_NICELINES_DEFAULT_SPEED_MAX 3.0
#define SPLITSLIB_NS_NICELINES_DEFAULT_SPEED_MIN 1.0
#define SPLITSLIB_NS_NICELINES_DEFAULT_WIDTH_MAX 16
#define SPLITSLIB_NS_NICELINES_DEFAULT_WIDTH_MIN 16
#define SPLITSLIB_NS_NICELINES_LINES_MAX 256
#define SPLITSLIB_NS_NICELINES_LINES_MIN 1
#define SPLITSLIB_NS_NICELINES_SPEED_MIN 0.0f
#define SPLITSLIB_NS_NICELINES_WIDTH_MIN 1

typedef struct splitslib_ns_NiceLines {
    bool isReady;
    
    float xPositions[SPLITSLIB_NS_NICELINES_LINES_MAX];
    float xSpeeds[SPLITSLIB_NS_NICELINES_LINES_MAX];
    float yPositions[SPLITSLIB_NS_NICELINES_LINES_MAX];
    int widths[SPLITSLIB_NS_NICELINES_LINES_MAX];
    
    Color color;
    int count;
    float minSpeed;
    float maxSpeed;
    int minWidth;
    int maxWidth;
} splitslib_ns_NiceLines;

void splitslib_ns_NiceLines_Animate(splitslib_ns_NiceLines *nlPtr);

// call this when you're done adjusting the settings
// to generate the line system based on these settings
// sets isReady to true when done
void splitslib_ns_NiceLines_Build(splitslib_ns_NiceLines *nlPtr);

void splitslib_ns_NiceLines_Draw(splitslib_ns_NiceLines *nlPtr);

// initializes the nice lines system to default values
void splitslib_ns_NiceLines_Init(splitslib_ns_NiceLines *nlPtr);

// uses raylib Color struct
void splitslib_ns_NiceLines_SetColor(splitslib_ns_NiceLines *nlPtr, Color color);

// clamped between 1 and SPLITSLIB_NS_NICELINES_LINES_MAX
// requires rebuild
void splitslib_ns_NiceLines_SetCount(splitslib_ns_NiceLines *nlPtr, int count);

// clamped to >= nlPtr->minSpeed
// requires rebuild
void splitslib_ns_NiceLines_SetMaxSpeed(splitslib_ns_NiceLines *nlPtr, float maxSpeed);

// clamped between 0 and current value of nlPtr->maxSpeed
// requires rebuild
void splitslib_ns_NiceLines_SetMinSpeed(splitslib_ns_NiceLines *nlPtr, float minSpeed);

// clamped to >= nlPtr->minWidth
// requires rebuild
void splitslib_ns_NiceLines_SetMaxWidth(splitslib_ns_NiceLines *nlPtr, float maxWidth);

// clamped between 1 and current value of nlPtr->maxWidth
// requires rebuild
void splitslib_ns_NiceLines_SetMinWidth(splitslib_ns_NiceLines *nlPtr, float minWidth);

#endif