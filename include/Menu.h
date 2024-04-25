#include "Snake.h"

class Menu
{
public:
   // void StartGame(SDL_Renderer *renderer, TTF_Font *font);
    Menu();
   ~Menu();
    void ExitGame();
    void ShowTutorial(SDL_Renderer* renderer, TTF_Font* font);
    void ShowMenu(SDL_Renderer *renderer, TTF_Font *font);
    static SDL_Event event;
    static SDL_Renderer* renderer;
private:
    bool isRunning = true;
};