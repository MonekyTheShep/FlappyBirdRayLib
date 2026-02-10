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
        .hitBox = (Rectangle) {0,0, 200, 200},
        .velocity = (Vector2) {0.0f, 0.0f}
    };

    Bird bird = {
        .sprite = LoadTexture(ASSETS_PATH"/flappy_bird.png"),
        .jumpVel = -250.0f,
        .gravVel = 200.0f,
        .velocity = (Vector2) {0.0f, 0.0f}
    };

    const float factor = 0.2f;
    bird.src = (Rectangle) {0,0, (float) bird.sprite.width, (float) bird.sprite.height};
    bird.hitBox = (Rectangle) {50, (float) GetScreenHeight() / 2, (float) bird.sprite.width * factor, (float) bird.sprite.height * factor};



    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();

        // Apply the Velocity Forces based on delta time
        bird.hitBox.y += bird.velocity.y * deltaTime;
        bird.hitBox.x += bird.velocity.x * deltaTime;

        // Apply a 10% friction to the velocity based on delta time
        const float decayRate = 0.1f;
        bird.velocity.x *= powf(1.0f - decayRate, deltaTime);

        // Constant Gravity based on delta time
        // Accelerates infinitely for now
        bird.velocity.y += bird.gravVel * deltaTime;

        // Use this later
        const int touchingEdges = (bird.hitBox.y > SCREEN_HEIGHT) || (bird.hitBox.y < 0);

        // Upward Force that resets previous velocity
        if (IsKeyPressed(KEY_SPACE)) bird.velocity.y = bird.jumpVel;


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
