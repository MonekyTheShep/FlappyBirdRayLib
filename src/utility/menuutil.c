#include "utility/menuutil.h"

void changeMenu(GameInfo *info, States *currentState, const States changeState)
{
    *currentState = changeState;
    info->musicPlaying = 0;
}
