#include "pipe.h"

#include <_abort.h>
#include <_stdio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <raymath.h>
#include <raylib.h>

#include "constants.h"

//----------------------------------------------------------------------------------
// Initialise Functions
//----------------------------------------------------------------------------------
static void initializePipe(Pipe *pipe, PipeTexture *pipeTexture)
{
    pipe->pipeChunk = &pipeTexture->pipeChunk;
    pipe->pipeCap = &pipeTexture->pipeCap;

    pipe->srcPipeChunkBottom = (Rectangle) {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float) pipe->pipeChunk->width,
        .height = (float) pipe->pipeChunk->height,
    };

    pipe->srcPipeChunkTop = (Rectangle) {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float) pipe->pipeChunk->width,
        .height = (float) -pipe->pipeChunk->height // flip the texture
    };

    pipe->srcPipeCapBottom = (Rectangle) {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float) pipe->pipeCap->width,
        .height = (float) pipe->pipeCap->height
    };

    pipe->srcPipeCapTop = (Rectangle) {
            .x = 0.0f,
            .y = 0.0f,
            .width = (float) pipe->pipeCap->width,
            .height = (float) -pipe->pipeCap->height // flip the texture
    };

    pipe->pipeGap = (float) pipe->pipeCap->height + 50.0f;
    resetPipe(pipe);
}

void initializePipePool(Pipe *pipePool, PipeTexture *pipeTexture)
{
    pipeTexture->pipeCap = LoadTexture(ASSETS_PATH"/pipe_cap.png");
    pipeTexture->pipeChunk = LoadTexture(ASSETS_PATH"/pipe_chunk.png");

    if (pipeTexture->pipeCap.id == 0 || pipeTexture->pipeChunk.id == 0)
    {
        fprintf(stderr, "Error loading resources!");
        abort();
    }

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

    Rectangle hitbox = {0};
    pipe->topHitBox = pipe->middleHitBox = pipe->bottomHitBox = hitbox;

    Rectangle dst = {0};
    pipe->dstPipeChunkBottom = pipe->dstPipeChunkTop = dst;

    Rectangle capDst = {
            .x = 0.0f,
            .y = 0.0f,
            .width = (float) pipe->pipeCap->width,
            .height = (float) pipe->pipeCap->height
    };

    pipe->dstPipeCapTop = pipe->dstPipeCapBottom = capDst;
}

static void scaleTopHitBox(Pipe *pipe)
{
    pipe->topHitBox.x = pipe->position.x;
    pipe->topHitBox.y = 0.0f;

    pipe->topHitBox.width = pipe->dstPipeChunkTop.width;
    pipe->topHitBox.height = fmaxf(0.0f, pipe->position.y + pipe->dstPipeCapTop.height);
}

static void scaleMiddleHitBox(Pipe *pipe)
{
    pipe->middleHitBox.x = pipe->position.x;
    pipe->middleHitBox.y = pipe->position.y + pipe->dstPipeCapTop.height;

    pipe->middleHitBox.width = pipe->dstPipeCapTop.width;
    pipe->middleHitBox.height = pipe->pipeGap;
}

static void scaleBottomHitbox(Pipe *pipe)
{
    pipe->bottomHitBox.x = pipe->position.x;
    pipe->bottomHitBox.y = pipe->dstPipeCapBottom.y;

    pipe->bottomHitBox.width = pipe->dstPipeCapBottom.width;
    pipe->bottomHitBox.height = fmaxf(0.0f, (float) GetScreenHeight() - pipe->dstPipeCapBottom.y);
}

static void scaleHitBox(Pipe *pipe)
{
    scaleTopHitBox(pipe);
    scaleMiddleHitBox(pipe);
    scaleBottomHitbox(pipe);
}

static void scaleTopPipeTexture(Pipe *pipe)
{
    pipe->dstPipeChunkTop.x = pipe->position.x;
    pipe->dstPipeChunkTop.y = 0.0f;
    pipe->dstPipeChunkTop.width = (float) pipe->pipeChunk->width;
    pipe->dstPipeChunkTop.height = fmaxf(0.0f, pipe->position.y);

    pipe->dstPipeCapTop.x = pipe->position.x;
    pipe->dstPipeCapTop.y = pipe->position.y;
}

static void scaleBottomPipeTexture(Pipe *pipe)
{
    const float pipeChunkBottomY = (pipe->position.y + (float) pipe->pipeCap->height
                                         + pipe->pipeGap
                                         + (float) pipe->pipeCap->height);

    pipe->dstPipeChunkBottom.x = pipe->position.x;
    pipe->dstPipeChunkBottom.y = pipeChunkBottomY;
    pipe->dstPipeChunkBottom.width = (float) pipe->pipeChunk->width;
    pipe->dstPipeChunkBottom.height = fmaxf(0.0f, (float) GetScreenHeight() - pipeChunkBottomY);

    pipe->dstPipeCapBottom.x = pipe->position.x;
    pipe->dstPipeCapBottom.y = pipeChunkBottomY - (float) pipe->pipeCap->height;
}

static void scalePipeTexture(Pipe *pipe)
{
    scaleTopPipeTexture(pipe);
    scaleBottomPipeTexture(pipe);
}

void scalePipe(Pipe *pipe)
{
    scalePipeTexture(pipe);
    scaleHitBox(pipe);
}

void cleanUpPipes(PipeTexture *pipeTexture)
{
    UnloadTexture(pipeTexture->pipeCap);
    UnloadTexture(pipeTexture->pipeChunk);
}

//----------------------------------------------------------------------------------
// Logic Functions
//----------------------------------------------------------------------------------
static void applyVelocity(Pipe *pipe, float deltaTime)
{
    pipe->velocity.x = PIPE_SPEED;
    pipe->position.x += pipe->velocity.x * deltaTime;
}

static void handleOffScreen(Pipe *pipe)
{
    const bool offScreen = pipe->position.x + pipe->dstPipeChunkTop.width < 0.0f;
    if (offScreen)
    {
        // move back to end of screen
        releasePipe(pipe);
    }
}

static void movePipeHitbox(Pipe *pipe)
{
    pipe->topHitBox.x = pipe->position.x;
    pipe->middleHitBox.x = pipe->position.x;
    pipe->bottomHitBox.x = pipe->position.x;
}

static void movePipeTexture(Pipe *pipe)
{
    pipe->dstPipeChunkTop.x = pipe->position.x;
    pipe->dstPipeChunkBottom.x = pipe->position.x;
    pipe->dstPipeCapTop.x = pipe->position.x;
    pipe->dstPipeCapBottom.x = pipe->position.x;
}

static void movePipe(Pipe *pipe)
{
    movePipeTexture(pipe);
    movePipeHitbox(pipe);
}

//----------------------------------------------------------------------------------
// Handle Functions
//----------------------------------------------------------------------------------
Pipe *acquirePipe(Pipe *pipePool)
{
    for (int i = 0; i < NUM_OF_PIPES; i++)
    {
        if (!pipePool[i].active) 
        {
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
            handleOffScreen(&pipePool[i]);
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
    DrawRectangleRec(pipe->bottomHitBox, Fade(RED, 0.5f));
}

void drawPipe(Pipe *pipe)
{
    DrawTexturePro(*pipe->pipeChunk, pipe->srcPipeChunkTop, pipe->dstPipeChunkTop, Vector2Zero(), 0.0f, WHITE);
    DrawTexturePro(*pipe->pipeChunk, pipe->srcPipeChunkBottom, pipe->dstPipeChunkBottom, Vector2Zero(), 0.0f, WHITE);

    DrawTexturePro(*pipe->pipeCap, pipe->srcPipeCapTop, pipe->dstPipeCapTop, Vector2Zero(), 0.0f, WHITE);
    DrawTexturePro(*pipe->pipeCap, pipe->srcPipeCapBottom, pipe->dstPipeCapBottom, Vector2Zero(), 0.0f, WHITE);

    #ifdef DEBUG
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
