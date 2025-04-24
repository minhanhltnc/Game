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
///set up các tài nguyên game
    // BG
    ScrollingBackground background;
    background.setTexture(graphics.loadTexture(BACKGROUND_IMG));

    // MUSIC
    Mix_Music *gMusic = graphics.loadMusic(BACKGROUND_MUSIC);
    if (soundOn) graphics.play(gMusic);
    Mix_Chunk *gChunk_wing = graphics.loadSound(CHUNK_WING);
    Mix_Chunk *gChunk_point = graphics.loadSound(CHUNK_POINT);
    // Fonts
    TTF_Font* font = graphics.loadFont(FONT_NAME, 90);
    TTF_Font* fontScoreInGameOver = graphics.loadFont(FONT_NAME, 125);
    TTF_Font* fontScoreInPlaying = graphics.loadFont(FONT_NAME, 50);
    // Mouse
    Mouse mouse(START_POS, SCREEN_HEIGHT / 2);
    Sprite bird;
    SDL_Texture* birdTexture = graphics.loadTexture(BIRD_SPRITE_FILE);
    bird.init(birdTexture, BIRD_FRAMES, BIRD_CLIPS);
    //pipes
    vector<pipes> pipeList;
    SDL_Texture* pipeTexture = graphics.loadTexture(PIPES_IMG);
    initPipes(pipeList, pipeTexture);

    while (!quit) {
        graphics.prepareScene();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;
///các gamestate, tương tác với các nút ở menu và màn hình gameover
            if (currentState == MENU && event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                Button playBtn = createButton(PLAY_BUT, font, graphics, PLAY_OFFSET);
                string soundText = soundOn ? SOUND_ON_BUT : SOUND_OFF_BUT;
                Button soundBtn = createButton(soundText, font, graphics, SOUND_OFFSET);

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

                Button playAgainBtn = createButton(REPLAY_BUT, font, graphics, REPLAY_OFFSET);
                Button backBtn = createButton(BACK_MENU_BUT, font, graphics, BACK_OFFSET);

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

/// Render theo gamestate
        if (currentState == MENU)
        {
            renderMenu(graphics, font, soundOn);
        }
        else if (currentState == PLAYING) {
            if (!gameOver(mouse, pipeList)) {
                if (mouse.isLeftButtonPressed()) {
                    if (soundOn)
                    {
                        graphics.play(gChunk_wing,0);
                    }
                }
                ///update game
                mouse.move();
                bird.tick();
                updatePipes(pipeList);

                background.scroll(SCOLL_BG);
                graphics.render(mouse, bird);
                graphics.renderPipes(pipeList, pipeTexture);
                int presScore=mouse.score;
                mouse.updateScore(pipeList);
                if(presScore!=mouse.score&&soundOn)graphics.play(gChunk_point,1);
                graphics.renderScore(fontScoreInPlaying, mouse.score, 20, 20, {255, 255, 255, 255});
            }
            else {
                currentState = GAME_OVER;
            }
        }
        else if (currentState == GAME_OVER) {
            renderGameOver(graphics, font);
            graphics.renderScore(fontScoreInGameOver, mouse.score, 150, 120, {255, 255, 0, 255});
        }

        graphics.presentScene();
        SDL_Delay(10);
    }
///clear
    // Giải phóng textures
SDL_DestroyTexture(birdTexture);
SDL_DestroyTexture(pipeTexture);

    //Giải phóng âm thanh
    Mix_FreeChunk(gChunk_wing);
Mix_FreeChunk(gChunk_point);

    SDL_DestroyTexture(background.texture);
    TTF_CloseFont(font);
    TTF_CloseFont(fontScoreInPlaying);
    TTF_CloseFont(fontScoreInGameOver);
    if (gMusic != nullptr) Mix_FreeMusic(gMusic);
    graphics.quit();

    return 0;
}
