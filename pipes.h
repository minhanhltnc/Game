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
    SDL_Rect bottomRect;
    SDL_Rect topRect;
    pipes(double  startX,SDL_Texture* pipeTexture)
    {
        x = startX;
        topHeight = -rand()%200;
        bottomHeight =   topHeight+PIPE_HEIGHT+PIPE_GAP;
        int texW, texH;
        SDL_QueryTexture(pipeTexture, nullptr, nullptr, &texW, &texH);
        //
        bottomRect.x = x;
        bottomRect.y = bottomHeight;
        bottomRect.w = texW;
        bottomRect.h = texH;
        //
        topRect.x = x;
        topRect.y = topHeight;
        topRect.w = texW;
        topRect.h = texH;
    }

    void update()
    {
        x -= PIPE_SPEED;
        if(x<0)
        {
            x=SCREEN_WIDTH;
            topHeight = -rand()%300 ;
            bottomHeight =   topHeight+PIPE_HEIGHT+PIPE_GAP;

        }
        bottomRect.x = x;
        bottomRect.y = bottomHeight;
        topRect.x = x;
        topRect.y = topHeight;
    }
    bool isOffScreen() const {
        return x + PIPE_WIDTH < 0;
    }
};


#endif


