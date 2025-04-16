#ifndef _BACKGROUND__H
#define _BACKGROUND__H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "defs.h"
#include "graphics.h"

struct ScrollingBackground {
    SDL_Texture* texture;
    int scrollingOffset = 0;
    int width, height;

    void setTexture(SDL_Texture* _texture) {
        texture = _texture;
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    }

    void scroll(int  distance) {
        scrollingOffset -= distance;
        if( scrollingOffset <= 0 ) { scrollingOffset = width; }

    }
};


#endif

