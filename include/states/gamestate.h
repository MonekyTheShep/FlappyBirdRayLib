#pragma once
#include "raylib.h"
#include "utility/menuutil.h"

void initializeGame(void);
void updateGameMenu(void);
void incrementScore(void);
void drawGameMenu(GameInfo *gameInfo, MenuStates *menuState);
void CleanUpGame(void);
