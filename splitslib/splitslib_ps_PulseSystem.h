#ifndef SPLITSLIB_PS_PulseSystem_H
#define SPLITSLIB_PS_PulseSystem_H

#include "raylib.h"

#define COLUMNPULSE_COUNT_MAX 128
#define SQUAREPULSE_COUNT_MAX 32

#define PULSESQUARE_DEFAULT_COLOR BLACK
#define PULSESQUARE_DEFAULT_PERIOD_SEC 1.0f
#define PULSESQUARE_DEFAULT_POS (Vector2){0.0f, 0.0f}
#define PULSESQUARE_DEFAULT_PULSECOLOR WHITE
#define PULSESQUARE_DEFAULT_PULSESPEED_PERSEC 64.0f // per second 
#define PULSESQUARE_DEFAULT_SIZE_MIN 64
#define PULSESQUARE_DEFAULT_SIZE_MAX 96 

typedef struct splitslib_ps_BasePulse {
    bool isActive;
} splitslib_ps_BasePulse;

typedef struct splitslib_ps_PulseBase {
    Vector2 pos;
    Color color;
    Color pulseColor;
    float pulseSpeed;
    float timer;
    float period;
} splitslib_ps_PulseBase;

typedef struct splitslib_ps_SquarePulse {
    splitslib_ps_BasePulse base;
    float size;
} splitslib_ps_SquarePulse;

typedef struct splitslib_ps_PulseSquare {
    splitslib_ps_PulseBase base;
    float size;
    float size_max;
    float size_min;
    splitslib_ps_SquarePulse pulses[SQUAREPULSE_COUNT_MAX];
} splitslib_ps_PulseSquare;

typedef struct splitslib_ps_ColumnPulse {
    splitslib_ps_BasePulse base;
    float dist;
    int dir;
} splitslib_ps_ColumnPulse;

typedef struct splitslib_ps_PulseColumn {
    splitslib_ps_PulseBase base;
    int w;
    int h;
    int pulseH;
    splitslib_ps_ColumnPulse pulses[COLUMNPULSE_COUNT_MAX];
} splitslib_ps_PulseColumn;

void splitslib_ps_PulseSquare_Animate(splitslib_ps_PulseSquare *ps);

void splitslib_ps_PulseSquare_Draw(splitslib_ps_PulseSquare *ps, float offsetX, float offsetY);

float splitslib_ps_PulseSquare_Get_BottomY(splitslib_ps_PulseSquare *ps);

float splitslib_ps_PulseSquare_Get_LeftX(splitslib_ps_PulseSquare *ps);

float splitslib_ps_PulseSquare_Get_RightX(splitslib_ps_PulseSquare *ps);

float splitslib_ps_PulseSquare_Get_TopY(splitslib_ps_PulseSquare *ps);

// sets parameters to pulse square & square pulse defaults
void splitslib_ps_PulseSquare_Init(splitslib_ps_PulseSquare *ps);

void splitslib_ps_PulseSquare_MoveBy(splitslib_ps_PulseSquare *ps, float x, float y);

void splitslib_ps_PulseSquare_MoveTo(splitslib_ps_PulseSquare *ps, float x, float y);

void splitslib_ps_PulseSquare_SetColor(splitslib_ps_PulseSquare *ps, Color c);

void splitslib_ps_PulseSquare_SetPulseColor(splitslib_ps_PulseSquare *ps, Color c);

void splitslib_ps_PulseSquare_SetSize(splitslib_ps_PulseSquare *ps, float size_min, float size_max);

#endif