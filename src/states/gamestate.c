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

        // Use this later
        const int touchingFloor = (bird.hitBox.y + bird.hitBox.height >= (float) GetScreenHeight());
        const int touchingCeiling = (bird.hitBox.y <= 0);

        // Floor and Ceiling detection
        if (touchingFloor) {
            // In the real game I will just make the bird gameover.
            // Prevent bird going underground
            bird.position.y = (float) GetScreenHeight() - bird.hitBox.height;
            // if the bird is going downwards then set velocity to 0
            if (bird.velocity.y > 0) bird.velocity.y = 0;
        }

        if (touchingCeiling) {
            // if the bird is going upwards then set velocity to 0
            if (bird.velocity.y < 0) bird.velocity.y = 0;
        }

        // Apply the Velocity Forces based on delta time
        bird.position.y += bird.velocity.y * deltaTime;
        bird.position.x += bird.velocity.x * deltaTime;

        // Apply the position to hitbox
        bird.hitBox.y = bird.position.y;
        bird.hitBox.x = bird.position.x;

        // Apply a 10% friction to the velocity based on delta time
        const float decayRate = 0.1f;
        bird.velocity.x *= powf(1.0f - decayRate, deltaTime);

        // Constant Gravity based on delta time
        // Accelerates infinitely for now
        bird.velocity.y += bird.gravVel * deltaTime;

        // Upward Force that resets previous velocity
        if (IsKeyPressed(KEY_SPACE) && !touchingCeiling) {
            bird.velocity.y = bird.jumpVel;
        }

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
