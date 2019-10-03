#ifndef SPLITSLIB_A_Audio_H
#define SPLITSLIB_A_Audio_H

#include "raylib.h"
 
typedef enum SPLITSLIB_SOUND {
    SPLITSLIB_SOUND_MenuOptionScroll = 0,
    SPLITSLIB_SOUND_MenuMainScroll,
    SPLITSLIB_SOUND_MenuSelect,
    SPLITSLIB_SOUND_PauseGame,
    SPLITSLIB_SOUND_SplitTransition
} SPLITSLIB_SOUND;

void splitslib_a_InitAudio();

#define SPLITSLIB_A_CustomSoundMaxCount 32
void splitslib_a_LoadCustomSounds
(char **filenames, char *relativePath, char *extension, int count);

void splitslib_a_PlayCommonSound(int name); // use SPLITSLIB_SOUND names
void splitslib_a_PlayCustomSound(int name);
void splitslib_a_SetCommonSoundVolume(int name, float volume);
void splitslib_a_SetCustomSoundVolume(int name, float volume);
void splitslib_a_ShutdownAudio();

#endif