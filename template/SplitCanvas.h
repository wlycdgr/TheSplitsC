#ifndef wly_SplitCanvas_H
#define wly_SplitCanvas_H

#define WLY_TRANSITION_DURATION_IN_SECS 0.42

void SplitCanvas_BeginTextureMode();
void SplitCanvas_ContinueTransition();
void SplitCanvas_DrawAll();
void SplitCanvas_DrawMain();
void SplitCanvas_EndTextureMode();
void SplitCanvas_Init(float width, float height);
bool SplitCanvas_IsTransitioning();
void SplitCanvas_StartTransition(float splitX);
    
#endif