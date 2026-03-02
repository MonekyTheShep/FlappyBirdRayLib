#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include <raylib.h>


#include "utility/menuutil.h"
#include "utility/gameutil.h"
#include "utility/soundutil.h"

#include "states.h"

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (600)

#define WINDOW_TITLE ("Flappy Bird")

// default menu
States menuState = TITLE_STATE;
GameInfo gameInfo = {.musicPlaying = 0};

static void changeScreen(States changeState)
{
    switch (menuState)
    {
        case TITLE_STATE:
            unloadTitleState();
            break;
        case GAME_STATE:
            unloadGameState();
            break;
        default:
            break;
    }

    switch (changeState)
    {
        case TITLE_STATE:
            initializeTitleState();
            break;
        case GAME_STATE:
            initializeGameState();
            break;
        default:
            break;
    }

    gameInfo.musicPlaying = 0;
    menuState = changeState;
}


int main(void)
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

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
            case TITLE_STATE:
                updateTitleState();
                if (finishTitleState()) changeScreen(GAME_STATE);
                if (exitState()) menuState = EXIT;
                break;
            case GAME_STATE:
                updateGameState(deltaTime);
                if (finishGameState()) changeScreen(TITLE_STATE);
                break;
            default:
                break;
        }

        // Draw for each state
        BeginDrawing();
            ClearBackground(WHITE);
            switch (menuState)
            {
                case TITLE_STATE:
                    drawTitleState();
                    break;
                case GAME_STATE:
                    drawGameState(deltaTime);
                    break;
                default:
                    break;
            }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
