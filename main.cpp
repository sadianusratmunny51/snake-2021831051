#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BLOCK_SIZE = 10;

struct SnakeSegment {
    int x, y;
};

void render(SDL_Renderer* renderer, const std::vector<SnakeSegment>& snake, const SDL_Point& food, int score, TTF_Font* font);
void update(std::vector<SnakeSegment>& snake, SDL_Point& food, SDL_Keycode direction, bool& running, int& score);
bool checkCollision(const std::vector<SnakeSegment>& snake, int x, int y);

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (TTF_Init() != 0) {
        std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Snake Game By Sadia Nusrat Munny", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("munni.ttf", 25); 

    std::vector<SnakeSegment> snake{{5, 5}}; 
    SDL_Point food = {10, 10};
    SDL_Keycode direction = SDLK_RIGHT;
    bool running = true;
    int score = 0;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { 
                running = false;
            } else if (event.type == SDL_KEYDOWN) {   
                switch (event.key.keysym.sym) {
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

        update(snake, food, direction, running, score);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        SDL_RenderClear(renderer);

        render(renderer, snake, food, score, font); 

        SDL_RenderPresent(renderer);

        SDL_Delay(150);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

void render(SDL_Renderer* renderer, const std::vector<SnakeSegment>& snake, const SDL_Point& food, int score, TTF_Font* font) {
    for (const auto& segment : snake) { 
        SDL_Rect blockRect = {segment.x * BLOCK_SIZE, segment.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
        SDL_SetRenderDrawColor(renderer, 255, 128, 128, 255); 
        SDL_RenderFillRect(renderer, &blockRect);
    }
    SDL_Rect foodRect = {food.x * BLOCK_SIZE, food.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
    SDL_SetRenderDrawColor(renderer, 204, 255, 255, 255);
    SDL_RenderFillRect(renderer, &foodRect);

    SDL_Color textColor = {255, 255, 255, 255}; 
    std::string scoreText = "Score: " + std::to_string(score);

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);

    SDL_Rect scoreRect = {10, 10, textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, nullptr, &scoreRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}
void update(std::vector<SnakeSegment>& snake, SDL_Point& food, SDL_Keycode direction, bool& running, int& score) {
    int headX = snake.front().x;
    int headY = snake.front().y;
    switch (direction) {
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

    if (headX == food.x && headY == food.y) { 
        SnakeSegment newSegment = {headX, headY};
        snake.insert(snake.begin(), newSegment);
        food.x = rand() % (SCREEN_WIDTH / BLOCK_SIZE); 
        food.y = rand() % (SCREEN_HEIGHT / BLOCK_SIZE);
        score += 10;
    } else {
        if (checkCollision(snake, headX, headY)) {
            running = false;
            return;
        }
        SnakeSegment newHead = {headX, headY};
        snake.insert(snake.begin(), newHead);
        snake.pop_back();
    }
}

bool checkCollision(const std::vector<SnakeSegment>& snake, int x, int y) {
    for (int i = 1; i < snake.size(); ++i) { 
        if (snake[i].x == x && snake[i].y == y) {
            return true;
        }
    }
    return false; 
}