#ifndef GAME_H
#define GAME_H
struct Mouse {
    int x, y;
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



bool gameOver(const Mouse& mouse) {
    return  mouse.x < 0 || mouse.x >= SCREEN_WIDTH ||
           mouse.y < 0 || mouse.y >= SCREEN_HEIGHT;
}

#endif // GAME_H
