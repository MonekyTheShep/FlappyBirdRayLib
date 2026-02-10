#pragma once

typedef struct Pipe {
    Texture pipeChunk, pipeBottom, pipeTop;
    Rectangle hitBox;
    Vector2 velocity;
} Pipe;