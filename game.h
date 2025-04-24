#ifndef GAME_H
#define GAME_H
#include <vector>
#include "defs.h"
#include "sprites.h"
#include "pipes.h"
#define CHEESE_SIZE 10
#define MOUSE_SIZE 20

using namespace std;
bool inside(int x, int y, SDL_Rect r) {
    return x > r.x && x < r.x+r.w && y > r.y && y < r.y+r.h;
}

bool overlap(const SDL_Rect& r1, const SDL_Rect& r2) {
    return inside(r1.x, r1.y, r2) || inside(r1.x + r1.w, r1.y, r2) ||
            inside(r1.x, r1.y+r1.h, r2) || inside(r1.x+r1.w, r1.y+r1.h, r2);
}
struct Mouse {
    SDL_Rect rect;
    int x = START_POS, y = SCREEN_HEIGHT / 2;
    int score ; vector<bool> passedPipe;
    int UP_SPEED,FALL_SPEED;
    int lastScoredPipeIndex = -1; // Lưu chỉ số pipe đã được cộng điểm gần nhất

    Mouse(int x, int y) {
        rect.x = x;
        rect.y = y;
        rect.h = MOUSE_SIZE;
        rect.w = MOUSE_SIZE;
        score=0;
        UP_SPEED=-8,FALL_SPEED=4;
    }
    void setPos(int x, int y)
    {
        rect.x = x;
        rect.y = y;
        rect.h = MOUSE_SIZE;
        rect.w = MOUSE_SIZE;
        score=0;
        lastScoredPipeIndex = -1;
    }

    bool isLeftButtonPressed()
    {
        Uint32 buttonState = SDL_GetMouseState(NULL, NULL);
        return (buttonState & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
    }
    void move() {
            if(isLeftButtonPressed()){y+=UP_SPEED;
            }
            y+=FALL_SPEED;
            rect.x = x;
            rect.y = y;
    }
    bool collide(const vector<pipes>& pipeList) const
    {
        for (int i = 0; i < NUM_PIPES; ++i)
        {if (overlap(rect, pipeList[i].bottomRect) || overlap(rect, pipeList[i].topRect))return true;}
        return false;
    }





    void updateScore(const vector<pipes>& pipeList) {

    for (int i = 0; i <(int) pipeList.size(); ++i)
    {
        int pipeRight = pipeList[i].topRect.x + pipeList[i].topRect.w;
        if (pipeRight < x&& lastScoredPipeIndex != i ) {
            score++;
            lastScoredPipeIndex = i;
            break;// Cập nhật thời gian điể

    }
    }
}



};
void initPipes( vector<pipes>& pipeList, SDL_Texture* pipeTexture)
{
    for (int i = 0; i < NUM_PIPES; ++i) {
        pipeList.emplace_back(START_X + i * PIPE_SPACING,pipeTexture);
    }
}
inline void updatePipes(vector<pipes>& pipeList)
{
    for (int i = 0; i < NUM_PIPES; ++i)
    {
        pipeList[i].update();
    }
}




bool gameOver(const Mouse& mouse,vector<pipes>& pipeList)
{
    return  mouse.x < 0 || mouse.x >= SCREEN_WIDTH ||
           mouse.y < 0 || mouse.y >= SCREEN_HEIGHT ||
           mouse.collide(pipeList); // Kiểm tra va chạm với các ống
}

#endif // GAME_H
