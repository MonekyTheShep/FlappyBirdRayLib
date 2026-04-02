#include "pipe.h"

#include <stdio.h>

#include "constants.h"

//----------------------------------------------------------------------------------
// Initialise Functions
//----------------------------------------------------------------------------------
static void initializePipe(Pipe *pipe, PipeTexture *pipeTexture)
{
    pipe->position = (Vector2) {
        .x = (float) GetScreenWidth(),
        .y = ((float) GetScreenHeight())
    };

    pipe->velocity = (Vector2) {
        .x = 0.0f,
        .y = 0.0f
    };

    pipe->scored = false;

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

    pipe->pipeChunkSize = (Vector2) {
        .x = (float) pipe->pipeBottom->width,
        .y = (float) pipe->pipeBottom->height
    };

    pipe->pipeGap = pipe->pipeChunkSize.y + 50.0f;
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

void cleanUpPipes(PipeTexture *pipeTexture, Pipe *pipePool)
{
    UnloadTexture(pipeTexture->pipeBottom);
    UnloadTexture(pipeTexture->pipeTop);
    UnloadTexture(pipeTexture->pipeChunkTop);
    UnloadTexture(pipeTexture->pipeChunkBottom);

    // Reset pipes
    for (int i = 0; i < NUM_OF_PIPES; i++)
    {
        releasePipe(&pipePool[i]);
    }
}

//----------------------------------------------------------------------------------
// Logic Functions
//----------------------------------------------------------------------------------
static void handleTopHitbox(Pipe *pipe)
{
    // Calculate position of hitbox
    pipe->topHitBox.x = pipe->position.x;
    // Position hitbox at top of pipe chunks
    pipe->topHitBox.y = 0.0f;

    // Calculate scale of hitbox
    pipe->topHitBox.width = pipe->pipeChunkSize.x;
    // 0 - pipe->position.y + pipe->pipeTop.height. This covers the chunks and the pipe top itself
    const float topHitBoxHeight = pipe->position.y + (float) pipe->pipeTop->height;
    pipe->topHitBox.height = (topHitBoxHeight > 0.0f) ? topHitBoxHeight : 0.0f;
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
    pipe->bottomHitBox.height = (bottomHitBoxHeight > 0.0f) ? bottomHitBoxHeight : 0.0f;
}

static void applyVelocity(Pipe *pipe, float deltaTime)
{
    pipe->velocity.x = PIPE_SPEED;
    pipe->position.x += pipe->velocity.x * deltaTime;
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
        pipe->scored = false;
        pipe->position = (Vector2) {
            .x = (float) GetScreenWidth(),
            .y = ((float) GetScreenHeight())
        };
    }
}

void handlePipes(const float deltaTime, Pipe *pipePool)
{
    for (int i = 0; i < NUM_OF_PIPES; i++)
    {
        if (pipePool[i].active)
        {
            applyVelocity(&pipePool[i], deltaTime);
            handleTopHitbox(&pipePool[i]);
            handleMiddleHitbox(&pipePool[i]);
            handleBottomHitbox(&pipePool[i]);
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
    const float pipeChunkTopHeight = (pipe->position.y);

    if (pipeChunkTopHeight > 0.0f)
    {
        pipe->dstPipeChunkTop.x = pipe->position.x;
        pipe->dstPipeChunkTop.y = 0.0f;
        pipe->dstPipeChunkTop.width = (float) pipe->pipeChunkTop->width;
        pipe->dstPipeChunkTop.height = pipeChunkTopHeight;
        DrawTexturePro(*pipe->pipeChunkTop, pipe->srcPipeChunkTop, pipe->dstPipeChunkTop, (Vector2) {0.0f,0.0f}, 0.0f, WHITE);
    }

    // Find difference between position and screen height.
    // Then remove the top pipe, middle, and bottom pipe to get the height.
    const float pipeChunkBottomHeight = ((float) GetScreenHeight()
    - pipe->position.y
    - (float) pipe->pipeTop->height
    - pipe->pipeGap
    - (float) pipe->pipeBottom->height);

    if (pipeChunkBottomHeight > 0.0f)
    {
        pipe->dstPipeChunkBottom.x = pipe->position.x;
        const float pipeBottomChunkOffsetY = (float) pipe->pipeTop->height + pipe->pipeGap + (float) pipe->pipeBottom->height;
        pipe->dstPipeChunkBottom.y = pipe->position.y + pipeBottomChunkOffsetY;

        pipe->dstPipeChunkBottom.width = (float) pipe->pipeBottom->width;
        pipe->dstPipeChunkBottom.height = pipeChunkBottomHeight;

        DrawTexturePro(*pipe->pipeChunkBottom, pipe->srcPipeChunkBottom, pipe->dstPipeChunkBottom, (Vector2) {0.0f,0.0f}, 0.0f, WHITE);
    }

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
