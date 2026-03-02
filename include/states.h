#pragma once
#include "utility/gameutil.h"
#include "utility/menuutil.h"

// Game State
// Initialise Functions
void initializeGameState(void);
void unloadGameState(void);
bool finishGameState(void);

// Logic function
void updateGameState(float deltaTime);
// Draw Functions
void drawGameState(float deltaTime, GameInfo *gameInfo, MenuStates *menuState);


// Title State
// Initialise functions
void initializeTitleState(void);
void unloadTitleState(void);
bool finishTitleState(void);
//Logic Functions
void updateTitleState(void);
// Draw Functions
void drawTitleState(GameInfo *gameInfo, MenuStates *menuState);

