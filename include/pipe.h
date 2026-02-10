#pragma once

typedef struct Pipe {
    Texture pipeChunk, pipeBottom, pipeTop;
    Vector2 position;
    Rectangle hitBox;
    Vector2 velocity;
} Pipe;