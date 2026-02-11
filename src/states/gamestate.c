#include "states/gamestate.h"

#include <stdio.h>
#include <stdlib.h>

#include "constants.h"

#include "pipe.h"
#include "bird.h"


Bird bird = {
    .jumpVel = JUMP_VELOCITY,
    .gravVel = GRAVITY_VELOCITY
};

int gameOver = 0;

Pipe pipePool[POOL_SIZE];

void initializeGame(void)
{
    // Pipe declaring
    initializePipePool(pipePool);

    // Bird declaring
    const float factor = 0.2f;
    bird.sprite = LoadTexture(ASSETS_PATH"/flappy_bird.png");
    bird.src = (Rectangle) {0,0, (float) bird.sprite.width, (float) bird.sprite.height};
    bird.hitBox = (Rectangle) {bird.position.x, bird.position.y, (float) bird.sprite.width * factor, (float) bird.sprite.height * factor};
    bird.position = (Vector2) {50, (float) GetScreenHeight() / 2};
}

float accumulationTime = 3.5f;

static void spawnPipe(void)
{
    if (accumulationTime >= 3.5f)
    {
        Pipe *pipe = acquirePipe(pipePool);
        if (pipe != NULL)
        {
            pipe->active = 1;
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
