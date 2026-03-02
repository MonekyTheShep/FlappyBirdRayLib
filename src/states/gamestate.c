#include "states/gamestate.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include <raygui.h>

#include "constants.h"
#include "pipe.h"
#include "bird.h"

#include "states.h"

GameState gameState =
{
    .pipePool = {0},
    .bird = {0},
    .pipeTexture = {0},

    .gameOver = false,
    .score = 0
};

static int finishState = false;

static float accumulationTime = PIPE_SPAWN_RATE;

// Initialise Functions
void initializeGameState(void)
{
    finishState = false;

    gameState.gameOver = false;
    gameState.score = 0;

    accumulationTime = PIPE_SPAWN_RATE;

    // Pipe declaring
    initializePipePool(gameState.pipePool, &gameState.pipeTexture);

    // Bird declaring
    initializeBird(&gameState.bird);
}

void unloadGameState(void)
{
    CleanUpBird(&gameState.bird);
    CleanUpPipes(&gameState.pipeTexture);
}

bool finishGameState(void)
{
    return finishState;
}

// Logic Functions
static void spawnPipe(void)
{
    if (accumulationTime >= PIPE_SPAWN_RATE)
    {
        Pipe *pipe = acquirePipe(gameState.pipePool);
        if (pipe != NULL)
        {
            pipe->position.y = (float) GetRandomValue(-100, 200);
        }
        accumulationTime = 0.0f;
    }
}

void incrementScore(void)
{
    gameState.score++;
}

void gameOver(void)
{
    gameState.gameOver = true;
}

void updateGameState(const float deltaTime)
{
    accumulationTime += deltaTime;

    if (!gameState.gameOver)
    {
        spawnPipe();
        handleBird(deltaTime, &gameState.bird);
        handlePipes(deltaTime, gameState.pipePool, &gameState.bird);
    }
}


// Draw Functions
static void drawGameOverMenu(void)
{
    const float buttonWidth = 100.0f;
    const float buttonHeight = 50.0f;
    const float gameOverButtonX = ((float) GetScreenWidth() - buttonWidth) / 2.0f;
    const float gameOverButtonY = ((float) GetScreenHeight() - buttonHeight) / 2.0f;
    const Rectangle gameOverButton = {gameOverButtonX,gameOverButtonY,buttonWidth,buttonHeight};

    if (GuiButton(gameOverButton, "Reset"))
    {
        finishState = true;
    }
}

void drawGameState(const float deltaTime)
{
    drawBird(&gameState.bird);
    drawPipes(gameState.pipePool);

    DrawText(TextFormat("Score: %0i", gameState.score), 0, 100, 20, GREEN);

    if (deltaTime != 0)
    {
        DrawText(TextFormat("CURRENT FPS: %i", (int)(1.0f/deltaTime)),  0, 0, 20, GREEN);
        DrawText(TextFormat("ACCELERATION M/2^2: %i", (int)(gameState.bird.velocity.y * deltaTime - 0 / (1.0f/deltaTime))),  0, 50, 20, GREEN);
    }

    if (gameState.gameOver)
    {
        drawGameOverMenu();
    }
}
