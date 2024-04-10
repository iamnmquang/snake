#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <deque>
#include <ctime>

void StartGame(SDL_Renderer* renderer, TTF_Font* font);
void ExitGame();

void ShowMenu(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Event event;
    bool isRunning = true;

    while (isRunning) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Vẽ menu
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* playSurface = TTF_RenderText_Solid(font, "Play Game", textColor);
        SDL_Texture* playTexture = SDL_CreateTextureFromSurface(renderer, playSurface);
        SDL_Rect playRect = {300, 200, playSurface->w, playSurface->h};
        SDL_RenderCopy(renderer, playTexture, NULL, &playRect);
        SDL_FreeSurface(playSurface);
        SDL_DestroyTexture(playTexture);

        SDL_Surface* exitSurface = TTF_RenderText_Solid(font, "Exit", textColor);
        SDL_Texture* exitTexture = SDL_CreateTextureFromSurface(renderer, exitSurface);
        SDL_Rect exitRect = {300, 300, exitSurface->w, exitSurface->h};
        SDL_RenderCopy(renderer, exitTexture, NULL, &exitRect);
        SDL_FreeSurface(exitSurface);
        SDL_DestroyTexture(exitTexture);

        SDL_RenderPresent(renderer);

        // Xử lý sự kiện
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                ExitGame();
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (mouseX >= playRect.x && mouseX <= playRect.x + playRect.w &&
                    mouseY >= playRect.y && mouseY <= playRect.y + playRect.h) {
                    // Nếu người dùng chọn Play Game5
                    isRunning = false;
                }
                else if (mouseX >= exitRect.x && mouseX <= exitRect.x + exitRect.w &&
                         mouseY >= exitRect.y && mouseY <= exitRect.y + exitRect.h) {
                    // Nếu người dùng chọn Exit
                    ExitGame();
                }
            }
        }
    }
}

void StartGame(SDL_Renderer* renderer, TTF_Font* font) {
    // Thực hiện logic để bắt đầu trò chơi
}

void ExitGame() {
    SDL_Quit();
    exit(0);
}

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    if (TTF_Init() < 0) {
        std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("SnakeScreen", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Event e;

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    Mix_Music* backgroundMusic = Mix_LoadMUS("Game.mp3");
    if (!backgroundMusic) {
        std::cout << "Failed to load background music: " << Mix_GetError() << std::endl;
        return 1;
    }

    Mix_Chunk* eatSound = Mix_LoadWAV("eat.mp3");
    if (!eatSound) {
        std::cout << "Failed to load eat sound: " << Mix_GetError() << std::endl;
        return 1;
    }

    Mix_Chunk* loseGame = Mix_LoadWAV("Ohno.mp3");
    if(!loseGame)
    {
        std::cout << "Failed to load lose game sound: " << Mix_GetError() << std::endl;
    }

    enum Direction
    {
        DOWN, LEFT, RIGHT, UP
    };

    StartGame:
    bool running = true;
	bool gameover = false;
    int dir = RIGHT;
    int score = 0;
    SDL_Rect head {500,500,10,10};
    std::deque<std::pair<int, int>> snake;
    snake.push_front(std::make_pair(head.x, head.y));

    // Apple position
    std::pair<int, int> apple = std::make_pair(rand() % 80 * 10, rand() % 64 * 10);

    SDL_Surface* gameOverSurface = IMG_Load("gameover.jpg");
    if (!gameOverSurface) {
        std::cout << "Failed to load game over image: " << IMG_GetError() << std::endl;
        return 1;
    }
    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
    if (!gameOverTexture) {
        std::cout << "Failed to create texture from game over surface: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Load font
    TTF_Font *font = TTF_OpenFont("Point.ttf",20);
    if(!font)
    {
        std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
        return 1;
    }
    TTF_Font* fong = TTF_OpenFont("Point.ttf", 50);
    if(!fong)
    {
        std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
        return 1;
    }
    ShowMenu(renderer, fong);

    // Play background music
    Mix_PlayMusic(backgroundMusic, -1);

    while(running && !gameover)
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                running = false;
            }
            if(e.type == SDL_KEYDOWN)
            {
                if(e.key.keysym.sym == SDLK_DOWN && dir != UP) {dir = DOWN;}
                if(e.key.keysym.sym == SDLK_UP && dir != DOWN) {dir = UP;}
                if(e.key.keysym.sym == SDLK_RIGHT && dir != LEFT) {dir = RIGHT;}
                if(e.key.keysym.sym == SDLK_LEFT && dir != RIGHT) {dir = LEFT;}
            }
        }

        // Move
        switch (dir)
        {
            case DOWN:
                head.y += 10;
                break;
            case UP:
                head.y -= 10;
                break;
            case RIGHT:
                head.x += 10;
                break;
            case LEFT:
                head.x -= 10;
                break;
            default:
                break;
        }
        snake.push_front(std::make_pair(head.x, head.y));
		if(head.x < 0 )
        {
            head.x = 800;
        }
        if(head.x > 800)
        {
            head.x = 0;
        }
        if(head.y < 0)
        {
            head.y = 640;
        }
        if(head.y > 640)
        {
            head.y = 0;
        }
        // Collision detection with apple
        if (head.x == apple.first && head.y == apple.second)
        {
            // Generate new position for the apple
            score++;
            Mix_PlayChannel(-1, eatSound, 0);
            apple = std::make_pair(rand() % 80 * 10, rand() % 64 * 10);
        }
        else
        {
            // Remove the tail
            snake.pop_back();
        }
        for (auto it = snake.begin() + 1; it != snake.end(); ++it) {
            if (head.x == it->first  && head.y == it->second) {
                gameover = true;
                Mix_PlayChannel(-1, loseGame, 0);
                break;
            }
        }
        if(gameover)
        {
           // Clear renderer
            SDL_RenderClear(renderer);
            
            // Draw game over texture
            SDL_RenderCopy(renderer, gameOverTexture, NULL, NULL);

            // Update screen
            SDL_RenderPresent(renderer);

            SDL_Delay(2000);
            // ShowMenu(renderer, fong);
            goto StartGame;
        }

        // Clear Window
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw Snake
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (auto &segment : snake)
        {
            SDL_Rect rect = {segment.first, segment.second, 10, 10};
            SDL_RenderFillRect(renderer, &rect);
        }

        // Draw apple
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect appleRect = {apple.first, apple.second, 10, 10};
        SDL_RenderFillRect(renderer, &appleRect);

        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, ("Score: " + std::to_string(score)).c_str(), textColor);
        if (!scoreSurface) {
            std::cout << "Failed to render score surface: " << TTF_GetError() << std::endl;
            return 1;
        }
        SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
        if (!scoreTexture) {
            std::cout << "Failed to create score texture: " << SDL_GetError() << std::endl;
            return 1;
        }
        SDL_Rect scoreRect = {0, 0, scoreSurface->w, scoreSurface->h};
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

        // Free memory
        SDL_FreeSurface(scoreSurface);
        SDL_DestroyTexture(scoreTexture);


        SDL_RenderPresent(renderer);
        SDL_Delay(100); // Adjust the speed of the snake
    }

    Mix_FreeChunk(eatSound);
    Mix_FreeChunk(loseGame);
    Mix_FreeMusic(backgroundMusic);
    Mix_CloseAudio();
    TTF_CloseFont(font);
    SDL_FreeSurface(gameOverSurface);
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}