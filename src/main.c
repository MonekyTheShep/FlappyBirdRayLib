#include <stdio.h>
#include <raylib.h>
#include <math.h>

#include "bird.h"

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (600)

#define WINDOW_TITLE "Flappy Bird"

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    Bird bird = {
        .sprite = LoadTexture(ASSETS_PATH"/flappybird.png"),
        .jumpVel = -300.0f,
        .gravVel = 200.0f,
        .velocity = (Vector2) {0.0f, 0.0f},
    };

    const float factor = 0.2f;
    bird.src = (Rectangle) {0,0, (float) bird.sprite.width, (float) bird.sprite.height};
    bird.rec = (Rectangle) {(float) GetScreenWidth() / 2, (float) GetScreenHeight() / 2, (float) bird.sprite.width * factor, (float) bird.sprite.height * factor};



    while (!WindowShouldClose())
    {

        const float deltaTime = GetFrameTime();

        // Constant Gravity based on delta time
        bird.velocity.y += bird.gravVel * deltaTime;

        // Use this later
        const int touchingEdges = (bird.rec.y > SCREEN_HEIGHT) || (bird.rec.y < 0);

        // Upward Force
        if (IsKeyPressed(KEY_SPACE)) bird.velocity.y += bird.jumpVel;

        // Apply a 10% friction to the velocity based on delta time
        const float decayRate = 0.1f;

        // Apply the Velocity Forces based on delta time
        bird.velocity.y *= powf(1.0f - decayRate, deltaTime);
        bird.velocity.x *= powf(1.0f - decayRate, deltaTime);

        // Move based on velocity
        bird.rec.y += bird.velocity.y * deltaTime;
        bird.rec.x += bird.velocity.x * deltaTime;


        printf("%f\n", bird.velocity.y);

        BeginDrawing();
        ClearBackground(RAYWHITE);


        DrawTexturePro(bird.sprite, bird.src, bird.rec, (Vector2) {0,0},0, WHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
