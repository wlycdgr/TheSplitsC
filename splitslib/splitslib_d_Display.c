#include "raylib.h"
#include "splitslib_d_Display.h"

void splitslib_d_InitDisplay
(char *windowTitle, int width, int height, bool isFullscreen)
{
    if (isFullscreen) ToggleFullscreen();
    InitWindow(width, height, windowTitle);
    
    splitslib_d_screen.width = GetScreenWidth();
    splitslib_d_screen.height = GetScreenHeight();
    splitslib_d_screen.center.x = splitslib_d_screen.width / 2;
    splitslib_d_screen.center.y = splitslib_d_screen.height / 2;
}