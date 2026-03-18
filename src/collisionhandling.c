#include "collisionhandling.h"

#include "events.h"

void collisionHandling(Pipe *pipe, Bird *bird)
{
    const bool offScreen = pipe->position.x + pipe->pipeChunkSize.x < 0.0f;
    if (offScreen)
    {
        // move back to end of screen
        releasePipe(pipe);
    }

    const bool birdHitPipe = CheckCollisionRecs(bird->hitBox, pipe->topHitBox) ||
                             CheckCollisionRecs(bird->hitBox, pipe->bottomHitBox);

    if (birdHitPipe)
    {
        gameOver();
    }

    const bool scoreCollided = CheckCollisionRecs(bird->hitBox, pipe->middleHitBox);

    // Each pipe stores if a score has been incremented.
    if (scoreCollided && !pipe->scored)
    {

        incrementScore();
        pipe->scored = true;
    }
}
