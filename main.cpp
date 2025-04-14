#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "defs.h"
#include "game.h"
#include "backgrond.h"
#include "sprites.h"
using namespace std;

int main(int argc, char *argv[])
{
    Graphics graphics;
    graphics.init();
    ScrollingBackground background;
    background.setTexture(graphics.loadTexture(BACKGROUND_IMG));
    Mouse mouse;
    mouse.x =START_POS;
    mouse.y = SCREEN_HEIGHT / 2;

    Sprite bird;
    SDL_Texture* birdTexture = graphics.loadTexture(BIRD_SPRITE_FILE);
    bird.init(birdTexture, BIRD_FRAMES, BIRD_CLIPS);

    bool quit = false;
    SDL_Event event;
    while (!quit && !gameOver(mouse)) {
        graphics.prepareScene();
        background.scroll(SCOLL_BG);
        graphics.render(background);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quit = true;
        }
        mouse.move();
        bird.tick();
        graphics.render(mouse, bird);



        graphics.presentScene();
        SDL_Delay(10);
    }



    SDL_DestroyTexture( background.texture );

    graphics.quit();
    return 0;
}
