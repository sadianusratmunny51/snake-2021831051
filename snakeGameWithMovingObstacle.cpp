#include<bits/stdc++.h>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BLOCK_SIZE = 10;
const  int rectSpeed = 5;

struct border{
  int x;
  int y;
};

struct snakesegment {
    int x, y;
};
bool checkCollision(const SDL_Rect& rect, const snakesegment& head) {
    return (head.x *BLOCK_SIZE  < rect.x + rect.w &&
            (head.x + 1) *BLOCK_SIZE  > rect.x &&
            head.y * BLOCK_SIZE < rect.y + rect.h &&
            (head.y + 1) * BLOCK_SIZE > rect.y);
}

void update(vector<snakesegment>& snake, SDL_Point& food,auto direction,bool &running,SDL_Rect &rect,int &score)
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

    //cout<<"Headx="<<headX<<" "<<"Heady="<<headY<<endl;
     if (headX == food.x && headY == food.y)
      { 
        snakesegment newSegment = {headX, headY};
        snake.insert(snake.begin(), newSegment);
        food.x = rand() % (SCREEN_WIDTH -2* BLOCK_SIZE)/BLOCK_SIZE+1; 
        food.y = rand() % (SCREEN_HEIGHT -2* BLOCK_SIZE)/BLOCK_SIZE+1;
        score=score+5;
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
      }
     if (checkCollision(rect, { headX, headY })) {
        cout << "Collision happened with the moving rectangle." << endl;
        cout<<"rect.x="<<rect.x<<"rect.y="<<rect.y<<endl;
        cout<<"Headx="<<headX<<" "<<"Heady="<<headY<<endl;

        if(score>=5)
        score-=5;
     }

      snakesegment newHead = {headX, headY};
      snake.insert(snake.begin(), newHead);
      snake.pop_back();
      

}
 void render(auto renderer, auto snake, auto food, SDL_Rect &rect, int &score, TTF_Font* font,SDL_Rect &bonusRect,auto borders)
 {
    for (int i = 0; i < snake.size(); ++i)
    {
    const auto& segment = snake[i];
    SDL_Rect blockRect = {segment.x * BLOCK_SIZE, segment.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
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
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);

    //border
    for (int i = 0; i < borders.size(); ++i)
    {
    const auto& segment = borders[i];
    SDL_Rect blockRect = {segment.x * BLOCK_SIZE, segment.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); 
    SDL_RenderFillRect(renderer, &blockRect);
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
   
            
    SDL_Color textColor = {0, 255, 255, 255};
    string scoreText = "Game Over. Score: " + to_string(score);

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
    auto window=SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,0);
    auto renderer=SDL_CreateRenderer(window, -1, 0);
    TTF_Font* font = TTF_OpenFont("munni.ttf", 25); 

    SDL_Rect rect = {0,SCREEN_HEIGHT/2 ,100 ,10 };
    SDL_Rect bonusRect={rand()%640,rand()%480,BLOCK_SIZE ,BLOCK_SIZE };

    vector<border>borders;
    for(int i=0;i<=63;i++)
    {
        borders.push_back({i,0});
        borders.push_back({i,47});
    }
    
     for(int i=1;i<=47;i++)
    {
        borders.push_back({0,i});
        borders.push_back({63,i});
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
        update(snake,food, direction,running,rect,score);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        SDL_RenderClear(renderer);

        render(renderer,snake,food,rect,score ,font,bonusRect,borders);

        SDL_RenderPresent(renderer);

        SDL_Delay(120);

    }
    Gameover(renderer,score,font);
    SDL_Delay(1000);

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
