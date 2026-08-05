#pragma once

#include <stdbool.h>

#include <raylib.h>

//----------------------------------------------------------------------------------
// Typedefs
//----------------------------------------------------------------------------------
typedef struct PipeTexture
{
    Texture pipeChunk, pipeCap;
} PipeTexture;

typedef struct Pipe
{
    Texture *pipeChunk, *pipeCap;

    Rectangle srcPipeChunkBottom, srcPipeChunkTop;
    Rectangle dstPipeChunkBottom, dstPipeChunkTop;

    Rectangle srcPipeCapBottom, srcPipeCapTop;
    Rectangle dstPipeCapBottom, dstPipeCapTop;

    Rectangle topHitBox, middleHitBox, bottomHitBox;
    Vector2 position, velocity;
    float pipeGap;

    bool active, scored;
} Pipe;

//----------------------------------------------------------------------------------
// Initialise Functions
//----------------------------------------------------------------------------------
void initializePipePool(Pipe *pipePool, PipeTexture *pipeTexture);
void resetPipe(Pipe *pipe);
void scalePipe(Pipe *pipe);
void cleanUpPipes(PipeTexture *pipeTexture);

//----------------------------------------------------------------------------------
// Handle Functions
//----------------------------------------------------------------------------------
Pipe *acquirePipe(Pipe *pipePool);
void releasePipe(Pipe *pipe);
void handlePipes(float deltaTime, Pipe *pipe);

//----------------------------------------------------------------------------------
// Draw Functions
//----------------------------------------------------------------------------------
void drawPipe(Pipe *pipe);
void drawPipes(Pipe *pipe);
