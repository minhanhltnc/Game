#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include "graphics.h"
#include "defs.h"
#include "game.h"
#include "backgrond.h"
#include "sprites.h"
#include "pipes.h"
#include "intro_outro.h"

using namespace std;

int main(int argc, char *argv[]) {
    Graphics graphics;
    graphics.init();
    SDL_Event event;
    bool quit = false;
    GameState currentState = MENU;
    bool soundOn = true;

    /// BG
    ScrollingBackground background;
    background.setTexture(graphics.loadTexture(BACKGROUND_IMG));

    /// MUSIC
    Mix_Music *gMusic = graphics.loadMusic(BACKGROUND_MUSIC);
    if (soundOn) graphics.play(gMusic);

    /// Fonts
    TTF_Font* font = graphics.loadFont("img//FlappyBirdRegular-9Pq0.ttf", 100);
    TTF_Font* fontScoreInGameOver = graphics.loadFont("img//FlappyBirdRegular-9Pq0.ttf", 120);
    TTF_Font* fontScoreInPlaying = graphics.loadFont("img//FlappyBirdRegular-9Pq0.ttf", 50);

    /// Buttons
    Button playButton = createButton("Play", font, graphics, 100);
    Button playAgainButton = createButton("Play Again", font, graphics, 0);
    Button backToMenuButton = createButton("Back to Menu", font, graphics, 100);

    /// Mouse
    Mouse mouse(START_POS, SCREEN_HEIGHT / 2);
    Sprite bird;
    SDL_Texture* birdTexture = graphics.loadTexture(BIRD_SPRITE_FILE);
    bird.init(birdTexture, BIRD_FRAMES, BIRD_CLIPS);

    vector<pipes> pipeList;
    SDL_Texture* pipeTexture = graphics.loadTexture(PIPES_IMG);
    initPipes(pipeList, pipeTexture);

    while (!quit) {
        graphics.prepareScene();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;

            if (currentState == MENU && event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                Button playBtn = createButton("Play", font, graphics, 100);
                string soundText = soundOn ? "Sound: On" : "Sound: Off";
                Button soundBtn = createButton(soundText, font, graphics, 200);

                if (isButtonClicked(mouseX, mouseY, playBtn)) {
                    currentState = PLAYING;
                }
                else if (isButtonClicked(mouseX, mouseY, soundBtn)) {
                    soundOn = !soundOn;
                    if (soundOn) graphics.play(gMusic);
                    else Mix_HaltMusic();
                }

                destroyButton(playBtn);
                destroyButton(soundBtn);
            }
            else if (currentState == GAME_OVER && event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                Button playAgainBtn = createButton("Play Again", font, graphics, 0);
                Button backBtn = createButton("Back to Menu", font, graphics, 100);

                if (isButtonClicked(mouseX, mouseY, playAgainBtn)) {
                    currentState = PLAYING;
                    mouse.setPos(START_POS, SCREEN_HEIGHT / 2);
                    pipeList.clear();
                    initPipes(pipeList, pipeTexture);
                }
                else if (isButtonClicked(mouseX, mouseY, backBtn)) {
                    currentState = MENU;
                    mouse.setPos(START_POS, SCREEN_HEIGHT / 2);
                    pipeList.clear();
                    initPipes(pipeList, pipeTexture);
                }

                destroyButton(playAgainBtn);
                destroyButton(backBtn);
            }
        }

        graphics.render(background);

        // Render menu based on the game state
        if (currentState == MENU) {
            renderMenu(graphics, font, soundOn);
        }
        else if (currentState == PLAYING) {
            if (!gameOver(mouse, pipeList)) {
                if (mouse.isLeftButtonPressed()) {
                    if (soundOn) {
                        Mix_Chunk *gChunk = graphics.loadSound("sound\\audio\\wing.wav");
                        graphics.play(gChunk);
                    }
                }

                mouse.move();
                bird.tick();
                updatePipes(pipeList);
                background.scroll(SCOLL_BG);
                graphics.render(mouse, bird);
                graphics.renderPipes(pipeList, pipeTexture);
                mouse.updateScore(pipeList);
                graphics.renderScore(fontScoreInPlaying, mouse.score, 20, 20, {255, 255, 255, 255});
            } else {
                currentState = GAME_OVER;
            }
        }
        else if (currentState == GAME_OVER) {
            renderGameOver(graphics, font);
            graphics.renderScore(fontScoreInGameOver, mouse.score, 100, 120, {255, 255, 0, 255});
        }

        graphics.presentScene();
        SDL_Delay(10);
    }

    SDL_DestroyTexture(background.texture);
    TTF_CloseFont(font);
    TTF_CloseFont(fontScoreInPlaying);
    TTF_CloseFont(fontScoreInGameOver);
    if (gMusic != nullptr) Mix_FreeMusic(gMusic);
    graphics.quit();

    return 0;
}
