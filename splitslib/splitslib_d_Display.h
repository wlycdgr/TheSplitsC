#ifndef Splitslib_Display_H
#define Splitslib_Display_H

#include "raylib.h"

#define WLY_WINDOW_WIDTH 1280
#define WLY_WINDOW_HEIGHT 720

#define WLY_WINDOW_WEB_WIDTH 960
#define WLY_WINDOW_WEB_HEIGHT 540 // 75%

#define WLY_OFFSCREEN (Vector2){-1000.0f, -1000.0f}

typedef struct splitslib_Screen{
    float height;
    float width;
    Vector2 center;
} splitslib_Screen;
splitslib_Screen splitslib_d_screen; // a global for everyone to share
// please be nice and do not mess with its values!

void splitslib_d_InitDisplay
(char *windowTitle, int width, int height, bool isFullscreen);

#endif