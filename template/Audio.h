#ifndef wly_Audio_H
#define wly_Audio_H

#include "raylib.h"

/*
prepping for project:
    adjust defines as necessary
    fill out the WLY_SFX enum with names for all the sfx used for project
    init the sfxFilenames array in order that matches the enum
on startup:
    loop through sfxFilenames loading all the audio files,
    and save references to them in the SFX array
to play sounds:
    call PlaySound(sfx[WLY_SFX_MenuSelect]) or similar
on shutdown:
    loop through sfx array calling unload sound
*/

#define WLY_SFXCount 32 // set to whatever the # of sfx in the project is
#define WLY_SFXDirectoryPath "../audio/sfx/"

typedef enum WLY_SFX {
    WLY_SFX_MenuScroll = 0,
    WLY_SFX_MenuSelect // , .... list all the sfx used in the project
} WLY_SFX;
char *wly_sfxFilenames[WLY_SFXCount];

Sound wly_sfx[WLY_SFXCount];

void wly_InitAudio();
void wly_ShutdownAudio();

#endif