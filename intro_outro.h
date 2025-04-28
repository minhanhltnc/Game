#ifndef _INTRO_OUTRO_H
#define _INTRO_OUTRO_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include "graphics.h"
#include "defs.h"

#define TITLE "Flappy Bird"
#define PLAY_BUT "Play"
#define SOUND_ON_BUT "Sound: On"
#define SOUND_OFF_BUT "Sound: Off"
#define GAMEOVER_BUT "Game Over"
#define REPLAY_BUT "Play Again"
#define BACK_MENU_BUT "Back to Menu"

const int TITLE_OFFSET = -150;
const int PLAY_OFFSET = -50;
const int SOUND_OFFSET = 50;
const int COLOR_OFFSET = 150;
const int REPLAY_OFFSET = 0;
const int BACK_OFFSET = 100;
const int GAMEOVER_OFFSET = -100;

// Enum game state
enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

// Enum bird color
enum BirdColor {
    BLUE,
    RED,
    YELLOW
};

// Button struct
struct Button {
    SDL_Texture* texture;
    int x, y, w, h;
};

// Tạo button
Button createButton(const std::string& text, TTF_Font* font, Graphics& graphics, int yOffset) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Texture* texture = graphics.renderText(text.c_str(), font, white);
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);

    Button button;
    button.texture = texture;
    button.w = w;
    button.h = h;
    button.x = (SCREEN_WIDTH - w) / 2;
    button.y = (SCREEN_HEIGHT - h) / 2 + yOffset;
    return button;
}

// Hủy button
void destroyButton(Button& button) {
    SDL_DestroyTexture(button.texture);
}

// Vẽ button
void renderButton(Graphics& graphics, Button& button) {
    graphics.renderTexture(button.texture, button.x, button.y);
}

// Vẽ menu
void renderMenu(Graphics& graphics, TTF_Font* font, bool soundOn, BirdColor currentBirdColor) {
    Button titleButton = createButton(TITLE, font, graphics, TITLE_OFFSET);
    Button playButton = createButton(PLAY_BUT, font, graphics, PLAY_OFFSET);

    std::string soundText = soundOn ? SOUND_ON_BUT : SOUND_OFF_BUT;
    Button soundButton = createButton(soundText, font, graphics, SOUND_OFFSET);

    std::string colorText;
    if (currentBirdColor == BLUE) colorText = "Bird Color: Blue";
    else if (currentBirdColor == RED) colorText = "Bird Color: Red";
    else colorText = "Bird Color: Yellow";
    Button colorButton = createButton(colorText, font, graphics, COLOR_OFFSET);

    renderButton(graphics, titleButton);
    renderButton(graphics, playButton);
    renderButton(graphics, soundButton);
    renderButton(graphics, colorButton);

    destroyButton(titleButton);
    destroyButton(playButton);
    destroyButton(soundButton);
    destroyButton(colorButton);
}

// Vẽ màn hình Game Over
void renderGameOver(Graphics& graphics, TTF_Font* font) {
    Button overButton = createButton(GAMEOVER_BUT, font, graphics, GAMEOVER_OFFSET);
    Button playAgainButton = createButton(REPLAY_BUT, font, graphics, REPLAY_OFFSET);
    Button backToMenuButton = createButton(BACK_MENU_BUT, font, graphics, BACK_OFFSET);

    renderButton(graphics, overButton);
    renderButton(graphics, playAgainButton);
    renderButton(graphics, backToMenuButton);

    destroyButton(overButton);
    destroyButton(playAgainButton);
    destroyButton(backToMenuButton);
}

// Kiểm tra click
bool isButtonClicked(int mouseX, int mouseY, const Button& button) {
    return (mouseX >= button.x && mouseX <= button.x + button.w &&
            mouseY >= button.y && mouseY <= button.y + button.h);
}

#endif
