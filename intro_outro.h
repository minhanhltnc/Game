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

// Enum game state
enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

// Button struct to store texture and position
struct Button {
    SDL_Texture* texture;
    int x, y, w, h;
};

/// Create button
Button createButton(const string& text, TTF_Font* font, Graphics& graphics, int yOffset) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Texture* texture = graphics.renderText(text.c_str(), font, white);
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);

    Button button;
    button.texture = texture;
    button.w = w;
    button.h = h;
    button.x = (SCREEN_WIDTH - w) / 2;
    button.y = (SCREEN_HEIGHT - h) / 2 + yOffset;//yOffset dịch chuyển y

    return button;
}

///destroy button
void destroyButton(Button& button) {
    SDL_DestroyTexture(button.texture);
}

/// render a button
void renderButton(Graphics& graphics, Button& button) {
    graphics.renderTexture(button.texture, button.x, button.y);
}

///render menu
void renderMenu(Graphics& graphics, TTF_Font* font, bool soundOn) {
    Button titleButton = createButton(TITLE, font, graphics, -50);
    Button playButton = createButton(PLAY_BUT, font, graphics, 100);

    string soundText = soundOn ? SOUND_ON_BUT : SOUND_OFF_BUT;
    Button soundButton = createButton(soundText, font, graphics, 180);

    /// Render buttons
    renderButton(graphics, titleButton);
    renderButton(graphics, playButton);
    renderButton(graphics, soundButton);

    destroyButton(titleButton);
    destroyButton(playButton);
    destroyButton(soundButton);
}

/// render game over screen
void renderGameOver(Graphics& graphics, TTF_Font* font) {
    Button overButton = createButton(GAMEOVER_BUT, font, graphics, -100);
    Button playAgainButton = createButton(REPLAY_BUT, font, graphics, 0);
    Button backToMenuButton = createButton(BACK_MENU_BUT, font, graphics, 100);

    // Render buttons
    renderButton(graphics, overButton);
    renderButton(graphics, playAgainButton);
    renderButton(graphics, backToMenuButton);

    destroyButton(overButton);
    destroyButton(playAgainButton);
    destroyButton(backToMenuButton);
}

bool isButtonClicked(int mouseX, int mouseY, const Button& button) {
    return (mouseX >= button.x && mouseX <= button.x + button.w &&
            mouseY >= button.y && mouseY <= button.y + button.h);
}

#endif
