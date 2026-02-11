#include "pipe.h"

#include <math.h>
#include <stdio.h>

void drawPipe(Pipe *pipe) {
    const float calculateNumberOfTopChunks = (pipe->position.y) / (float) pipe->pipeChunk.height;
    printf("Top %f\n", ceilf(calculateNumberOfTopChunks));

    // Build top chunks
    for (int i = 0; i <= (int) ceilf(calculateNumberOfTopChunks); i++) {
        const float yOffset = (float) i * (float) pipe->pipeChunk.height;

        DrawTextureEx(pipe->pipeChunk, (Vector2) {pipe->position.x, pipe->position.y - yOffset}, 0.0f, 1.0f,  WHITE);
    }

    const float calculateNumberOfBottomChunks = ((float) GetScreenHeight() - pipe->position.y - (float) pipe->pipeTop.height - pipe->pipeGap - (float) pipe->pipeBottom.height) / (float) pipe->pipeChunk.height;
    printf("Bottom %f\n", ceilf(calculateNumberOfBottomChunks));

    // Build bottom chunks
    for (int i = 0; i <= (int) ceilf(calculateNumberOfBottomChunks); i++) {
        const float yOffset = ((float) i * (float) pipe->pipeChunk.height) + (float) pipe->pipeTop.height + pipe->pipeGap + (float) pipe->pipeBottom.height;

        DrawTextureEx(pipe->pipeChunk, (Vector2) {pipe->position.x, pipe->position.y + yOffset}, 0.0f, 1.0f,  WHITE);
    }

    // Draw top pipe
    DrawTextureEx(pipe->pipeTop, (Vector2) {pipe->position.x, pipe->position.y}, 0.0f, 1.0f,  WHITE);

    const float bottomPipeYOffset = pipe->pipeGap + pipe->pipeTop.height;
    // Draw bottom pipe
    DrawTextureEx(pipe->pipeBottom, (Vector2) {pipe->position.x, pipe->position.y + bottomPipeYOffset}, 0.0f, 1.0f,  WHITE);

}

static void handleTopHitbox(Pipe *pipe) {
    const float calculateNumberOfTopChunks = ceilf((pipe->position.y) / (float) pipe->pipeChunk.height);

    // Calculate position of hitbox
    pipe->topHitBox.x = pipe->position.x;
    // Position hitbox at top of pipe chunks
    pipe->topHitBox.y = 0;

    // Calculate scale of hitbox
    pipe->topHitBox.width = (float) pipe->pipeChunk.width;
    // 0 - pipe->position.y + pipe->pipeTop.height. This covers the chunks and the pipe top itself
    pipe->topHitBox.height = pipe->position.y + (float) pipe->pipeTop.height;
}

static void handleMiddleHitbox(Pipe *pipe) {
    pipe->middleHitBox.x = pipe->position.x;
    // Position middle hitbox after the pipeTop
    pipe->middleHitBox.y = pipe->position.y + (float) pipe->pipeTop.height;

    // Make it the width of pipeTop
    pipe->middleHitBox.width = (float) pipe->pipeTop.width;
    pipe->middleHitBox.height = pipe->pipeGap;
}

static void handleBottomHitbox(Pipe *pipe) {
    pipe->bottomHitBox.x = pipe->position.x;
    // Position the hitbox at bottom pipe
    pipe->bottomHitBox.y =  pipe->position.y + (float) pipe->pipeTop.height + pipe->pipeGap;

    pipe->bottomHitBox.width = (float) pipe->pipeBottom.width;
    // Stretch hitbox to end of screen
    pipe->bottomHitBox.height = (float) GetScreenHeight();
}

void handlePipe(Pipe *pipe) {
    // pipe->position.x += -200 * GetFrameTime();
    handleTopHitbox(pipe);
    handleMiddleHitbox(pipe);
    handleBottomHitbox(pipe);
}
