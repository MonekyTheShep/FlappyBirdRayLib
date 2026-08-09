#pragma once

#include <raylib.h>

//----------------------------------------------------------------------------------
// Global Asset Variables
//----------------------------------------------------------------------------------
enum Sounds
{
    PLACEHOLDER_SOUND,
    NUM_OF_SOUNDS
};

enum Musics
{
    PLACEHOLDER_MUSIC,
    NUM_OF_MUSICS
};

extern Sound sounds[NUM_OF_SOUNDS];
extern Music music[NUM_OF_MUSICS];
