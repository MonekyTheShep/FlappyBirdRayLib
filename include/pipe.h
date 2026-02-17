#pragma once
#include <raylib.h>

#include "bird.h"

typedef struct Pipe {
    Texture pipeChunkBottom, pipeChunkTop, pipeBottom, pipeTop;
    Vector2 pipeChunkSize;

    Rectangle srcPipeChunkBottom, srcPipeChunkTop;
    Rectangle dstPipeChunkBottom, dstPipeChunkTop;

    Rectangle topHitBox, middleHitBox, bottomHitBox;

    float pipeGap;
    int active;
    int scored;
    Vector2 position, velocity;
} Pipe;

void drawPipe(Pipe *pipe);
void initializePipePool(Pipe *pipePool);
void initializePipe(Pipe *pipe);
Pipe *acquirePipe(Pipe *pipePool);
void releasePipe(Pipe *pipe);
void drawPipes(Pipe *pipe);
void handlePipes(float deltaTime, Pipe *pipe, Bird *bird);
