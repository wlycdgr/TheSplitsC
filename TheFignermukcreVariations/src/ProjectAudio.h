#ifndef wly_ProjectAudio_H
#define wly_ProjectAudio_H

#include "raylib.h"

/*
prepping for project:
    adjust defines as necessary
    fill out the WLY_SFX enum with names for all the project-specific sfx
    init the sfxFilenames array in order that matches the enum
on startup:
    call splitlib_a_LoadCustomSounds, passing in the sfx count and
    a pointer to the filenames array and the extension and path
    splislib_a_Audio supports 32 custom sounds by default,
    if more necessary, update library and recompile
to play sounds:
    splitslib_a_PlayCustomSound(WLY_SFX_ACustomSound);
on shutdown:
    splitslib_a_ShutdownAudio takes care of closing the audio device
    and unloading the custom sounds
*/

#define WLY_ProjectAudioSfxCount 20 // set to whatever the # of sfx in the project is
#define WLY_ProjectAudioSfxFilepath "audio/sfx/"
#define WLY_ProjectAudioSfxFileExtension ".ogg"

typedef enum WLY_PROJECTAUDIO_SFX {
    WLY_PROJECTAUDIO_SFX_BarDecayGameOver = 0,
    WLY_PROJECTAUDIO_SFX_ClassicAvatarFailedMove, // DOES NOT WORK for some reason?
    WLY_PROJECTAUDIO_SFX_ClassicAvatarSuccessMove,
    WLY_PROJECTAUDIO_SFX_EnemyGameOver,
    WLY_PROJECTAUDIO_SFX_EnergyAppear,
    WLY_PROJECTAUDIO_SFX_EnergyEat,
    WLY_PROJECTAUDIO_SFX_EnergyHitsBottom, // VERY QUIET
    WLY_PROJECTAUDIO_SFX_FailedSourceColumn,
    WLY_PROJECTAUDIO_SFX_GridFlip, // QUIET
    WLY_PROJECTAUDIO_SFX_GridReversal, // QUIET
    WLY_PROJECTAUDIO_SFX_GridSelect, // DOES NOT WORK
    WLY_PROJECTAUDIO_SFX_HighScore,
    WLY_PROJECTAUDIO_SFX_NoScoreEnemyBottom, // LOUD
    WLY_PROJECTAUDIO_SFX_PinkEnemyAppears,
    WLY_PROJECTAUDIO_SFX_ScoreEnemyBottom,
    WLY_PROJECTAUDIO_SFX_SuccessfulSourceColumn, // LOUD and NICE
    WLY_PROJECTAUDIO_SFX_TargetColumnAvatar, // very different from SuccessfulSourceColumn
    WLY_PROJECTAUDIO_SFX_TargetColumnEmpty, // NICE
    WLY_PROJECTAUDIO_SFX_WebFailedMove, // LOUD
    WLY_PROJECTAUDIO_SFX_WebSuccessMove // QUIET
} WLY_PROJECTAUDIO_SFX;

char *wly_projectAudioSfxFilenames[WLY_ProjectAudioSfxCount] = {
    "BarDecaygameover",
    "classicavatarfailedmove",
    "classicavatarsuccessmove",
    "enemygameover",
    "energyappear",
    "energyeat",
    "energyhitsbottom",
    "failedsourcecolumn",
    "gridflip",
    "gridreversal",
    "gridselect",
    "highscore",
    "noscoreenemybottom",
    "pinkenemyappears",
    "scoreenemybottom",
    "successfulsourcecolumn",
    "targetcolumnavatar",
    "targetcolumnempty",
    "webfailedmove",
    "websuccessmove"
};

#endif