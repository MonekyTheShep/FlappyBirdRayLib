#include "states/gamestate.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "raygui.h"

#include "constants.h"

#include "pipe.h"
#include "bird.h"


int gameOver = 0;
int score = 0;

Bird bird;
Pipe pipePool[POOL_SIZE];

void initializeGame(void)
{
    // Pipe declaring
    initializePipePool(pipePool);

    // Bird declaring
    initializeBird(&bird);
}

static void resetGame(Pipe *currentPipePool, Bird *currentBird, GameInfo *gameInfo, MenuStates *menuState) {
    gameOver = 0;
    score = 0;
    gameInfo->musicPlaying = 0;

    // Move bird back to starting position and reset velocity
    currentBird->position = (Vector2) {50, (float) GetScreenHeight() / 2};
    currentBird->velocity = (Vector2) {0,0};

    // Reset pipes
    for (int i = 0; i < POOL_SIZE; i++) {
        releasePipe(&currentPipePool[i]);
    }

    *menuState = MAIN_MENU;

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

void updateGameMenu(void)
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

void incrementScore(void) {
    score += 1;
}

static void drawGameOverMenu(GameInfo *gameInfo, MenuStates *menuState)
{
    const float buttonWidth = 100;
    const float buttonHeight = 50;
    const float gameOverButtonX = ((float) GetScreenWidth() - buttonWidth) / 2;
    const float gameOverButtonY = ((float) GetScreenHeight() - buttonHeight) / 2;
    const Rectangle gameOverButton = {gameOverButtonX,gameOverButtonY,buttonWidth,buttonHeight};

    if (GuiButton(gameOverButton, "Reset"))
    {
        resetGame(pipePool, &bird, gameInfo, menuState);
    }
}

void drawGameMenu(GameInfo *gameInfo, MenuStates *menuState)
{
    const float deltaTime = GetFrameTime();

    drawBird(&bird);
    drawPipes(pipePool);

    DrawText(TextFormat("Score: %0i", score), 0, 100, 20, GREEN);

    if (deltaTime != 0)
    {
        DrawText(TextFormat("CURRENT FPS: %i", (int)(1.0f/deltaTime)),  0, 0, 20, GREEN);
        DrawText(TextFormat("ACCELERATION M/2^2: %i", (int)(bird.velocity.y * deltaTime - 0 / (1.0f/deltaTime))),  0, 50, 20, GREEN);
    }

    if (gameOver) {
        drawGameOverMenu(gameInfo, menuState);
    }
}
