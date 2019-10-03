#include "raylib.h"
#include "../splitslib/splitslib_a_Audio.h"
#include "../splitslib/splitslib_d_Display.h"
#include "../splitslib/splitslib_sf_Splitsfont.h"
#include "../splitslib/splitslib_ui_UserInterface.h"
#include "src/HighScoreManager.h"
#include "src/Options.h"
#include "src/ProjectAudio.h"
#include "src/SplitCanvas.h"
#include "src/StateManager.h"

#define WLY_TargetFPS 60
#define WLY_WindowTitle "The Fignermukcre Variations"
#define WLY_IsFullscreen false

#define WLY_DEBUGMODE_ON true
#define WLY_DEBUGMODE_KEY_EXIT KEY_Z

//#define PLATFORM_WEBGL
#if defined(PLATFORM_WEBGL)
    #include <emscripten/emscripten.h>
#endif

void UpdateDrawFrame(void);     // Update and Draw one frame
    
int main()
{        
    SetTargetFPS(WLY_TargetFPS);
    
    splitslib_d_InitDisplay(
        WLY_WindowTitle,
        WLY_WINDOW_WIDTH, WLY_WINDOW_HEIGHT,
        WLY_IsFullscreen);
    
    SplitCanvas_Init(WLY_WINDOW_WIDTH, WLY_WINDOW_HEIGHT);
        
    splitslib_sf_Init();
    
    splitslib_ui_InitReturnPrompt();
    
    wly_LoadOptions();
    wly_LoadHighScores();
    
    splitslib_a_InitAudio();
    splitslib_a_LoadCustomSounds(
        wly_projectAudioSfxFilenames,
        WLY_ProjectAudioSfxFilepath,
        WLY_ProjectAudioSfxFileExtension,
        WLY_ProjectAudioSfxCount
    );
    splitslib_a_SetCommonSoundVolume(
        SPLITSLIB_SOUND_SplitTransition,
        0.05
    );
    splitslib_a_SetCustomSoundVolume(
        WLY_PROJECTAUDIO_SFX_ClassicAvatarSuccessMove,
        0.05
    );
    splitslib_a_SetCustomSoundVolume(
        WLY_PROJECTAUDIO_SFX_PinkEnemyAppears,
        0.05
    );
    
    wly_SetState(WLY_STATE_Title);
    
    wly_doMainLoop = true;
    #if defined(PLATFORM_WEBGL)
        emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
    #else 
        while (wly_doMainLoop){
            UpdateDrawFrame();
        }
    #endif
    
    splitslib_a_ShutdownAudio();

    CloseWindow();
    
    return 0;
}

void UpdateDrawFrame(){
    wly_CurrentUpdate();

    BeginDrawing();
        ClearBackground(WHITE);
        wly_CurrentDraw2D();     
    EndDrawing();

    if (WLY_DEBUGMODE_ON){
        if (IsKeyPressed(WLY_DEBUGMODE_KEY_EXIT))
            wly_doMainLoop = false;
    } 
}