#ifndef _PIPES__H
#define _PIPES__H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "defs.h"
#include "graphics.h"

struct pipes {
    double  x;                // Tọa độ ngang (X) hiện tại của pipe
    int topHeight;          // Chiều cao phần ống trên (được random)
    int bottomHeight;
    pipes(double  startX) {
        x = startX;
        topHeight = -rand()%200;
        bottomHeight =   topHeight+PIPE_HEIGHT+PIPE_GAP;
    }

    void update() {
        x -= PIPE_SPEED;
        if(x<0)
        {x=SCREEN_WIDTH;
            topHeight = -rand()%300 ;
            bottomHeight =   topHeight+PIPE_HEIGHT+PIPE_GAP; }
        }
    bool isOffScreen() const {
        return x + PIPE_WIDTH < 0;
    }
};


#endif


