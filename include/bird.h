#pragma once
#include <raylib.h>
typedef struct Bird {
    Texture sprite;
    Vector2 position;
    Rectangle src, hitBox;
    float jumpVel, gravVel;
    Vector2 velocity;
} Bird;

void handleBird(Bird *bird);
void drawBird(Bird *bird);
void initializeBird(Bird *bird);
