#pragma once
#include <raylib.h>
typedef struct Bird {
    Texture sprite;
    Rectangle src, dest, hitBox;
    Vector2 position, velocity;

    float rotationVel;
    float rotation;
} Bird;

void handleBird(float deltaTime, Bird *bird);
void drawBird(Bird *bird);
void initializeBird(Bird *bird);
void CleanUpBird(Bird *bird);
