#include "pipe.h"

#include <stdio.h>

#include "constants.h"

#include <raymath.h>

//----------------------------------------------------------------------------------
// Initialise Functions
//----------------------------------------------------------------------------------
static void initializePipe(Pipe *pipe, PipeTexture *pipeTexture)
{
    pipe->pipeBottom = &pipeTexture->pipeBottom;
    pipe->pipeTop = &pipeTexture->pipeTop;
    pipe->pipeChunkTop = &pipeTexture->pipeChunkTop;
    pipe->pipeChunkBottom = &pipeTexture->pipeChunkBottom;

    pipe->srcPipeChunkBottom = (Rectangle) {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float) pipe->pipeChunkBottom->width,
        .height = (float) pipe->pipeChunkBottom->height
    };

    pipe->srcPipeChunkTop = (Rectangle) {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float) pipe->pipeChunkTop->width,
        .height = (float) pipe->pipeChunkTop->height
    };

    pipe->pipeGap = pipe->srcPipeChunkTop.height + 50.0f;
    resetPipe(pipe);
}

void initializePipePool(Pipe *pipePool, PipeTexture *pipeTexture)
{
    pipeTexture->pipeBottom = LoadTexture(ASSETS_PATH"/pipe_bottom.png");
    pipeTexture->pipeTop = LoadTexture(ASSETS_PATH"/pipe_top.png");
    pipeTexture->pipeChunkBottom = LoadTexture(ASSETS_PATH"/pipe_chunk_bottom.png");
    pipeTexture->pipeChunkTop = LoadTexture(ASSETS_PATH"/pipe_chunk_top.png");

    for (int i = 0; i < NUM_OF_PIPES; i++)
    {
        pipePool[i].active = false;
        initializePipe(&pipePool[i], pipeTexture);
    }
}

void resetPipe(Pipe *pipe)
{
    pipe->velocity = Vector2Zero();
    pipe->scored = false;

    pipe->position = (Vector2) {
            .x = (float) GetScreenWidth(),
            .y = ((float) GetScreenHeight())
    };

    Rectangle hitbox = (Rectangle) {
            .x = pipe->position.x,
            .y = pipe->position.y,
            .height = 0.0f,
            .width = 0.0f
    };

    Rectangle dst = (Rectangle) {0};
    pipe->dstPipeChunkBottom = pipe->dstPipeChunkTop = dst;

    pipe->topHitBox = pipe->middleHitBox = pipe->bottomHitBox = hitbox;
}

void scalePipe(Pipe *pipe)
{
    const float pipeChunkTopHeight = pipe->position.y;

    pipe->dstPipeChunkTop.x = pipe->position.x;
    pipe->dstPipeChunkTop.y = 0.0f;
    pipe->dstPipeChunkTop.width = (float) pipe->pipeTop->width;
    pipe->dstPipeChunkTop.height = fmaxf(0.0f, pipeChunkTopHeight);

    const float pipeChunkBottomHeight = ((float) GetScreenHeight()
            - pipe->position.y
            - (float) pipe->pipeTop->height
            - pipe->pipeGap
            - (float) pipe->pipeBottom->height);

    pipe->dstPipeChunkBottom.x = pipe->position.x;
    const float pipeBottomChunkOffsetY = (float) pipe->pipeTop->height + pipe->pipeGap + (float) pipe->pipeBottom->height;
    pipe->dstPipeChunkBottom.y = pipe->position.y + pipeBottomChunkOffsetY;
    pipe->dstPipeChunkBottom.width = (float) pipe->pipeBottom->width;
    pipe->dstPipeChunkBottom.height = fmaxf(0.0f, pipeChunkBottomHeight);
}

void cleanUpPipes(PipeTexture *pipeTexture)
{
    UnloadTexture(pipeTexture->pipeBottom);
    UnloadTexture(pipeTexture->pipeTop);
    UnloadTexture(pipeTexture->pipeChunkTop);
    UnloadTexture(pipeTexture->pipeChunkBottom);
}

//----------------------------------------------------------------------------------
// Logic Functions
//----------------------------------------------------------------------------------
static void handleTopHitbox(Pipe *pipe) // TODO scaling based on destination
{
    // Calculate position of hitbox
    pipe->topHitBox.x = pipe->position.x;
    // Position hitbox at top of pipe chunks
    pipe->topHitBox.y = 0.0f;

    // Calculate scale of hitbox
    pipe->topHitBox.width = (float) pipe->pipeTop->width;
    // 0 - pipe->position.y + pipe->pipeTop.height. This covers the chunks and the pipe top itself
    const float topHitBoxHeight = pipe->position.y + (float) pipe->pipeTop->height;
    pipe->topHitBox.height = fmaxf(0.0f, topHitBoxHeight);
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
    pipe->bottomHitBox.y = pipe->position.y + (float) pipe->pipeTop->height + pipe->pipeGap;

    pipe->bottomHitBox.width = (float) pipe->pipeBottom->width;

    const float bottomHitBoxHeight = (float) GetScreenHeight()
            - pipe->position.y
            - (float) pipe->pipeTop->height
            - pipe->pipeGap;

    // Stretch hitbox to end of screen
    pipe->bottomHitBox.height = fmaxf(0.0f, bottomHitBoxHeight);
}

static void applyVelocity(Pipe *pipe, float deltaTime)
{
    pipe->velocity.x = PIPE_SPEED;
    pipe->position.x += pipe->velocity.x * deltaTime;
}

static void handleCollision(Pipe *pipe)
{
    const bool offScreen = pipe->position.x + pipe->dstPipeChunkTop.width < 0.0f;
    if (offScreen)
    {
        // move back to end of screen
        releasePipe(pipe);
    }
}

static void movePipe(Pipe *pipe)
{
    pipe->dstPipeChunkTop.x = pipe->position.x;
    pipe->dstPipeChunkBottom.x = pipe->position.x;
}

//----------------------------------------------------------------------------------
// Handle Functions
//----------------------------------------------------------------------------------
Pipe *acquirePipe(Pipe *pipePool)
{
    for (int i = 0; i < NUM_OF_PIPES; i++)
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
        resetPipe(pipe);
    }
}

void handlePipes(const float deltaTime, Pipe *pipePool)
{
    for (int i = 0; i < NUM_OF_PIPES; i++)
    {
        if (pipePool[i].active)
        {
            applyVelocity(&pipePool[i], deltaTime);
            movePipe(&pipePool[i]);
            handleTopHitbox(&pipePool[i]);
            handleMiddleHitbox(&pipePool[i]);
            handleBottomHitbox(&pipePool[i]);
            handleCollision(&pipePool[i]);
        }
    }
}


//----------------------------------------------------------------------------------
// Draw Functions
//----------------------------------------------------------------------------------
static void drawHitBoxDebug(Pipe *pipe)
{
    DrawRectangleRec(pipe->topHitBox, Fade(RED, 0.5f));
    DrawRectangleRec(pipe->middleHitBox, Fade(GREEN, 0.5f));
    DrawRectangleRec(pipe-> bottomHitBox, Fade(RED, 0.5f));
}

void drawPipe(Pipe *pipe)
{
    DrawTexturePro(*pipe->pipeChunkTop, pipe->srcPipeChunkTop, pipe->dstPipeChunkTop, Vector2Zero(), 0.0f, WHITE);
    DrawTexturePro(*pipe->pipeChunkBottom, pipe->srcPipeChunkBottom, pipe->dstPipeChunkBottom, Vector2Zero(), 0.0f, WHITE);

    // Draw top pipe
    DrawTextureEx(*pipe->pipeTop, (Vector2) {.x = pipe->position.x, .y = pipe->position.y}, 0.0f, 1.0f, WHITE);

    // Draw bottom pipe
    const float pipeBottomYOffset = pipe->pipeGap + (float) pipe->pipeTop->height;
    DrawTextureEx(*pipe->pipeBottom, (Vector2) {.x = pipe->position.x, .y = pipe->position.y + pipeBottomYOffset}, 0.0f, 1.0f,  WHITE);

    #ifdef debug
    drawHitBoxDebug(pipe);
    #endif
}

void drawPipes(Pipe *pipePool)
{
    for (int i = 0; i < NUM_OF_PIPES; i++)
    {
        if (pipePool[i].active)
        {
            drawPipe(&pipePool[i]);
        }
    }
}
