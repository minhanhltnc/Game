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
#include "highscore.h"

using namespace std;

int main(int argc, char* argv[]) {
    Graphics graphics;
    graphics.init();
    SDL_Event event;
    bool quit = false;
    GameState currentState = MENU;
    bool soundOn = true;
    BirdColor currentBirdColor = BLUE;
    int highestScore = loadHighScore("highscore.txt");

    /// Setup tài nguyên
    ScrollingBackground background;
    background.setTexture(graphics.loadTexture(BACKGROUND_IMG));

    Mix_Music* gMusic = graphics.loadMusic(BACKGROUND_MUSIC);
    if (soundOn) graphics.play(gMusic);
    Mix_Chunk* gChunk_wing = graphics.loadSound(CHUNK_WING);
    Mix_Chunk* gChunk_point = graphics.loadSound(CHUNK_POINT);

    TTF_Font* font = graphics.loadFont(FONT_NAME, 90);
    TTF_Font* fontScoreInGameOver = graphics.loadFont(FONT_NAME, 125);
    TTF_Font* fontScoreInPlaying = graphics.loadFont(FONT_NAME, 60);

    Mouse mouse(START_POS, SCREEN_HEIGHT / 2);
    Sprite bird;

    SDL_Texture* pipeTexture = graphics.loadTexture(PIPES_IMG);
    vector<pipes> pipeList;
    initPipes(pipeList, pipeTexture);

    // Bird texture load sau khi chọn Play
    SDL_Texture* birdTexture = nullptr;

    while (!quit) {
        graphics.prepareScene();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                quit = true;
            ///MENU
            if (currentState == MENU && event.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                Button playBtn = createButton(PLAY_BUT, font, graphics, PLAY_OFFSET);
                string soundText = soundOn ? SOUND_ON_BUT : SOUND_OFF_BUT;
                Button soundBtn = createButton(soundText, font, graphics, SOUND_OFFSET);

                string colorText;
                if (currentBirdColor == BLUE) colorText = "Bird Color: Blue";
                else if (currentBirdColor == RED) colorText = "Bird Color: Red";
                else colorText = "Bird Color: Yellow";
                Button colorBtn = createButton(colorText, font, graphics, COLOR_OFFSET);

                if (isButtonClicked(mouseX, mouseY, playBtn))
                {
                    currentState = PLAYING;
                    mouse.setPos(START_POS, SCREEN_HEIGHT / 2);
                    pipeList.clear();
                    initPipes(pipeList, pipeTexture);

                    // Load bird texture theo color
                    if (birdTexture != nullptr) SDL_DestroyTexture(birdTexture);
                    if (currentBirdColor == BLUE) birdTexture = graphics.loadTexture("img//bird_blue.png");
                    else if (currentBirdColor == RED) birdTexture = graphics.loadTexture("img//bird_red.png");
                    else birdTexture = graphics.loadTexture("img//bird_yellow.png");

                    bird.init(birdTexture, BIRD_FRAMES, BIRD_CLIPS);
                }
                else if (isButtonClicked(mouseX, mouseY, soundBtn))
                {
                    soundOn = !soundOn;
                    if (soundOn) graphics.play(gMusic);
                    else Mix_HaltMusic();//dừng phát nhạc ngay
                }
                else if (isButtonClicked(mouseX, mouseY, colorBtn))
                {
                    if (currentBirdColor == BLUE) currentBirdColor = RED;
                    else if (currentBirdColor == RED) currentBirdColor = YELLOW;
                    else currentBirdColor = BLUE;
                }

                destroyButton(playBtn);
                destroyButton(soundBtn);
                destroyButton(colorBtn);
            }

            ///state GAME_OVER
            else if (currentState == GAME_OVER && event.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                Button playAgainBtn = createButton(REPLAY_BUT, font, graphics, REPLAY_OFFSET);
                Button backBtn = createButton(BACK_MENU_BUT, font, graphics, BACK_OFFSET);
                //truong hop choi lai
                if (isButtonClicked(mouseX, mouseY, playAgainBtn))
                {
                    currentState = PLAYING;
                    mouse.setPos(START_POS, SCREEN_HEIGHT / 2);
                    pipeList.clear();
                    initPipes(pipeList, pipeTexture);
                }
                else if (isButtonClicked(mouseX, mouseY, backBtn))
                {
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

        if (currentState == MENU) {
            renderMenu(graphics, font, soundOn, currentBirdColor);
        }
        else if (currentState == PLAYING) {
            if (!gameOver(mouse, pipeList)) {
                if (mouse.isLeftButtonPressed() && soundOn) {
                    graphics.play(gChunk_wing, 0);
                }

                mouse.move();
                bird.tick();
                updatePipes(pipeList);

                background.scroll(SCOLL_BG);
                graphics.render(mouse, bird);
                graphics.renderPipes(pipeList, pipeTexture);

                int presScore = mouse.score;
                mouse.updateScore(pipeList);
                if (presScore != mouse.score && soundOn) graphics.play(gChunk_point, 1);

                graphics.renderScore(fontScoreInPlaying, "Score: ", mouse.score, 20, 20, {255, 100, 100, 255});
            }
            else {
                currentState = GAME_OVER;
            }
        }
        else if (currentState == GAME_OVER)
        {
            if (mouse.score > highestScore) {highestScore = mouse.score;saveHighScore("highscore.txt", highestScore);}
            renderGameOver(graphics, font);
            graphics.renderScore(fontScoreInPlaying, "Highest Score: ", highestScore, 125, 200, {255, 105, 0, 255});
            graphics.renderScore(fontScoreInGameOver, "Score: ", mouse.score, 150, 100, {255, 255, 100, 255});
        }

        graphics.presentScene();
        SDL_Delay(10);
    }

    // Clean
    if (birdTexture != nullptr) SDL_DestroyTexture(birdTexture);
    SDL_DestroyTexture(pipeTexture);
    SDL_DestroyTexture(background.texture);

    Mix_FreeChunk(gChunk_wing);
    Mix_FreeChunk(gChunk_point);

    TTF_CloseFont(font);
    TTF_CloseFont(fontScoreInPlaying);
    TTF_CloseFont(fontScoreInGameOver);

    if (gMusic != nullptr) Mix_FreeMusic(gMusic);

    graphics.quit();
    return 0;
}
