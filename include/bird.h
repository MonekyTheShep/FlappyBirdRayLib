#pragma once
#include <raylib.h>
typedef struct Bird {
    Texture sprite;
    Rectangle src, hitBox;
    const float jumpVel, gravVel;
    Vector2 velocity;
} Bird;