#pragma once
#include <stdbool.h>

typedef enum States
{
    TITLE_STATE,
    GAME_STATE,
    EXIT
} States;

// Title State
// Initialise functions
void initializeTitleState(void);
void unloadTitleState(void);
bool finishTitleState(void);
bool exitState(void);
//Logic Functions
void updateTitleState(void);
// Draw Functions
void drawTitleState(void);

// Game State
// Initialise Functions
void initializeGameState(void);
void unloadGameState(void);
bool finishGameState(void);

// Logic function
void updateGameState(float deltaTime);
// Draw Functions
void drawGameState(float deltaTime);




