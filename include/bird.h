#pragma once
#include <raylib.h>
typedef struct Bird {
    Texture sprite;
    Rectangle src, dest, hitBox;
    Vector2 position, velocity;

    float rotationVel;
    float rotation;
} Bird;

// Bird initialising
void initializeBird(Bird *bird);
void CleanUpBird(Bird *bird);

// Bird Handling
void handleBird(float deltaTime, Bird *bird);

// Draw Functions
void drawBird(Bird *bird);

