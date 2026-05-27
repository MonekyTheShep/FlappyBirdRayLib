#include "states/gamestate.h"
#include "states.h"

#include <stdio.h>
#include <stdbool.h>

#include <raygui.h>

#include "collision.h"

#include "events.h"

#include "constants.h"

//----------------------------------------------------------------------------------
// Module Local Variables
//----------------------------------------------------------------------------------
static int finishState = false;

static GameState gameState =
{
    .pipePool = {0},
    .bird = {0},
    .pipeTexture = {0},

    .gameOver = false,
    .score = 0
};

static float pipeAccumulatedTime = PIPE_SPAWN_RATE;

//----------------------------------------------------------------------------------
// Initialise Functions
//----------------------------------------------------------------------------------
void initializeGameState(void)
{
    finishState = false;

    gameState.gameOver = false;
    gameState.score = 0;

    pipeAccumulatedTime = PIPE_SPAWN_RATE;

    initializePipePool(gameState.pipePool, &gameState.pipeTexture);

    initializeBird(&gameState.bird);
}

void unloadGameState(void)
{
    cleanUpBird(&gameState.bird);
    cleanUpPipes(&gameState.pipeTexture);
}

bool finishGameState(void)
{
    return finishState;
}

//----------------------------------------------------------------------------------
// Logic Functions
//----------------------------------------------------------------------------------
#define PIPE_SPAWN_PADDING (20)
#define MIN_PIPE_Y (0 - (int) pipe->pipeTop->height)
#define MAX_PIPE_Y (GetScreenHeight() - (int) pipe->pipeTop->height - (int) pipe->pipeGap)

static void spawnPipe(const float deltaTime)
{
    pipeAccumulatedTime += deltaTime;
    if (pipeAccumulatedTime >= PIPE_SPAWN_RATE)
    {
        Pipe *pipe = acquirePipe(gameState.pipePool);
        if (pipe != NULL)
        {
            pipe->position.y = (float) GetRandomValue(MIN_PIPE_Y + PIPE_SPAWN_PADDING, MAX_PIPE_Y - PIPE_SPAWN_PADDING);
            scalePipe(pipe);
        }
        pipeAccumulatedTime = 0.0f;
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

static void handleCollisions(void)
{
    for (int i = 0; i < NUM_OF_PIPES; i++)
    {
        if (gameState.pipePool[i].active)
        {
            resolveBirdPipeCollisions(&gameState.pipePool[i], &gameState.bird);
        }
    }
}

void updateGameState(const float deltaTime)
{
    if (!gameState.gameOver)
    {
        spawnPipe(deltaTime);
        handleBird(deltaTime, &gameState.bird);
        handlePipes(deltaTime, gameState.pipePool);
        handleCollisions();
    }
}


//----------------------------------------------------------------------------------
// Draw Functions
//----------------------------------------------------------------------------------
static void drawGameOverMenu(void)
{
    const float buttonWidth = 100.0f;
    const float buttonHeight = 50.0f;
    const float gameOverButtonX = ((float) GetScreenWidth() - buttonWidth) / 2.0f;
    const float gameOverButtonY = ((float) GetScreenHeight() - buttonHeight) / 2.0f;

    const Rectangle gameOverButton = {
            .x = gameOverButtonX,
            .y = gameOverButtonY,
            .width = buttonWidth,
            .height = buttonHeight
    };

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
        DrawText(TextFormat("ACCELERATION M/2^2: %i", (int)(gameState.bird.velocity.y * deltaTime - 0 / deltaTime)), 0, 50, 20, GREEN);
    }

    if (gameState.gameOver)
    {
        drawGameOverMenu();
    }
}
