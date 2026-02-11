#include "states/gamestate.h"

#include <stdio.h>
#include "constants.h"

#include "pipe.h"
#include "bird.h"

Bird bird = {
    .jumpVel = JUMP_VELOCITY,
    .gravVel = GRAVITY_VELOCITY
};

Pipe pipe;

void initializeGame(void)
{
    // Pipe declaring
    pipe.pipeBottom = LoadTexture(ASSETS_PATH"/pipe_bottom.png");
    pipe.pipeTop = LoadTexture(ASSETS_PATH"/pipe_top.png");
    pipe.pipeChunk = LoadTexture(ASSETS_PATH"/pipe_chunk.png");
    pipe.position = (Vector2) {(float) GetScreenWidth() / 2, (float) GetScreenHeight() / 2};
    pipe.velocity = (Vector2) {0.0f, 0.0f};

    // Bird declaring
    const float factor = 0.2f;
    bird.sprite = LoadTexture(ASSETS_PATH"/flappy_bird.png");
    bird.src = (Rectangle) {0,0, (float) bird.sprite.width, (float) bird.sprite.height};
    bird.hitBox = (Rectangle) {bird.position.x, bird.position.y, (float) bird.sprite.width * factor, (float) bird.sprite.height * factor};
    bird.position = (Vector2) {50, (float) GetScreenHeight() / 2};
}

void updateGameMenu(GameInfo *gameInfo, MenuStates *menuState) {
    handleBird(&bird);
    handlePipe(&pipe);
}

static void drawHitBoxDebug(void) {
    DrawRectangleRec(bird.hitBox, Fade(RED, 0.5f));
    DrawRectangleRec(pipe.topHitBox, Fade(RED, 0.5f));
}

void drawGameMenu(void) {
    const float deltaTime = GetFrameTime();
    if (deltaTime != 0)
    {
        DrawText(TextFormat("CURRENT FPS: %i", (int)(1.0f/deltaTime)),  0, 0, 20, GREEN);
        DrawText(TextFormat("ACCELERATION M/2^2: %i", (int)(bird.velocity.y * deltaTime - 0 / (1.0f/deltaTime))),  0, 50, 20, GREEN);
    }

    drawHitBoxDebug();
    drawBird(&bird);
    drawPipe(&pipe);
}
