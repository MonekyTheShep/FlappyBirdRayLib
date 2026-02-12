#include "pipe.h"

#include "constants.h"

#include <math.h>
#include <stdio.h>

#include "states/gamestate.h"


static void drawHitBoxDebug(Pipe *pipe) {
    DrawRectangleRec(pipe->topHitBox, Fade(RED, 0.5f));
    DrawRectangleRec(pipe->middleHitBox, Fade(GREEN, 0.5f));
    DrawRectangleRec(pipe-> bottomHitBox, Fade(RED, 0.5f));
}

void drawPipe(Pipe *pipe)
{
    // Find the number of chunks to fill from top to top pipe
    const float calculateNumberOfTopChunks = (pipe->position.y) / pipe->pipeChunkSize.y;
    // printf("Top %f\n", ceilf(calculateNumberOfTopChunks));

    // Build top chunks
    for (int i = 0; i < (int) ceilf(calculateNumberOfTopChunks); i++)
    {
        // Position above the top pillar
        const float startingPosition = pipe->position.y - pipe->pipeChunkSize.y;
        const float yOffset = (float) i * pipe->pipeChunkSize.y;

        DrawTextureEx(pipe->pipeChunkTop, (Vector2) {pipe->position.x, startingPosition - yOffset}, 0.0f, 1.0f,  WHITE);
    }

    // Find difference between position and screen height.
    // Then remove the top pipe, middle, and bottom pipe to get the chunks.
    const float calculateNumberOfBottomChunks = ((float) GetScreenHeight() - pipe->position.y - (float) pipe->pipeTop.height - pipe->pipeGap - (float) pipe->pipeBottom.height) / pipe->pipeChunkSize.y;
    // printf("Bottom %f\n", ceilf(calculateNumberOfBottomChunks));

    // Build bottom chunks
    for (int i = 0; i < (int) ceilf(calculateNumberOfBottomChunks); i++)
    {
        // Position below the bottom pillar
        const float startingPosition = pipe->position.y + (float) pipe->pipeTop.height + pipe->pipeGap + (float) pipe->pipeBottom.height;
        const float yOffset = (float) i * pipe->pipeChunkSize.y;

        DrawTextureEx(pipe->pipeChunkBottom, (Vector2) {pipe->position.x, startingPosition + yOffset}, 0.0f, 1.0f,  WHITE);
    }

    // Draw top pipe
    DrawTextureEx(pipe->pipeTop, (Vector2) {pipe->position.x, pipe->position.y}, 0.0f, 1.0f,  WHITE);

    const float bottomPipeYOffset = pipe->pipeGap + (float) pipe->pipeTop.height;
    // Draw bottom pipe
    DrawTextureEx(pipe->pipeBottom, (Vector2) {pipe->position.x, pipe->position.y + bottomPipeYOffset}, 0.0f, 1.0f,  WHITE);


    // drawHitBoxDebug(pipe);
}

static void handleTopHitbox(Pipe *pipe)
{
    // Calculate position of hitbox
    pipe->topHitBox.x = pipe->position.x;
    // Position hitbox at top of pipe chunks
    pipe->topHitBox.y = 0;

    // Calculate scale of hitbox
    pipe->topHitBox.width = pipe->pipeChunkSize.x;
    // 0 - pipe->position.y + pipe->pipeTop.height. This covers the chunks and the pipe top itself
    pipe->topHitBox.height = pipe->position.y + (float) pipe->pipeTop.height;
}

static void handleMiddleHitbox(Pipe *pipe)
{
    pipe->middleHitBox.x = pipe->position.x;
    // Position middle hitbox after the pipeTop
    pipe->middleHitBox.y = pipe->position.y + (float) pipe->pipeTop.height;

    // Make it the width of pipeTop
    pipe->middleHitBox.width = (float) pipe->pipeTop.width;
    pipe->middleHitBox.height = pipe->pipeGap;
}

static void handleBottomHitbox(Pipe *pipe)
{
    pipe->bottomHitBox.x = pipe->position.x;
    // Position the hitbox at bottom pipe
    pipe->bottomHitBox.y =  pipe->position.y + (float) pipe->pipeTop.height + pipe->pipeGap;

    pipe->bottomHitBox.width = (float) pipe->pipeBottom.width;
    // Stretch hitbox to end of screen
    pipe->bottomHitBox.height = (float) GetScreenHeight();
}

static void applyVelocity(Pipe *pipe, float deltaTime)
{
    pipe->velocity.x = -5000.0f * deltaTime;
    pipe->position.x += pipe->velocity.x * deltaTime;
}


static void collisionHandling(Pipe *pipe, Bird *bird, int *gameOver) {
    const int offScreen = pipe->position.x + pipe->pipeChunkSize.x < 0;
    if (offScreen)
    {
        printf("remove");
        // move back to end of screen
        releasePipe(pipe);
    }

    const int birdHitPipe = CheckCollisionRecs(bird->hitBox, pipe->topHitBox) ||
    CheckCollisionRecs(bird->hitBox, pipe->bottomHitBox);

    if (birdHitPipe)
    {
        *gameOver = 1;
    }

    const int scoreCollided = CheckCollisionRecs(bird->hitBox, pipe->middleHitBox);

    // Each pipe stores if a score has been incremented.
    if (scoreCollided && pipe->scored != 1)
    {

        incrementScore();
        pipe->scored = 1;
    }
}



void initializePipePool(Pipe *pipePool)
{
    for (int i = 0; i < POOL_SIZE; i++)
    {
        pipePool[i].active = 0;
        initializePipe(&pipePool[i]);
    }
}

void initializePipe(Pipe *pipe)
{
    pipe->pipeBottom = LoadTexture(ASSETS_PATH"/pipe_bottom.png");
    pipe->pipeTop = LoadTexture(ASSETS_PATH"/pipe_top.png");
    pipe->pipeChunkTop = LoadTexture(ASSETS_PATH"/pipe_chunk_top.png");
    pipe->pipeChunkBottom = LoadTexture(ASSETS_PATH"/pipe_chunk_bottom.png");
    pipe->pipeChunkSize = (Vector2) {(float) pipe->pipeBottom.width, (float) pipe->pipeBottom.height};
    pipe->pipeGap = pipe->pipeChunkSize.y + 50;
    pipe->scored = 0;
    pipe->position = (Vector2) {(float) GetScreenWidth(), ((float) GetScreenHeight() / 2) - pipe->pipeChunkSize.y};
    pipe->velocity = (Vector2) {0.0f, 0.0f};
}

Pipe *acquirePipe(Pipe *pipePool)
{
    for (int i = 0; i < POOL_SIZE; i++)
    {
        if (&pipePool[i] != NULL) {
            if (!pipePool[i].active)
            {
                pipePool[i].active = 1;
                return &pipePool[i];
            }
        }
    }
    return NULL;
}

void releasePipe(Pipe *pipe)
{
    if (pipe != NULL)
    {
        pipe->active = 0;
        pipe->scored = 0;
        pipe->position = (Vector2) {(float) GetScreenWidth(), ((float) GetScreenHeight() / 2) - pipe->pipeChunkSize.y};
    }
}

void drawPipes(Pipe *pipePool) {
    for (int i = 0; i < POOL_SIZE; i++)
    {
        if (pipePool[i].active)
        {
            drawPipe(&pipePool[i]);
        }
    }
}

void handlePipes(Pipe *pipePool, Bird *bird, int *gameOver)
{
    const float deltaTime = GetFrameTime();
    for (int i = 0; i < POOL_SIZE; i++)
    {
        if (pipePool[i].active)
        {
            handleTopHitbox(&pipePool[i]);
            handleMiddleHitbox(&pipePool[i]) ;
            handleBottomHitbox(&pipePool[i]);
            collisionHandling(&pipePool[i], bird, gameOver);
            applyVelocity(&pipePool[i], deltaTime);
        }
    }
}
