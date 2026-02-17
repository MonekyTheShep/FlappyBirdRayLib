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
    bird->dest.y = bird->position.y;
    bird->dest.x = bird->position.x;

    // Apply rotation
    // bird->rotation += bird->rotationVel * deltaTime;
}

static void handleHitbox(Bird *bird) {
    // Position and Center Hitbox
    bird->hitBox.x = bird->position.x - bird->dest.width / 2.0f;
    bird->hitBox.y = bird->position.y - bird->dest.height / 2.0f;
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
    bird->velocity.y += GRAVITY_VELOCITY * deltaTime;

    // Rotation should go back down
    // if (bird->rotation >= 0.0f) {
    //     bird->rotationVel -= ROTATION_VELOCITY * -1 * deltaTime;
    // }
}

static int isTouchingCeiling(const Bird *bird)
{
    return (bird->hitBox.y <= 0.0f);
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
        bird->position.y = (float) GetScreenHeight() - bird->hitBox.height + bird->dest.height / 2;
        // if the bird is going downwards then set velocity to 0
        if (bird->velocity.y > 0.0f) bird->velocity.y = 0.0f;
    }

    if (isTouchingCeiling(bird))
    {
        // if the bird is going upwards then set velocity to 0
        if (bird->velocity.y < 0.0f) bird->velocity.y = 0.0f;
    }
}

static void inputHandling(Bird *bird)
{
    // Upward Force that resets previous velocity
    if (IsKeyPressed(KEY_SPACE) && !isTouchingCeiling(bird))
    {
        bird->velocity.y = JUMP_VELOCITY;
        bird->rotationVel += ROTATION_VELOCITY;
    }
}

void initializeBird(Bird *bird) {
    const float factor = 0.2f;
    bird->velocity = (Vector2) {0.0f,0.0f};
    bird->sprite = LoadTexture(ASSETS_PATH"/flappy_bird.png");
    bird->src = (Rectangle) {0.0f,0.0f, (float) bird->sprite.width, (float) bird->sprite.height};
    bird->dest = (Rectangle) {bird->position.x, bird->position.y, (float) bird->sprite.width * factor, (float) bird->sprite.height * factor};
    // Should I handle hitbox size here?
    bird->hitBox.width = bird->dest.width;
    bird->hitBox.height = bird->dest.height;
    bird->position = (Vector2) {100.0f, (float) GetScreenHeight() / 2};
}

void handleBird(const float deltaTime, Bird *bird)
{
    // Handle Jumping
    inputHandling(bird);

    // Physics
    applyFriction(bird, deltaTime);
    applyGravity(bird, deltaTime);

    // Apply velocity to position and destination
    applyVelocity(bird, deltaTime);

    handleHitbox(bird);

    // Check if the bird is touching floor or ceiling
    collisionHandling(bird);
    // printf("%f\n", bird->velocity.y);
}

static void drawHitBoxDebug(Bird *bird)
{
    DrawRectangleRec(bird->hitBox, Fade(RED, 0.5f));
}

void drawBird(Bird *bird)
{
    drawHitBoxDebug(bird);
    DrawTexturePro(bird->sprite, bird->src, bird->dest, (Vector2) {bird->dest.width / 2.0f,bird->dest.height / 2.0f}, bird->rotation, WHITE);
}
