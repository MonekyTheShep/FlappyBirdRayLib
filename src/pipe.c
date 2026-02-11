#include "pipe.h"


void drawPipe(Pipe *pipe) {
    DrawTextureEx(pipe->pipeTop, (Vector2) {pipe->position.x,pipe->position.x}, 0.0f, 0.5f,  WHITE);
}

void handlePipe(Pipe *pipe) {

}
