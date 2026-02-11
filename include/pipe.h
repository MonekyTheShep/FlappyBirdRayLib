#pragma once
#include <raylib.h>
typedef struct Pipe {
    Texture pipeChunk, pipeBottom, pipeTop;
    Rectangle topHitBox, middleHitBox, bottomHitBox;
    float pipeGap;
    int active;
    Vector2 position;
    Vector2 velocity;
} Pipe;

void drawPipe(Pipe *pipe);
void handlePipe(Pipe *pipe);