#pragma once
#include "raylib.h"
#include "utility/menuutil.h"

// Initialise Functions
void InitializeGameState(void);
void UnloadGameState(void);

// Logic Functions
void incrementScore(void);
void updateGameState(float deltaTime);

// Draw Functions
void drawGameState(float deltaTime, GameInfo *gameInfo, MenuStates *menuState);

