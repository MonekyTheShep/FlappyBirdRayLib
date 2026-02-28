#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include <raylib.h>


#include "utility/menuutil.h"
#include "utility/gameutil.h"
#include "utility/soundutil.h"

#include "states/gamestate.h"
#include "states/titlestate.h"

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (600)

#define WINDOW_TITLE ("Flappy Bird")

// default menu
MenuStates menuState = MAIN_MENU;
GameInfo gameInfo = {.musicPlaying = 0};

int main(void)
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    InitializeGameState();

    while (!WindowShouldClose())
    {
        if (menuState == EXIT)
        {
            break;
        }

        const float deltaTime = GetFrameTime();

        // Update logic for each state
        switch (menuState)
        {
            case GAME_MENU:
                updateGameState(deltaTime);
                break;
            default:
                break;
        }

        // Draw for each state
        BeginDrawing();
            ClearBackground(WHITE);
            switch (menuState)
            {
                case MAIN_MENU:
                    drawTitleState(&gameInfo, &menuState);
                    break;
                case GAME_MENU:
                    drawGameState(deltaTime, &gameInfo, &menuState);
                    break;
                default:
                    break;
            }
        EndDrawing();
    }


    UnloadGameState();
    CloseWindow();
    return 0;
}
