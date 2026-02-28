#include "states/gamestate.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include <raygui.h>

#include "constants.h"
#include "pipe.h"
#include "bird.h"


bool gameOver = false;
int score = 0;

Bird bird;
Pipe pipePool[POOL_SIZE];
PipeTexture pipeTexture;

float accumulationTime = PIPE_SPAWN_RATE;

// Initialise Functions
void InitializeGameState(void)
{
    // Pipe declaring
    initializePipePool(pipePool, &pipeTexture);

    // Bird declaring
    initializeBird(&bird);
}

void UnloadGameState(void) {
    CleanUpBird(&bird);
    CleanUpPipes(&pipeTexture);
}

// Logic Functions
static void resetGame(Pipe *currentPipePool, Bird *currentBird, GameInfo *gameInfo, MenuStates *menuState)
{
    gameOver = false;
    score = 0;

    // Move bird back to starting position and reset velocity
    currentBird->position = (Vector2) {100.0f, (float) GetScreenHeight() / 2.0f};
    currentBird->velocity = (Vector2) {0.0f,0.0f};
    currentBird->rotationVel = 0.0f;
    currentBird->rotation = 0.0f;

    // Reset pipes
    for (int i = 0; i < POOL_SIZE; i++)
    {
        releasePipe(&currentPipePool[i]);
    }

    accumulationTime = PIPE_SPAWN_RATE;

    changeMenu(gameInfo, menuState, MAIN_MENU);
}

static void spawnPipe(void)
{
    if (accumulationTime >= PIPE_SPAWN_RATE)
    {
        Pipe *pipe = acquirePipe(pipePool);
        if (pipe != NULL)
        {
            pipe->position.y = (float) GetRandomValue(-100, 200);
        }
        accumulationTime = 0.0f;
    }
}

void incrementScore(void)
{
    score += 1;
}

void updateGameState(const float deltaTime)
{
    accumulationTime += deltaTime;

    if (!gameOver)
    {
        spawnPipe();
        handleBird(deltaTime, &bird);
        handlePipes(deltaTime, pipePool, &bird);
    }
}


// Draw Functions
static void drawGameOverMenu(GameInfo *gameInfo, MenuStates *menuState)
{
    const float buttonWidth = 100.0f;
    const float buttonHeight = 50.0f;
    const float gameOverButtonX = ((float) GetScreenWidth() - buttonWidth) / 2.0f;
    const float gameOverButtonY = ((float) GetScreenHeight() - buttonHeight) / 2.0f;
    const Rectangle gameOverButton = {gameOverButtonX,gameOverButtonY,buttonWidth,buttonHeight};

    if (GuiButton(gameOverButton, "Reset"))
    {
        resetGame(pipePool, &bird, gameInfo, menuState);
    }
}

void drawGameState(const float deltaTime, GameInfo *gameInfo, MenuStates *menuState)
{
    drawBird(&bird);
    drawPipes(pipePool);

    DrawText(TextFormat("Score: %0i", score), 0, 100, 20, GREEN);

    if (deltaTime != 0)
    {
        DrawText(TextFormat("CURRENT FPS: %i", (int)(1.0f/deltaTime)),  0, 0, 20, GREEN);
        DrawText(TextFormat("ACCELERATION M/2^2: %i", (int)(bird.velocity.y * deltaTime - 0 / (1.0f/deltaTime))),  0, 50, 20, GREEN);
    }

    if (gameOver)
    {
        drawGameOverMenu(gameInfo, menuState);
    }
}