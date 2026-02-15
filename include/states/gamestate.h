#pragma once
#include "raylib.h"
#include "utility/menuutil.h"

void initializeGame(void);
void updateGameMenu(float deltaTime);
void incrementScore(void);
void drawGameMenu(float deltaTime, GameInfo *gameInfo, MenuStates *menuState);
void CleanUpGame(void);
