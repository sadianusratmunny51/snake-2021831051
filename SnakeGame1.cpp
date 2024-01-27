#include<bits/stdc++.h>
#include <vector>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include<SDL2/SDL_mixer.h>
Mix_Chunk *eat;
Mix_Chunk *bonus;
Mix_Chunk *movingRect;
Mix_Chunk *obstacles;
Mix_Chunk *gameEnd;
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BLOCK_SIZE = 10;
const  int rectSpeed = 5;
const int bonush_timeLimit=5000;
int bonush_start=0;
int flag_1=0;
int flag_2=0;
int checkLevel=1;

struct border
{
    int x;
    int y;
};

struct obstacle
{
    int x;
    int y;
};

struct snakesegment
{
    int x, y;
};
void hitobstaclesmessege(auto renderer,auto font)
{
    cout<<"hitted obstacle"<<endl;

    SDL_Color textColor = {255, 255, 255, 255};
    string scoreText = "   Press Up arrow for continue the Game else it'll over" ;

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);

    SDL_Rect scoreRect = {10, 200, textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, nullptr, &scoreRect);
    SDL_RenderPresent(renderer);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    SDL_Delay(5000);

}
void showtext(auto renderer,auto font)
{
    SDL_Color textColor = {255, 0, 0, 255};
    string scoreText = "   Hitted  " ;

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);

    SDL_Rect scoreRect = {200, 200, textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, nullptr, &scoreRect);
    SDL_RenderPresent(renderer);
    SDL_Delay(150);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}
bool checkCollision(const SDL_Rect& rect, const snakesegment& head)
{
    return (head.x *BLOCK_SIZE  < rect.x + rect.w &&
            (head.x + 1) *BLOCK_SIZE  > rect.x &&
            head.y * BLOCK_SIZE < rect.y + rect.h &&
            (head.y + 1) * BLOCK_SIZE > rect.y);
}

void update(auto renderer,auto font,vector<snakesegment>& snake, SDL_Point& food,auto direction,bool &running,SDL_Rect &rect,int &score,
            SDL_Rect &bonushfood, int bonush_timeLimit,int &bonush_timeStart,vector<obstacle>&obstacle1,vector<obstacle>&obstacle2,vector<border>&borders)
{
    int headX = snake.front().x;
    int headY = snake.front().y;
    switch (direction)
    {
    case SDLK_UP:
        headY--;
        break;
    case SDLK_DOWN:
        headY++;
        break;
    case SDLK_LEFT:
        headX--;
        break;
    case SDLK_RIGHT:
        headX++;
        break;
    default:
        break;
    }
    headX = (headX + SCREEN_WIDTH / BLOCK_SIZE) % (SCREEN_WIDTH / BLOCK_SIZE);
    headY = (headY + SCREEN_HEIGHT / BLOCK_SIZE) % (SCREEN_HEIGHT / BLOCK_SIZE);
    if(headX==0)
    {
        headX=62;
    }
    if(headX==63)
    {
        headX=0;
    }
    if(headY==0)
    {
        headY=46;
    }
    if(headY==47)
    {
        headY=0;
    }

    //cout<<"Headx="<<headX<<" "<<"Heady="<<headY<<endl;
    if (headX == food.x && headY == food.y)
    {
        snakesegment newSegment = {headX, headY};
        snake.insert(snake.begin(), newSegment);
        food.x = rand() % (SCREEN_WIDTH -2* BLOCK_SIZE)/BLOCK_SIZE+1;
        food.y = rand() % (SCREEN_HEIGHT -2* BLOCK_SIZE)/BLOCK_SIZE+1;
        Mix_PlayChannel(-1, eat, 0);

        score=score+5;
        //obstacle1
        for (const auto& obs : obstacle1)
        {
            if(obs.x == food.x && obs.y == food.y)
            {
                food.x = rand() % (SCREEN_WIDTH -2* BLOCK_SIZE)/BLOCK_SIZE+1;
                food.y = rand() % (SCREEN_HEIGHT -2* BLOCK_SIZE)/BLOCK_SIZE+1;
            }
        }
        //obstacle2
        for (const auto& obs : obstacle2)
        {
            if(obs.x == food.x && obs.y == food.y)
            {
                food.x = rand() % (SCREEN_WIDTH -2* BLOCK_SIZE)/BLOCK_SIZE+1;
                food.y = rand() % (SCREEN_HEIGHT -2* BLOCK_SIZE)/BLOCK_SIZE+1;
            }
        }

        long long current_time=SDL_GetTicks();
        if(score%25==0 && score!=0 && current_time-bonush_timeStart>=bonush_timeLimit)
        {
            bonushfood.x = rand() % (SCREEN_WIDTH -2* BLOCK_SIZE)/BLOCK_SIZE+1;
            bonushfood.y = rand() % (SCREEN_HEIGHT -2* BLOCK_SIZE)/BLOCK_SIZE+1;
            Mix_PlayChannel(-1, bonus, 0);

            bonush_start=1;
            bonush_timeStart=current_time;

        }


    }

    else
    {
        for (int i = 1; i < snake.size(); ++i)
        {
            if(snake[i].x == headX && snake[i].y == headY)
            {
                running= false;
            }
        }


        if (headX == bonushfood.x && headY == bonushfood.y)
        {
            Mix_PlayChannel(-1, eat, 0);
            snakesegment newSegment = { headX, headY };
            snake.insert(snake.begin(), newSegment);
            score += 10;  // Increase the score for bonus food
            bonushfood.x = -1;
            bonush_start=0;
        }
        if (checkCollision(rect, { headX, headY }))
        {
            cout << "Collision happened with the moving rectangle." << endl;
           // cout<<"rect.x="<<rect.x<<"rect.y="<<rect.y<<endl;
           // cout<<"Headx="<<headX<<" "<<"Headx+1="<<headX+1<<endl;
            //cout<<"Heady="<<headY<<" "<<"Heady+1="<<headY+1<<endl;
            Mix_PlayChannel(-1, movingRect, 0);

            if(score>=5)
                score-=5;
            showtext(renderer,font);

        }
        //collision with obstacle
        if(flag_1==1)
        {
            for (int i = 1; i < obstacle1.size(); ++i)
            {
                if(obstacle1[i].x == headX && obstacle1[i].y == headY)
                {
                    running=false;
                    Mix_PlayChannel(-1, obstacles, 0);
                    hitobstaclesmessege(renderer, font);


                    SDL_Event e;
                    if(running==false)
                    {
                        while(SDL_PollEvent(&e))
                        {
                            if(e.type==SDL_KEYDOWN)
                            {
                                switch(e.key.keysym.sym)
                                {
                                case SDLK_UP:
                                    running=true;
                                    if(score>=10)
                                        score-=10;
                                    break;
                                case SDLK_DOWN:
                                    running=false;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        //obstacle2 hitting
        if(flag_2==1)
        {
            for (int i = 1; i < obstacle2.size(); ++i)
            {
                if(obstacle2[i].x == headX && obstacle2[i].y == headY)
                {
                    running=false;
                    Mix_PlayChannel(-1, obstacles, 0);
                    hitobstaclesmessege(renderer, font);

                    SDL_Event e;
                    if(running==false)
                    {
                        while(SDL_PollEvent(&e))
                        {
                            if(e.type==SDL_KEYDOWN)
                            {
                                switch(e.key.keysym.sym)
                                {
                                case SDLK_UP:
                                    running=true;
                                    if(score>=20)
                                        score-=20;
                                    break;
                                case SDLK_DOWN:
                                    running=false;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        snakesegment newHead = {headX, headY};
        snake.insert(snake.begin(), newHead);
        snake.pop_back();

    }

}
void render(auto renderer, auto snake, auto food, SDL_Rect &rect, int &score, TTF_Font* font,auto borders,SDL_Rect &bonushfood,int &bonush_timeStart,int bonush_timeLimit,auto obstacle1,auto obstacle2)
{
    for (int i = 0; i < snake.size(); ++i)
    {
        const auto& segment = snake[i];
        SDL_Rect blockRect = {segment.x * BLOCK_SIZE, segment.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
        if(i==0)
        {
            SDL_SetRenderDrawColor(renderer,168, 100,180, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer,255, 165,0, 255);
        }

        SDL_RenderFillRect(renderer, &blockRect);
    }
    SDL_Rect foodRect = {food.x * BLOCK_SIZE, food.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &foodRect);

    rect.x += rectSpeed;
    cout<<rect.x+rect.w<<endl;
    if (rect.x > 640)
    {
        rect.x = -rect.w;
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);

    //border
    for (int i = 0; i < borders.size(); ++i)
    {
        const auto& segment = borders[i];
        SDL_Rect blockRect = {segment.x * BLOCK_SIZE, segment.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &blockRect);
    }
    if (bonush_start==1&& (SDL_GetTicks()-bonush_timeStart)<bonush_timeLimit)
    {
        SDL_Rect bonusFood = { bonushfood.x * BLOCK_SIZE, bonushfood.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE };
        SDL_SetRenderDrawColor(renderer,255, 20, 147, 255);
        SDL_RenderFillRect(renderer, &bonusFood);
    }
    //collision with obstacle
    if(score>=10||flag_1==1)
    {
        flag_1=1;
        for (int i = 0; i < obstacle1.size(); ++i)
        {
            const auto& segment = obstacle1[i];
            SDL_Rect obstacle1Rect = {segment.x * BLOCK_SIZE, segment.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &obstacle1Rect);
        }
    }

    if(score>=20||flag_2==1)
    {
        flag_2=1;
        for (int i = 0; i < obstacle2.size(); ++i)
        {
            const auto& segment = obstacle2[i];
            SDL_Rect obstacle2Rect = {segment.x * BLOCK_SIZE, segment.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &obstacle2Rect);
        }
    }


    SDL_Color textColor = {255, 255, 255, 255};
    string scoreText = "Score: " + to_string(score);

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);

    SDL_Rect scoreRect = {10, 10, textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, nullptr, &scoreRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);


}
void Gameover(auto renderer, int score,auto font)
{

    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderClear(renderer);
    SDL_Color textColor = {0, 0, 0, 255};
    Mix_PlayChannel(-1, gameEnd, 0);
    string scoreText = "Game  is Over with Score: " + to_string(score);


    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);


    SDL_Rect scoreRect = {200, 200, textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, nullptr, &scoreRect);
    SDL_RenderPresent(renderer);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 1024);
    bonus = Mix_LoadWAV("bonus.mp3");
    eat = Mix_LoadWAV("eat.mp3");
    movingRect = Mix_LoadWAV("movingRect.mp3");
    obstacles = Mix_LoadWAV("obstacles.mp3");
    gameEnd = Mix_LoadWAV("gameEnd.mp3");


    auto window=SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,0);
    auto renderer=SDL_CreateRenderer(window, -1, 0);
    TTF_Font* font = TTF_OpenFont("munni.ttf", 25);
    SDL_Surface* backgroundSurface = IMG_Load("snake2.jpeg");

    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    SDL_Rect rect = {0,SCREEN_HEIGHT/2,100,10 };
    SDL_Rect bonushfood= {-1,-1,BLOCK_SIZE,BLOCK_SIZE};
    int bonush_timeStart=0;
    vector<border>borders;
    for(int i=0; i<=63; i++)
    {
        borders.push_back({i,0});
        borders.push_back({i,47});
    }

    for(int i=1; i<=47; i++)
    {
        borders.push_back({0,i});
        borders.push_back({63,i});
    }
    //obstacle
    vector<obstacle>obstacle1;

    for(int i=44; i<=53; i++)
    {
        obstacle1.push_back({i,7});
    }
    for(int i=8; i<18; i++)
    {
        obstacle1.push_back({53,i});
    }
    vector<obstacle>obstacle2;

    for(int i=10; i<=20; i++)
    {
        obstacle2.push_back({i,38});
    }
    for(int i=38; i>28; i--)
    {
        obstacle2.push_back({10,i});
    }


    vector <snakesegment> snake{{5,5}};
    SDL_Point food = {10, 10};
    SDL_Keycode direction = SDLK_RIGHT;



    bool running = true;
    int score = 0;
    SDL_Event e;

    while(running)
    {
        while(SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running=false;
            }
            else if(e.type==SDL_KEYDOWN)
            {
                switch(e.key.keysym.sym)
                {
                case SDLK_UP:
                    if (direction != SDLK_DOWN)
                        direction = SDLK_UP;
                    break;
                case SDLK_DOWN:
                    if (direction != SDLK_UP)
                        direction = SDLK_DOWN;
                    break;
                case SDLK_LEFT:
                    if (direction != SDLK_RIGHT)
                        direction = SDLK_LEFT;
                    break;
                case SDLK_RIGHT:
                    if (direction != SDLK_LEFT)
                        direction = SDLK_RIGHT;
                    break;
                default:
                    break;
                }
            }

        }
        update(renderer,font,snake,food, direction,running,rect,score,bonushfood,bonush_timeLimit,bonush_timeStart,obstacle1,obstacle2,borders);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        render(renderer,snake,food,rect,score,font,borders,bonushfood,bonush_timeStart,bonush_timeLimit,obstacle1,obstacle2);

        SDL_RenderPresent(renderer);
        if(score>=50)
        {
            checkLevel=2;
        }
        if(checkLevel==1)
        {
            SDL_Delay(120);
        }
        else
        {
            SDL_Delay(60);
        }

    }
    Gameover(renderer,score,font);
    SDL_Delay(3000);

    TTF_CloseFont(font);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
