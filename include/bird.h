#pragma once
#include <raylib.h>
typedef struct Bird {
    Texture sprite;
    Rectangle src, rec;
    const float jumpVel, gravVel;
    Vector2 velocity;
} Bird;