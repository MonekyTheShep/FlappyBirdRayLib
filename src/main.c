#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include <raylib.h>


#include "utility/menuutil.h"
#include "utility/gameutil.h"
#include "utility/soundutil.h"

#include "states/gamestate.h"
#include "states/mainmenustate.h"

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (600)

#define WINDOW_TITLE "Flappy Bird"

int main(void)
{
    // default menu
    MenuStates menuState = MAIN_MENU;
    GameInfo gameInfo = {.musicPlaying = 0};

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    initializeGame();

    while (!WindowShouldClose())
    {
        if (menuState == EXIT)
        {
            break;
        }

        switch (menuState)
        {
            case MAIN_MENU:
                updateMainMenu(&gameInfo, &menuState);
                break;
            case GAME_MENU:
                updateGameMenu(&gameInfo, &menuState);
                break;
            default:
                break;
        }
    }

    CloseWindow();

    return 0;
}
