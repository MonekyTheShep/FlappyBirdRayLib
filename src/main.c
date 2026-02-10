#include <stdio.h>
#include <raylib.h>
#include <math.h>

#include "bird.h"
#include "pipe.h"

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (600)

#define WINDOW_TITLE "Flappy Bird"

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    Pipe pipe = {
        .pipeBottom = LoadTexture(ASSETS_PATH"/pipe_bottom.png"),
        .pipeTop = LoadTexture(ASSETS_PATH"/pipe_top.png"),
        .pipeChunk = LoadTexture(ASSETS_PATH"/pipe_chunk.png"),
        .position = (Vector2) {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2},
        .velocity = (Vector2) {0.0f, 0.0f}
    };

    Bird bird = {
        .sprite = LoadTexture(ASSETS_PATH"/flappy_bird.png"),
        .jumpVel = -250.0f,
        .gravVel = 200.0f,
        .position = (Vector2) {50, SCREEN_HEIGHT / 2},
        .velocity = (Vector2) {0.0f, 0.0f}
    };

    const float factor = 0.2f;
    bird.src = (Rectangle) {0,0, (float) bird.sprite.width, (float) bird.sprite.height};
    bird.hitBox = (Rectangle) {bird.position.x, bird.position.y, (float) bird.sprite.width * factor, (float) bird.sprite.height * factor};



    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();

        // Use this later
        const int touchingFloor = (bird.hitBox.y + bird.hitBox.height >= SCREEN_HEIGHT);
        const int touchingCeiling = (bird.hitBox.y <= 0);

        // Floor and Ceiling detection
        if (touchingFloor) {
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
        DrawTextureEx(pipe.pipeChunk, (Vector2) {0,(float) GetScreenHeight() - pipe.pipeBottom.height / 2}, 0.0f, 0.5f,  WHITE);

        if (deltaTime != 0) {
            DrawText(TextFormat("CURRENT FPS: %i", (int)(1.0f/deltaTime)),  0, 0, 20, GREEN);
        }

        DrawRectangleRec(bird.hitBox, Fade(RED, 0.5f));
        DrawRectangleRec(pipe.hitBox, Fade(RED, 0.5f));

        DrawTexturePro(bird.sprite, bird.src, bird.hitBox, (Vector2) {0,0},0, WHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
