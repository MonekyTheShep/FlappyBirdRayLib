#pragma once
#include <raylib.h>
typedef struct Bird {
    float rotationVel;
    float rotation;
    Texture sprite;
    Vector2 position, velocity;
    Rectangle src, dest, hitBox;
} Bird;

void handleBird(float deltaTime, Bird *bird);
void drawBird(Bird *bird);
void initializeBird(Bird *bird);
void CleanUpBird(Bird *bird);
