#include "Menu.h"

SDL_Event Menu::event;

Menu::Menu()
{}
Menu::~Menu()
{}

void Menu::ExitGame()
{
    SDL_Quit();
    exit(0);
}

void Menu::ShowTutorial(SDL_Renderer* renderer, TTF_Font* font)
{
    SDL_Event event;
    bool isRunning = true;

    while (isRunning) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Vẽ hướng dẫn
        SDL_Color textColor = {255, 255, 255, 255};

        std::string tutorialText = "Welcome to Snake Game!";
        SDL_Surface* tutorialSurface = TTF_RenderText_Solid(font, tutorialText.c_str(), textColor);
        SDL_Texture* tutorialTexture = SDL_CreateTextureFromSurface(renderer, tutorialSurface);
        SDL_Rect tutorialRect = {50, 50, tutorialSurface->w, tutorialSurface->h};
        SDL_RenderCopy(renderer, tutorialTexture, NULL, &tutorialRect);
        SDL_FreeSurface(tutorialSurface);
        SDL_DestroyTexture(tutorialTexture);

        TTF_Font* small = TTF_OpenFont("Point.ttf",20);
        std::string Line1 = "This product is based on the basic snake game and does not have many features or graphics. The gameplay is very basic, similar to regular snake games when you press the movement buttons to move the snake's direction and eat the apples drawn on the screen to grow. Press Back to return the main screen ";
        SDL_Surface* Line1Surface = TTF_RenderText_Blended_Wrapped(small, Line1.c_str(), textColor, 600);
        SDL_Texture* Line1Texture = SDL_CreateTextureFromSurface(renderer, Line1Surface);
        SDL_Rect Line1Rect = {50, 120, Line1Surface->w, Line1Surface->h};
        SDL_RenderCopy(renderer, Line1Texture, NULL, &Line1Rect);
        SDL_FreeSurface(Line1Surface);
        SDL_DestroyTexture(Line1Texture);

        TTF_Font* NMQ = TTF_OpenFont("Point.ttf", 20);
        std::string Name = "This project was created by Nguyen Minh Quang";
        SDL_Surface* NameSurface = TTF_RenderText_Blended_Wrapped(NMQ, Name.c_str(), textColor, 100);
        SDL_Texture* NameTexture = SDL_CreateTextureFromSurface(renderer, NameSurface);
        SDL_Rect NameRect = {600, 420, NameSurface->w, NameSurface->h};
        SDL_RenderCopy(renderer, NameTexture, NULL, &NameRect);
        SDL_FreeSurface(NameSurface);
        SDL_DestroyTexture(NameTexture);
        

        std::string backButton = "Back";
        SDL_Surface* backSurface = TTF_RenderText_Solid(font, backButton.c_str(), textColor);
        SDL_Texture* backTexture = SDL_CreateTextureFromSurface(renderer, backSurface);
        SDL_Rect backButtonRect = {50, 250, backSurface->w, backSurface->h};
        SDL_RenderCopy(renderer, backTexture, NULL, &backButtonRect);
        SDL_FreeSurface(backSurface);
        SDL_DestroyTexture(backTexture);

        SDL_RenderPresent(renderer);

        // Xử lý sự kiện
       while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                ExitGame();
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (mouseX >= backButtonRect.x && mouseX <= backButtonRect.x + backButtonRect.w &&
                    mouseY >= backButtonRect.y && mouseY <= backButtonRect.y + backButtonRect.h) {
                    // Nếu người dùng chọn Back
                    isRunning = false; // Quay lại menu
                }
            }
        }
    }
}

void Menu::ShowMenu(SDL_Renderer* renderer, TTF_Font* font) {
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

        SDL_Surface* TutorialSurface = TTF_RenderText_Solid(font, "Tutorial", textColor);
        SDL_Texture* TutotialTexture = SDL_CreateTextureFromSurface(renderer, TutorialSurface);
        SDL_Rect TutorialRect = {300, 300, TutorialSurface->w, TutorialSurface->h};
        SDL_RenderCopy(renderer, TutotialTexture, NULL, &TutorialRect);
        SDL_FreeSurface(TutorialSurface);
        SDL_DestroyTexture(TutotialTexture);

        SDL_Surface* exitSurface = TTF_RenderText_Solid(font, "Exit", textColor);
        SDL_Texture* exitTexture = SDL_CreateTextureFromSurface(renderer, exitSurface);
        SDL_Rect exitRect = {300, 400, exitSurface->w, exitSurface->h};
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
                    // Nếu người dùng chọn Play Game
                    isRunning = false;
                }
                else if(mouseX >= TutorialRect.x && mouseX <= TutorialRect.x + TutorialRect.w &&
                    mouseY >= TutorialRect.y && mouseY <= TutorialRect.y + TutorialRect.h)
                {
                    SDL_RenderClear(renderer); // Xóa màn hình hiện tại
        ShowTutorial(renderer, font); // Hiển thị màn hình hướng dẫn
        SDL_RenderPresent(renderer);
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
