#include "bird.h"

#include "constants.h"

#include <stdio.h>
#include <math.h>


static void applyVelocity(Bird *bird, const float deltaTime)
{
    // Apply the Velocity Forces based on delta time
    bird->position.y += bird->velocity.y * deltaTime;
    bird->position.x += bird->velocity.x * deltaTime;

    // Apply the position to hitbox
    bird->hitBox.y = bird->position.y;
    bird->hitBox.x = bird->position.x;
}

static void applyFriction(Bird *bird, const float deltaTime)
{
    // Apply a 10% friction to the velocity based on delta time
    // Exponential decay
    const float decayRate = FRICTION;
    bird->velocity.x *= powf(1.0f - decayRate, deltaTime);
}

static void applyGravity(Bird *bird, const float deltaTime)
{
    // Constant Gravity based on delta time
    // Accelerates infinitely for now
    bird->velocity.y += bird->gravVel * deltaTime;
}

static int isTouchingCeiling(const Bird *bird)
{
    return (bird->hitBox.y <= 0);
}

static int isTouchingFloor(const Bird *bird)
{
    return (bird->hitBox.y + bird->hitBox.height >= (float) GetScreenHeight());
}

static void collisionHandling(Bird *bird)
{
    // Floor and Ceiling detection
    if (isTouchingFloor(bird))
    {
        // In the real game I will just make the bird gameover.
        // Prevent bird going underground
        bird->position.y = (float) GetScreenHeight() - bird->hitBox.height;
        // if the bird is going downwards then set velocity to 0
        if (bird->velocity.y > 0) bird->velocity.y = 0;
    }

    if (isTouchingCeiling(bird))
    {
        // if the bird is going upwards then set velocity to 0
        if (bird->velocity.y < 0) bird->velocity.y = 0;
    }
}

static void inputHandling(Bird *bird)
{
    // Upward Force that resets previous velocity
    if (IsKeyPressed(KEY_SPACE) && !isTouchingCeiling(bird))
    {
        bird->velocity.y = bird->jumpVel;
    }
}

void initializeBird(Bird *bird) {
    const float factor = 0.2f;
    bird->jumpVel = JUMP_VELOCITY;
    bird->gravVel = GRAVITY_VELOCITY;
    bird->velocity = (Vector2) {0,0};
    bird->sprite = LoadTexture(ASSETS_PATH"/flappy_bird.png");
    bird->src = (Rectangle) {0,0, (float) bird->sprite.width, (float) bird->sprite.height};
    bird->hitBox = (Rectangle) {bird->position.x, bird->position.y, (float) bird->sprite.width * factor, (float) bird->sprite.height * factor};
    bird->position = (Vector2) {50, (float) GetScreenHeight() / 2};
}

void handleBird(Bird *bird)
{
    const float deltaTime = GetFrameTime();
    // Handle Jumping
    inputHandling(bird);

    // Physics
    applyFriction(bird, deltaTime);
    applyGravity(bird, deltaTime);

    // Check if the bird is touching floor or ceiling
    collisionHandling(bird);

    // Apply velocity to position and hitbox
    applyVelocity(bird, deltaTime);
    // printf("%f\n", bird->velocity.y);
}

void drawBird(Bird *bird)
{
    DrawTexturePro(bird->sprite, bird->src, bird->hitBox, (Vector2) {0,0},0, WHITE);
}