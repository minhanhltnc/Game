#ifndef GAME_H
#define GAME_H
#include <vector>
#include "pipes.h"
#include "defs.h"
using namespace std;
struct Mouse {
    double x, y;
    bool isLeftButtonPressed()
    {
        Uint32 buttonState = SDL_GetMouseState(NULL, NULL);
        return (buttonState & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
    }
    void move() {
            x+=RUN_SPEED;
            if(isLeftButtonPressed()){y+=UP_SPEED;
            }
            y+=FALL_SPEED;
    }

};
void initPipes(vector<pipes>& pipeList)
{
    for (int i = 0; i < NUM_PIPES; ++i) {
        pipeList.emplace_back(START_X + i * PIPE_SPACING);
    }
}



bool gameOver(const Mouse& mouse) {
    return  mouse.x < 0 || mouse.x >= SCREEN_WIDTH ||
           mouse.y < 0 || mouse.y >= SCREEN_HEIGHT;
}

#endif // GAME_H
