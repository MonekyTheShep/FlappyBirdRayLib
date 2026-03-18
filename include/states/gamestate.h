#pragma once

#include "bird.h"
#include "pipe.h"

#include "constants.h"

//----------------------------------------------------------------------------------
// Typedefs
//----------------------------------------------------------------------------------
typedef struct GameState
{
    Pipe pipePool[NUM_OF_PIPES];
    Bird bird;
    PipeTexture pipeTexture;

    bool gameOver;
    int score;
} GameState;
