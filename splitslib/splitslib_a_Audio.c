#include "raylib.h"
#include "splitslib_a_Audio.h"

#define WLY_CommonSoundCount 5
#define WLY_CommonSoundFileExtension ".ogg"
#define WLY_CommonSoundFilepath "../splitslib/audio/sfx/"

char *wly_commonSoundFilenames[WLY_CommonSoundCount] = {
    "optionscroll",
    "mainscroll",
    "menuselect",
    "pausegame",
    "opentransition"
};

Sound wly_commonSounds[WLY_CommonSoundCount];
Sound wly_customSounds[SPLITSLIB_A_CustomSoundMaxCount];
int wly_customSoundCount;

// TODO implement error handling (wrong path, wrong extension, missing files, etc)
void splitslib_a_InitAudio(){
    InitAudioDevice();
    
    for (int i = 0; i < WLY_CommonSoundCount; i++){
        wly_commonSounds[i] = LoadSound(
            FormatText(
                "%s%s%s",
                WLY_CommonSoundFilepath,
                wly_commonSoundFilenames[i],
                WLY_CommonSoundFileExtension
            )
        );  
    }
    
    wly_customSoundCount = -1;
}

// TODO implement error handling (wrong path, wrong extension, missing files, etc)
void splitslib_a_LoadCustomSounds
(char **filenames, char *relativePath, char *extension, int count){
    if (count > SPLITSLIB_A_CustomSoundMaxCount) return;

    for (int i = 0; i< count; i++){
        wly_customSounds[i] = LoadSound(
            FormatText(
                "%s%s%s",
                relativePath,
                filenames[i],
                extension
            )
        );
    }
    
    wly_customSoundCount = count;
}

void splitslib_a_PlayCommonSound(int name){
    if (name < 0 || name >= WLY_CommonSoundCount) return;
    
    PlaySound(wly_commonSounds[name]);
}

void splitslib_a_PlayCustomSound(int name){
    if (name < 0 || name >= wly_customSoundCount) return;
    
    PlaySound(wly_customSounds[name]);
}

void splitslib_a_SetCommonSoundVolume(int name, float volume){
    if (name < 0 || name >= WLY_CommonSoundCount) return;
    
    SetSoundVolume(wly_commonSounds[name], volume);
}

void splitslib_a_SetCustomSoundVolume(int name, float volume){
    if (name < 0 || name > wly_customSoundCount) return;
    
    SetSoundVolume(wly_customSounds[name], volume);
}

void splitslib_a_ShutdownAudio(){
    for (int i = 0; i < WLY_CommonSoundCount; i++){
        UnloadSound(wly_commonSounds[i]);
    }
    
    for (int i = 0; i < wly_customSoundCount; i++){
        UnloadSound(wly_customSounds[i]);
    }
    
    CloseAudioDevice();
}