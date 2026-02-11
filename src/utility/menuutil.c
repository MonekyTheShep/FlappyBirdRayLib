#include "utility/menuutil.h"

void changeMenu(GameInfo *info, MenuStates *currentState, const MenuStates changeState)
{
    *currentState = changeState;
    info->musicPlaying = 0;
}