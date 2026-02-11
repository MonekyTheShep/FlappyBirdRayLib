#include "pipe.h"

#include <math.h>
#include <stdio.h>

void drawPipe(Pipe *pipe) {
    const float calculateNumberOfTopChunks = ceilf((pipe->position.y) / (float) pipe->pipeChunk.height);
    printf("%f\n", ceilf(calculateNumberOfTopChunks));

    // Build top chunks
    for (int i = 0; i <= (int) calculateNumberOfTopChunks; i++) {
        const float yOffset = (float) i * (float) pipe->pipeChunk.height;
        printf("%f\n", yOffset);

        DrawTextureEx(pipe->pipeChunk, (Vector2) {pipe->position.x, pipe->position.y - yOffset}, 0.0f, 1.0f,  WHITE);
    }

    // Draw top pipe
    DrawTextureEx(pipe->pipeTop, (Vector2) {pipe->position.x, pipe->position.y}, 0.0f, 1.0f,  WHITE);
}

static void handleTopHitbox(Pipe *pipe) {
    const float calculateNumberOfTopChunks = ceilf((pipe->position.y) / (float) pipe->pipeChunk.height);

    // Calculate position of hitbox
    pipe->topHitBox.x = pipe->position.x;
    // Position hitbox at top of pipe chunks
    pipe->topHitBox.y = pipe->position.y - (calculateNumberOfTopChunks * (float) pipe->pipeChunk.height);

    // Calculate scale of hitbox
    pipe->topHitBox.width = (float) pipe->pipeChunk.width;
    // 0 - pipe->position.y + pipe->pipeTop.height. This covers the chunks and the pipe top itself
    pipe->topHitBox.height = pipe->position.y + (float) pipe->pipeTop.height;
}

void handlePipe(Pipe *pipe) {
    handleTopHitbox(pipe);
}
