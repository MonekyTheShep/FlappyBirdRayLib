#include "collision.h"

#include <stdbool.h>

#include <raylib.h>

#include "bird.h"
#include "events.h"
#include "pipe.h"

void resolveBirdPipeCollisions(Pipe *pipe, Bird *bird)
{
    const bool birdHitPipe = CheckCollisionRecs(bird->hitBox, pipe->topHitBox) ||
                             CheckCollisionRecs(bird->hitBox, pipe->bottomHitBox);

    if (birdHitPipe) gameOver();

    const bool birdScored = CheckCollisionRecs(bird->hitBox, pipe->middleHitBox);

    // Each pipe stores if a score has been incremented.
    if (birdScored && !pipe->scored)
    {
        incrementScore();
        pipe->scored = true;
    }
}
