#pragma once
#include <raylib.h>

#include "bird.h"
#include <stdbool.h>

typedef struct PipeTexture {
    Texture pipeChunkBottom, pipeChunkTop, pipeBottom, pipeTop;
} PipeTexture;

typedef struct Pipe {
    Texture *pipeChunkBottom, *pipeChunkTop, *pipeBottom, *pipeTop;

    Rectangle srcPipeChunkBottom, srcPipeChunkTop;
    Rectangle dstPipeChunkBottom, dstPipeChunkTop;
    Rectangle topHitBox, middleHitBox, bottomHitBox;
    Vector2 position, velocity;
    Vector2 pipeChunkSize;
    float pipeGap;

    bool active;
    bool scored;
} Pipe;

void initializePipePool(Pipe *pipePool, PipeTexture *pipeTexture);
void CleanUpPipes(PipeTexture *pipeTexture);
Pipe *acquirePipe(Pipe *pipePool);
void releasePipe(Pipe *pipe);


void handlePipes(float deltaTime, Pipe *pipe, Bird *bird);

void drawPipe(Pipe *pipe);
void drawPipes(Pipe *pipe);


