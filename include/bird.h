#pragma once
#include <raylib.h>
typedef struct Bird {
    Texture sprite;
    Vector2 position, velocity;
    Rectangle src, dest, hitBox;
    float jumpVel;
} Bird;

void handleBird(float deltaTime, Bird *bird);
void drawBird(Bird *bird);
void initializeBird(Bird *bird);
