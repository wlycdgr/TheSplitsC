#include "Audio.h"

void wly_InitAudio(){
    InitAudioDevice();
    
    for (int i = 0; i < WLY_SFXCount; i++){
        wly_sfx[i] = LoadSound(
            FormatText(
                "%s%s",
                WLY_SFXDirectoryPath,
                wly_sfxFilenames[i]
            )
        );  
    }
}

void wly_ShutdownAudio(){
    for (int i = 0; i < WLY_SFXCount; i++){
        UnloadSound(wly_sfx[i]);
    }
    
    CloseAudioDevice();
}