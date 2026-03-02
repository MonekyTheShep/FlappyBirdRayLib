#pragma once
#include <stdbool.h>

//----------------------------------------------------------------------------------
// Typedefs
//----------------------------------------------------------------------------------
typedef enum States {
    TITLE_STATE,
    GAME_STATE,
    EXIT
} States;


//----------------------------------------------------------------------------------
// Title State
//----------------------------------------------------------------------------------
void initializeTitleState(void);
void unloadTitleState(void);
bool finishTitleState(void);
bool exitState(void);
void updateTitleState(void);
void drawTitleState(void);


//----------------------------------------------------------------------------------
// Game State
//----------------------------------------------------------------------------------
void initializeGameState(void);
void unloadGameState(void);
bool finishGameState(void);
void updateGameState(float deltaTime);
void drawGameState(float deltaTime);




