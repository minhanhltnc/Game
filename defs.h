
#ifndef _DEFS__H
#define _DEFS__H

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 800;
const int START_POS=300;
const double SCOLL_BG=3;
const char*  BIRD_SPRITE_FILE = "img\\extended_flappybird_sprite.png";
const int BIRD_CLIPS[4][4] = {
    {0, 0, 34, 24},
    {34, 0, 34, 24},
    {68, 0, 34, 24},
    {102, 0, 34, 24}
};

const int BIRD_FRAMES = sizeof(BIRD_CLIPS)/sizeof(int)/4;

#define WINDOW_TITLE "My game"
#define BACKGROUND_IMG "img\\bg.jpg"
//#define BIRD "flappy-bird-assets\\sprites\\yellowbird-midflap.png"

#endif
