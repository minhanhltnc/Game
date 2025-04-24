#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include "defs.h"
#include "backgrond.h"
#include "sprites.h"
#include "game.h"
#include "pipes.h"
#include "game.cpp"
struct Graphics {
    SDL_Renderer *renderer;
	SDL_Window *window;

	void logErrorAndExit(const char* msg, const char* error)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
        SDL_Quit();
    }
///init
	void init() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
            logErrorAndExit("SDL_Init", SDL_GetError());

        window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

        if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
            logErrorAndExit( "SDL_image error:", IMG_GetError());

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);

        if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
        //Initialize SDL_mixer
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
        logErrorAndExit( "SDL_mixer could not initialize! SDL_mixer Error: %s\n",
                    Mix_GetError() );}
        //font
        if (TTF_Init() == -1) {
            logErrorAndExit("SDL_ttf could not initialize! SDL_ttf Error: ",
                             TTF_GetError());
        }


    }

    void prepareScene()
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);//màu đen
        SDL_RenderClear(renderer);//vẽ hàm hình đen
    }

	void prepareScene(SDL_Texture * background)
    {
        SDL_RenderClear(renderer);//xóa màn hình trước khi vẽ
        SDL_RenderCopy( renderer, background, NULL, NULL);//sao chép texture background lên bộ vẽ
    }

    void presentScene()
    {
        SDL_RenderPresent(renderer);//cập nhật màn hình(bao gồm tất cả những gì đc vẽ)
    }
///load texture
    SDL_Texture *loadTexture(const char *filename) const
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);//Ghi log thông tin rằng chương trình đang tải tệp hình ảnh

        SDL_Texture *texture = IMG_LoadTexture(renderer, filename);//tải ảnh
        if (texture == NULL)
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());//báo lỗi

        return texture;
    }


///render text ture
    void renderTexture( SDL_Texture *texture,const int x,const int y) const
    {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        SDL_QueryTexture( texture, NULL, NULL, &dest.w, &dest.h);

        SDL_RenderCopy( renderer, texture, NULL, &dest);
    }


///render pipes
   void renderPipe(const pipes& pipe,  SDL_Texture* pipeTexture) const{
    SDL_RenderCopy(renderer, pipeTexture, nullptr, &pipe.bottomRect);
    SDL_RenderCopyEx(renderer, pipeTexture, nullptr, &pipe.topRect, 0, nullptr, SDL_FLIP_VERTICAL);
    }
    void renderPipes(const vector<pipes>& pipeList, SDL_Texture* pipeTexture)const
    {
        for (int i = 0; i < NUM_PIPES; ++i)
        {
            renderPipe(pipeList[i], pipeTexture); // Gọi phương thức renderPipe đã có
        }
    }



///render background
     void render(const ScrollingBackground& background)const
     {

        renderTexture(background.texture, background.scrollingOffset, 0);//rendertexture được viết ở trên
        if (background.scrollingOffset > 0) {
        renderTexture(background.texture, background.scrollingOffset - background.width, 0);}
    }


///render spites
     void render(int x, int y, const Sprite& sprite)const {
        const SDL_Rect* clip = sprite.getCurrentClip();
        SDL_Rect renderQuad = {x, y, clip->w, clip->h};
        SDL_RenderCopy(renderer, sprite.texture, clip, &renderQuad);
    }
///render sprite gan voi con tro chuot
    void render(const Mouse& mouse,const Sprite& sprite) const {
    const SDL_Rect* clip = sprite.getCurrentClip(); // Lấy khung hình hiện tại
    SDL_Rect renderQuad = {mouse.x, mouse.y, clip->w, clip->h}; // Vị trí hiển thị tại con trỏ chuột
    SDL_RenderCopy(renderer, sprite.texture, clip, &renderQuad); // Hiển thị
}
///music and sounds

    Mix_Music *loadMusic(const char* path)const
    {
        Mix_Music *gMusic = Mix_LoadMUS(path);
        if (gMusic == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
                           "Could not load music! SDL_mixer Error: %s", Mix_GetError());
        }
        return gMusic;
    }
    void play(Mix_Music *gMusic)
    {
        if (gMusic == nullptr) return;

        if (Mix_PlayingMusic() == 0) {
            Mix_PlayMusic( gMusic, -1 );
        }
        else if( Mix_PausedMusic() == 1 ) {
            Mix_ResumeMusic();
        }
    }

    Mix_Chunk* loadSound(const char* path) const {
        Mix_Chunk* gChunk = Mix_LoadWAV(path);
        if (gChunk == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
                       "Could not load sound! SDL_mixer Error: %s", Mix_GetError());
        }
        return gChunk;
    }
    void play(Mix_Chunk* gChunk, int channel) {
        if (gChunk != nullptr) {
            Mix_PlayChannel( channel, gChunk, 0 );
        }
    }
///FONT
    TTF_Font* loadFont(const char* path, int size) const
    {
        TTF_Font* gFont = TTF_OpenFont( path, size );
        if (gFont == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                           "Load font %s", TTF_GetError());
        }
        return gFont;
    }
     SDL_Texture* renderText(const char* text,TTF_Font* font, SDL_Color textColor)const
    {
        SDL_Surface* textSurface =
                TTF_RenderText_Solid( font, text, textColor );
        if( textSurface == nullptr ) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                           "Render text surface %s", TTF_GetError());
            return nullptr;
        }

        SDL_Texture* texture =
                SDL_CreateTextureFromSurface( renderer, textSurface );
        if( texture == nullptr ) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                           "Create texture from text %s", SDL_GetError());
        }
        SDL_FreeSurface( textSurface );
        return texture;
    }
    void renderScore(TTF_Font* font,const string s, const int score,const int x,const int y, SDL_Color color)const {
    string scoreText = s+std::to_string(score);
    SDL_Texture* scoreTex = renderText(scoreText.c_str(), font, color);

    if (scoreTex != nullptr) {
        int w, h;
        SDL_QueryTexture(scoreTex, NULL, NULL, &w, &h);
        renderTexture(scoreTex, x, y);
        SDL_DestroyTexture(scoreTex);
    }
}





    void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y)
    {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        dest.w = src->w;
        dest.h = src->h;

        SDL_RenderCopy(renderer, texture, src, &dest);
    }

    void quit()
    {
        IMG_Quit();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        Mix_Quit();
        TTF_Quit();


    }
};

#endif // _GRAPHICS__H
