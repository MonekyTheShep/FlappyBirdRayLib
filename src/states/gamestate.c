#include "states/gamestate.h"

#include "math.h"
#include "pipe.h"
#include "bird.h"

Bird bird = {
    .jumpVel = -250.0f,
    .gravVel = 200.0f
};

Pipe pipe;

void initializeGame(void) {

    // Pipe declaring
    pipe.pipeBottom = LoadTexture(ASSETS_PATH"/pipe_bottom.png");
    pipe.pipeTop = LoadTexture(ASSETS_PATH"/pipe_top.png");
    pipe.pipeChunk = LoadTexture(ASSETS_PATH"/pipe_chunk.png");
    pipe.position = (Vector2) {(float) GetScreenWidth() / 2 + 40, (float) GetScreenHeight() / 2};
    pipe.velocity = (Vector2) {0.0f, 0.0f};

    // Bird declaring
    const float factor = 0.2f;
    bird.sprite = LoadTexture(ASSETS_PATH"/flappy_bird.png");
    bird.src = (Rectangle) {0,0, (float) bird.sprite.width, (float) bird.sprite.height};
    bird.hitBox = (Rectangle) {bird.position.x, bird.position.y, (float) bird.sprite.width * factor, (float) bird.sprite.height * factor};
    bird.position = (Vector2) {50, (float) GetScreenHeight() / 2};
}

void updateGameMenu(GameInfo *gameInfo, MenuStates *menuState) {
        const float deltaTime = GetFrameTime();

        handleBird(&bird);

        // Pipe code
        pipe.hitBox = (Rectangle) {pipe.position.x, pipe.position.y, 200, 200};

        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawTextureEx(pipe.pipeBottom, (Vector2) {0,(float) GetScreenHeight() / 2}, 0.0f, 0.5f,  WHITE);
            DrawTextureEx(pipe.pipeChunk, (Vector2) {0,(float) GetScreenHeight() - (float) pipe.pipeBottom.height / 2}, 0.0f, 0.5f,  WHITE);

            if (deltaTime != 0) {
                DrawText(TextFormat("CURRENT FPS: %i", (int)(1.0f/deltaTime)),  0, 0, 20, GREEN);
            }

            DrawRectangleRec(bird.hitBox, Fade(RED, 0.5f));
            DrawRectangleRec(pipe.hitBox, Fade(RED, 0.5f));

            DrawTexturePro(bird.sprite, bird.src, bird.hitBox, (Vector2) {0,0},0, WHITE);
        EndDrawing();
}
