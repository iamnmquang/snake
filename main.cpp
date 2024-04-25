#define SDL_MAIN_HANDLED
#include "Snake.h"
#include "Menu.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 640;
const int GRID_SIZE = 10;
const int GAME_SPEED = 100;

bool paused = false;
const int PAUSE_KEY = SDLK_SPACE;

Menu menu;

int main()
{
    // Khởi tạo cũng như thực hiện các việc như tải ảnh và âm thanh
    SDL_Init(SDL_INIT_EVERYTHING);
    if (TTF_Init() < 0) {
        std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("SnakeScreen", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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

    // Bắt đầu trò chơi
    StartGame:
    bool running = true;
	bool gameover = false;
    int dir = RIGHT;
    int score = 0;
    SDL_Rect head {500,500,10,10};
    std::deque<std::pair<int, int>> snake;
    snake.push_front(std::make_pair(head.x, head.y));

    // Vị trí quả táo
    std::pair<int, int> apple = std::make_pair(rand() % (WINDOW_WIDTH/GRID_SIZE) * GRID_SIZE, rand() % (WINDOW_HEIGHT/GRID_SIZE) * GRID_SIZE);

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
    menu.ShowMenu(renderer, fong);

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
                if (e.key.keysym.sym == PAUSE_KEY) { paused = !paused; } // Kiểm tra nút tạm dừng
            }
        }

        if(paused)
        {
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* pauseSurface = TTF_RenderText_Solid(fong, "Paused", textColor);
        if (!pauseSurface) {
        std::cout << "Failed to render pause surface: " << TTF_GetError() << std::endl;
        return 1;
        }
        SDL_Texture* pauseTexture = SDL_CreateTextureFromSurface(renderer, pauseSurface);
        if (!pauseTexture) {
        std::cout << "Failed to create pause texture: " << SDL_GetError() << std::endl;
        return 1;
        }
        SDL_Rect pauseRect = {WINDOW_WIDTH / 2 - pauseSurface->w / 2, WINDOW_HEIGHT / 2 - pauseSurface->h / 2, pauseSurface->w, pauseSurface->h};
        SDL_RenderCopy(renderer, pauseTexture, NULL, &pauseRect);

        // Giải phóng bộ nhớ
        SDL_FreeSurface(pauseSurface);
        SDL_DestroyTexture(pauseTexture);

        SDL_RenderPresent(renderer);

        // Tiếp tục vòng lặp chính
        continue;
        }
        // Di chuyển
        switch (dir)
        {
            case DOWN:
                head.y += GRID_SIZE;
                break;
            case UP:
                head.y -= GRID_SIZE;
                break;
            case RIGHT:
                head.x += GRID_SIZE;
                break;
            case LEFT:
                head.x -= GRID_SIZE;
                break;
            default:
                break;
        }
        snake.push_front(std::make_pair(head.x, head.y));
		if(head.x < 0 )
        {
            head.x = WINDOW_WIDTH;
        }
        if(head.x > WINDOW_WIDTH)
        {
            head.x = 0;
        }
        if(head.y < 0)
        {
            head.y = WINDOW_HEIGHT;
        }
        if(head.y > WINDOW_HEIGHT)
        {
            head.y = 0;
        }
        // Kiểm tra rắn ăn táo
        if (head.x == apple.first && head.y == apple.second)
        {
            // Tạo vị trí mới cho quả táo
            score++;
            Mix_PlayChannel(-1, eatSound, 0);
            apple = std::make_pair(rand() % (WINDOW_WIDTH / GRID_SIZE) * GRID_SIZE, rand() % (WINDOW_HEIGHT / GRID_SIZE) * GRID_SIZE);
        }
        else
        {
            // Cắt bỏ phần đuôi
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
           // Giải phóng
            SDL_RenderClear(renderer);
            
            // Vẽ GameOver
            SDL_RenderCopy(renderer, gameOverTexture, NULL, NULL);

            // Cập nhật màn hình
            SDL_RenderPresent(renderer);

            SDL_Delay(2000);

            goto StartGame;
            // Trở lại StartGame
        }

        // Giải phóng cửa sổ
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Vẽ con rắn
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (auto &segment : snake)
        {
            SDL_Rect rect = {segment.first, segment.second, GRID_SIZE, GRID_SIZE};
            SDL_RenderFillRect(renderer, &rect);
        }

        // Vẽ quả táo
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect appleRect = {apple.first, apple.second, GRID_SIZE, GRID_SIZE};
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

        // Giải phóng bộ nhớ
        SDL_FreeSurface(scoreSurface);
        SDL_DestroyTexture(scoreTexture);


        SDL_RenderPresent(renderer);
        SDL_Delay(100); 
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