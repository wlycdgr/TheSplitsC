#ifndef wly_Options_H
#define wly_Options_H

void wly_Options_Init();
void wly_Options_Update();
void wly_Options_Draw();

typedef struct wly_OptionValues {
    int sfxVolume;
    int musicVolume;
} wly_OptionValues;
wly_OptionValues wly_optionValues;

#define WLY_DefaultSFXVolume 86
#define WLY_DefaultMusicVolume 35
void wly_LoadOptions();

#endif