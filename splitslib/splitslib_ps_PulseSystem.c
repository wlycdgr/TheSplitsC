#include "raylib.h"
#include "splitslib_ps_PulseSystem.h"

void splitslib_ps_PulseSquare_Animate(splitslib_ps_PulseSquare *ps){
    float frameTime = GetFrameTime();
    
    // grow base square
    //ps->size += 0.1f;
    if (ps->size > ps->size_max)
        ps->size = ps->size_min;
    
    // advance timer & spawn new pulse if necessary
    ps->base.timer += frameTime;
    if (ps->base.timer >= ps->base.period){
        ps->base.timer = 0.0f;
        for (int i = 0; i < SQUAREPULSE_COUNT_MAX; i++){
            if (ps->pulses[i].base.isActive) continue;
            
            ps->pulses[i].base.isActive = true;
            ps->pulses[i].size = 0;
            
            break;
        }
    }
    
    // animate active pulses and deactivate if necessary
    for (int i = 0; i < SQUAREPULSE_COUNT_MAX; i++){
        if (!ps->pulses[i].base.isActive) continue;
        
        ps->pulses[i].size += ps->base.pulseSpeed * frameTime;
        if (ps->pulses[i].size > ps->size)
            ps->pulses[i].base.isActive = false;
    }
}

void splitslib_ps_PulseSquare_Draw(splitslib_ps_PulseSquare *ps, float offsetX, float offsetY){
    float centerX = ps->base.pos.x + offsetX;
    float centerY = ps->base.pos.y + offsetY;
    
    DrawRectangle(
        centerX - ps->size / 2,
        centerY - ps->size / 2,
        ps->size, ps->size,
        ps->base.color);
        
    for (int i = 0; i < SQUAREPULSE_COUNT_MAX; i++){
        if (!ps->pulses[i].base.isActive) continue;
        
        DrawRectangleLines(
            centerX - ps->pulses[i].size / 2,
            centerY - ps->pulses[i].size / 2,
            2 * (ps->pulses[i].size / 2), 2 * (ps->pulses[i].size / 2),
            ps->base.pulseColor);
    }
}

float splitslib_ps_PulseSquare_Get_BottomY(splitslib_ps_PulseSquare *ps){
    return ps->base.pos.y + ps->size / 2;
}

float splitslib_ps_PulseSquare_Get_LeftX(splitslib_ps_PulseSquare *ps){
    return ps->base.pos.x - ps->size / 2;
}

float splitslib_ps_PulseSquare_Get_RightX(splitslib_ps_PulseSquare *ps){
    return ps->base.pos.x + ps->size / 2;
}

float splitslib_ps_PulseSquare_Get_TopY(splitslib_ps_PulseSquare *ps){
    return ps->base.pos.y - ps->size / 2;
}

void splitslib_ps_PulseSquare_Init(splitslib_ps_PulseSquare *ps){
    ps->base.pos = PULSESQUARE_DEFAULT_POS;
    ps->base.color = PULSESQUARE_DEFAULT_COLOR;
    ps->base.pulseColor = PULSESQUARE_DEFAULT_PULSECOLOR;
    ps->base.pulseSpeed = PULSESQUARE_DEFAULT_PULSESPEED_PERSEC;
    ps->base.timer = 0.0f;
    ps->base.period = PULSESQUARE_DEFAULT_PERIOD_SEC;
    
    ps->size = PULSESQUARE_DEFAULT_SIZE_MIN;
    ps->size_min = PULSESQUARE_DEFAULT_SIZE_MIN;
    ps->size_max = PULSESQUARE_DEFAULT_SIZE_MAX;
    
    for (int i = 0; i < SQUAREPULSE_COUNT_MAX; i++){
        ps->pulses[i].base.isActive = false;
    }
}

void splitslib_ps_PulseSquare_MoveBy(splitslib_ps_PulseSquare *ps, float x, float y){
    ps->base.pos.x += x;
    ps->base.pos.y += y;
}

void splitslib_ps_PulseSquare_MoveTo(splitslib_ps_PulseSquare *ps, float x, float y){
    ps->base.pos.x = x;
    ps->base.pos.y = y;
}

void splitslib_ps_PulseSquare_SetColor(splitslib_ps_PulseSquare *ps, Color c){
    ps->base.color = c;
}

void splitslib_ps_PulseSquare_SetPulseColor(splitslib_ps_PulseSquare *ps, Color c){
    ps->base.pulseColor = c;
}

void splitslib_ps_PulseSquare_SetSize(splitslib_ps_PulseSquare *ps, float size_min, float size_max){
    ps->size = size_min;
    ps->size_min = size_min;
    ps->size_max = size_max;
}