#pragma once
#include "bird.h"
#include "pipe.h"
#include "constants.h"

typedef struct GameState {
    Pipe pipePool[POOL_SIZE];
    Bird bird;
    PipeTexture pipeTexture;

    bool gameOver;
    int score;
} GameState;


// Logic Functions
void incrementScore(void);
void gameOver(void);

