#pragma once

#include <raylib.h>

//----------------------------------------------------------------------------------
// Global Asset Variables
//----------------------------------------------------------------------------------
enum Sounds
{
    PLACEHOLDER_MUSIC_SOUND,
    NUM_OF_SOUND
};

enum Musics
{
    PLACEHOLDER_MUSIC,
    NUM_OF_MUSIC
};

extern Sound sounds[NUM_OF_SOUND];
extern Music music[NUM_OF_MUSIC];
