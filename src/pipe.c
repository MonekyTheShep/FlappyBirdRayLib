#include "pipe.h"

#include "constants.h"

#include <math.h>
#include <stdio.h>

#include "states/gamestate.h"

extern bool gameOver;

// Logic Functions
static void handleTopHitbox(Pipe *pipe)
{
    // Calculate position of hitbox
    pipe->topHitBox.x = pipe->position.x;
    // Position hitbox at top of pipe chunks
    pipe->topHitBox.y = 0.0f;

    // Calculate scale of hitbox
    pipe->topHitBox.width = pipe->pipeChunkSize.x;
    // 0 - pipe->position.y + pipe->pipeTop.height. This covers the chunks and the pipe top itself
    pipe->topHitBox.height = pipe->position.y + (float) pipe->pipeTop->height;
}

static void handleMiddleHitbox(Pipe *pipe)
{
    pipe->middleHitBox.x = pipe->position.x;
    // Position middle hitbox after the pipeTop
    pipe->middleHitBox.y = pipe->position.y + (float) pipe->pipeTop->height;

    // Make it the width of pipeTop
    pipe->middleHitBox.width = (float) pipe->pipeTop->width;
    pipe->middleHitBox.height = pipe->pipeGap;
}

static void handleBottomHitbox(Pipe *pipe)
{
    pipe->bottomHitBox.x = pipe->position.x;
    // Position the hitbox at bottom pipe
    pipe->bottomHitBox.y =  pipe->position.y + (float) pipe->pipeTop->height + pipe->pipeGap;

    pipe->bottomHitBox.width = (float) pipe->pipeBottom->width;
    // Stretch hitbox to end of screen
    pipe->bottomHitBox.height = (float) GetScreenHeight();
}

static void applyVelocity(Pipe *pipe, float deltaTime)
{
    pipe->velocity.x = PIPE_SPEED;
    pipe->position.x += pipe->velocity.x * deltaTime;
}


static void collisionHandling(Pipe *pipe, Bird *bird)
{
    const bool offScreen = pipe->position.x + pipe->pipeChunkSize.x < 0.0f;
    if (offScreen)
    {
        // move back to end of screen
        releasePipe(pipe);
    }

    const bool birdHitPipe = CheckCollisionRecs(bird->hitBox, pipe->topHitBox) ||
    CheckCollisionRecs(bird->hitBox, pipe->bottomHitBox);

    if (birdHitPipe)
    {
        gameOver = true;
    }

    const bool scoreCollided = CheckCollisionRecs(bird->hitBox, pipe->middleHitBox);

    // Each pipe stores if a score has been incremented.
    if (scoreCollided && !pipe->scored)
    {

        incrementScore();
        pipe->scored = true;
    }
}

// Pipe Initialiasing and Handling

static void initializePipe(Pipe *pipe, PipeTexture *pipeTexture)
{
    pipe->pipeBottom = &pipeTexture->pipeBottom;
    pipe->pipeTop = &pipeTexture->pipeTop;

    pipe->pipeChunkTop = &pipeTexture->pipeChunkTop;
    pipe->pipeChunkBottom = &pipeTexture->pipeChunkBottom;

    pipe->srcPipeChunkBottom = (Rectangle) {0.0f, 0.0f, (float) pipe->pipeChunkBottom->width, (float) pipe->pipeChunkBottom->height};
    pipe->srcPipeChunkTop = (Rectangle) {0.0f, 0.0f, (float) pipe->pipeChunkTop->width, (float) pipe->pipeChunkTop->height};

    pipe->pipeChunkSize = (Vector2) {(float) pipe->pipeBottom->width, (float) pipe->pipeBottom->height};

    pipe->pipeGap = pipe->pipeChunkSize.y + 50.0f;
    pipe->scored = false;
    pipe->position = (Vector2) {(float) GetScreenWidth(), ((float) GetScreenHeight())};
    pipe->velocity = (Vector2) {0.0f, 0.0f};
}

void initializePipePool(Pipe *pipePool, PipeTexture *pipeTexture)
{
    pipeTexture->pipeBottom = LoadTexture(ASSETS_PATH"/pipe_bottom.png");
    pipeTexture->pipeTop = LoadTexture(ASSETS_PATH"/pipe_top.png");
    pipeTexture->pipeChunkBottom = LoadTexture(ASSETS_PATH"/pipe_chunk_bottom.png");
    pipeTexture->pipeChunkTop = LoadTexture(ASSETS_PATH"/pipe_chunk_top.png");

    for (int i = 0; i < POOL_SIZE; i++)
    {
        pipePool[i].active = 0;
        initializePipe(&pipePool[i], pipeTexture);
    }
}

void CleanUpPipes(PipeTexture *pipeTexture) {
    UnloadTexture(pipeTexture->pipeBottom);
    UnloadTexture(pipeTexture->pipeTop);
    UnloadTexture(pipeTexture->pipeChunkTop);
    UnloadTexture(pipeTexture->pipeChunkBottom);
}

Pipe *acquirePipe(Pipe *pipePool)
{
    for (int i = 0; i < POOL_SIZE; i++)
    {
        if (!pipePool[i].active) {
            pipePool[i].active = true;
            return &pipePool[i];
        }
    }
    return NULL;
}

void releasePipe(Pipe *pipe)
{
    if (pipe != NULL)
    {
        pipe->active = false;
        pipe->scored = false;
        pipe->position = (Vector2) {(float) GetScreenWidth(), ((float) GetScreenHeight() / 2.0f)};
    }
}

void handlePipes(const float deltaTime, Pipe *pipePool, Bird *bird)
{
    for (int i = 0; i < POOL_SIZE; i++)
    {
        if (pipePool[i].active)
        {
            applyVelocity(&pipePool[i], deltaTime);
            handleTopHitbox(&pipePool[i]);
            handleMiddleHitbox(&pipePool[i]);
            handleBottomHitbox(&pipePool[i]);
            collisionHandling(&pipePool[i], bird);
        }
    }
}


// Draw Functions
static void drawHitBoxDebug(Pipe *pipe)
{
    DrawRectangleRec(pipe->topHitBox, Fade(RED, 0.5f));
    DrawRectangleRec(pipe->middleHitBox, Fade(GREEN, 0.5f));
    DrawRectangleRec(pipe-> bottomHitBox, Fade(RED, 0.5f));
}

void drawPipe(Pipe *pipe)
{
    // Find the number of chunks from top of screen to top pipe
    const float numberOfTopChunks = (pipe->position.y) / pipe->pipeChunkSize.y;

    if (numberOfTopChunks > 0.0f)
    {
        pipe->dstPipeChunkTop.x = pipe->position.x;
        pipe->dstPipeChunkTop.y = 0.0f;
        pipe->dstPipeChunkTop.width = (float) pipe->pipeChunkTop->width;
        pipe->dstPipeChunkTop.height = (float) pipe->pipeChunkTop->height * numberOfTopChunks;
        DrawTexturePro(*pipe->pipeChunkTop, pipe->srcPipeChunkTop, pipe->dstPipeChunkTop, (Vector2) {0.0f,0.0f}, 0.0f, WHITE);
    }

    // Find difference between position and screen height.
    // Then remove the top pipe, middle, and bottom pipe to get the chunks.
    const float numberOfBottomChunks = ((float) GetScreenHeight()
    - pipe->position.y
    - (float) pipe->pipeTop->height
    - pipe->pipeGap
    - (float) pipe->pipeBottom->height) / pipe->pipeChunkSize.y;

    if (numberOfBottomChunks > 0.0f)
    {
        pipe->dstPipeChunkBottom.x = pipe->position.x;
        const float pipeBottomChunkOffsetY = (float) pipe->pipeTop->height + pipe->pipeGap + (float) pipe->pipeBottom->height;
        pipe->dstPipeChunkBottom.y = pipe->position.y + pipeBottomChunkOffsetY;

        pipe->dstPipeChunkBottom.width = (float) pipe->pipeBottom->width;
        pipe->dstPipeChunkBottom.height = (float) pipe->pipeChunkTop->height * numberOfBottomChunks;

        DrawTexturePro(*pipe->pipeChunkBottom, pipe->srcPipeChunkBottom, pipe->dstPipeChunkBottom, (Vector2) {0.0f,0.0f}, 0.0f, WHITE);
    }

    // Draw top pipe
    DrawTextureEx(*pipe->pipeTop, (Vector2) {pipe->position.x, pipe->position.y}, 0.0f, 1.0f, WHITE);

    // Draw bottom pipe
    const float pipeBottomYOffset = pipe->pipeGap + (float) pipe->pipeTop->height;
    DrawTextureEx(*pipe->pipeBottom, (Vector2) {pipe->position.x, pipe->position.y + pipeBottomYOffset}, 0.0f, 1.0f,  WHITE);

    #ifdef debug
    drawHitBoxDebug(pipe);
    #endif
}

void drawPipes(Pipe *pipePool)
{
    for (int i = 0; i < POOL_SIZE; i++)
    {
        if (pipePool[i].active)
        {
            drawPipe(&pipePool[i]);
        }
    }
}