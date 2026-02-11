#pragma once
#include <raylib.h>
typedef struct Bird {
    Texture sprite;
    Vector2 position;
    Rectangle src, hitBox;
    const float jumpVel, gravVel;
    Vector2 velocity;
} Bird;

void handleBird(Bird *bird);