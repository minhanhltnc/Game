
#ifndef _DEFS__H
#define _DEFS__H

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 800;
const int START_POS=300;
const double SCOLL_BG=3;

const int BIRD_CLIPS[4][4] = {
    {0, 0, 34, 24},
    {34, 0, 34, 24},
    {68, 0, 34, 24},
    {102, 0, 34, 24}
};
const int BIRD_FRAMES = sizeof(BIRD_CLIPS)/sizeof(int)/4;

///PIPES
const int PIPE_WIDTH = 52;
const int PIPE_HEIGHT=520;
const int PIPE_GAP = 80;
const double PIPE_SPEED = 2.0;
const int NUM_PIPES = 3;
const double START_X = 400;
const double PIPE_SPACING = 200; // khoảng cách giữa các pipes

//duong dan
#define BIRD_SPRITE_FILE  "img\\extended_flappybird_sprite.png"
#define WINDOW_TITLE "My game"
#define BACKGROUND_IMG "img\\bg.jpg"
#define BACKGROUND_MUSIC "sound\\background.mp3"
#define PIPES_IMG "img\\long_pipe_1_5x.png"
//di chuyen
#define RUN_SPEED 0
#define FALL_SPEED 4
#define UP_SPEED -8


#endif
