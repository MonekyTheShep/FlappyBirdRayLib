#include "states/gamestate.h"

#include <stdio.h>
#include <stdlib.h>

#include "constants.h"

#include "pipe.h"
#include "bird.h"


Bird bird;

int gameOver = 0;

Pipe pipePool[POOL_SIZE];

void initializeGame(void)
{
    // Pipe declaring
    initializePipePool(pipePool);

    // Bird declaring
    initializeBird(&bird);
}

float accumulationTime = 3.5f;

static void spawnPipe(void)
{
    if (accumulationTime >= 3.5f)
    {
        Pipe *pipe = acquirePipe(pipePool);
        if (pipe != NULL)
        {
            pipe->position.y = (rand() % 300) - 150;
            accumulationTime = 0.0f;
        }
    }
}

void updateGameMenu(GameInfo *gameInfo, MenuStates *menuState)
{
    const float deltaTime = GetFrameTime();
    accumulationTime += deltaTime;

    if (!gameOver)
    {
        spawnPipe();
        handleBird(&bird);
        handlePipes(pipePool, &bird, &gameOver);
    }
}


static void drawHitBoxDebug(void)
{
    DrawRectangleRec(bird.hitBox, Fade(RED, 0.5f));
}

void drawGameMenu(void)
{
    const float deltaTime = GetFrameTime();

    drawBird(&bird);
    drawPipes(pipePool);
    drawHitBoxDebug();

    if (deltaTime != 0)
    {
        DrawText(TextFormat("CURRENT FPS: %i", (int)(1.0f/deltaTime)),  0, 0, 20, GREEN);
        DrawText(TextFormat("ACCELERATION M/2^2: %i", (int)(bird.velocity.y * deltaTime - 0 / (1.0f/deltaTime))),  0, 50, 20, GREEN);
    }
}
