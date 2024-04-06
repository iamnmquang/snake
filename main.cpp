#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <deque>
#include <vector>



int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Event e;

    enum Direction
    {
        DOWN,LEFT,RIGHT,UP
    };

    bool running = true;
    int dir = 0;
    SDL_Rect head {500,500,10,10};

    // Snake body container

    std::deque<SDL_Rect> rq;
    int siz = 5;
    // Apple container

    std::vector<SDL_Rect> apples;
    for(int i = 0; i < 100; i++)
    {
        apples.emplace_back(SDL_Rect{rand() % 100 * 10, rand() % 100 * 10, 10, 10});
    }

    while(running)
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                running = false;
            }
            if(e.type == SDL_KEYDOWN)
            {
                if(e.key.keysym.sym == SDLK_DOWN) {dir = DOWN;}
                 if(e.key.keysym.sym == SDLK_UP) {dir = UP;}
                  if(e.key.keysym.sym == SDLK_RIGHT) {dir = RIGHT;}
                   if(e.key.keysym.sym == SDLK_LEFT) {dir = LEFT;}
            }
        }
        // Move
        switch (dir)
        {
        case DOWN:
            head.y += 5;
            break;
        case UP:
            head.y -= 5;
            break;
        case RIGHT:
            head.x += 5;
            break;
        case LEFT:
            head.x -= 5;
            break;
        
        default:
            break;
        }
    // Collision detection
    std::for_each(apples.begin(), apples.end(), [&](auto &apple)
    {
        // if((head.x == apple.x || head.x == apple.y) && (head.y == apple.x || head.y == apple.y))
        // {
        //     siz += 5;
        //     apple.x = -5;
        //     apple.y = -5;
        // }

        if (head.x + head.w >= apple.x &&  
            apple.x + apple.w >= head.x &&
            head.y + head.h >= apple.y && 
            apple.y + apple.h >= head.y)
        {
            siz += 5;
            apple.x = -5;
            apple.y = -5;
        }
    });

    // Collision detection with snake body
    std::for_each(rq.begin(), rq.end(), [&](auto & snake_segment)
    {
        if(head.x == snake_segment.x && head.y == snake_segment.y)
        {
            siz = 5;
        }
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
    });

    // Add newest head
    rq.push_front(head);
    while((int)rq.size() > siz)
    {
        rq.pop_back();
    }

    // Clear Window
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);

    // Draw Body
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
   for (auto &segment : rq)
        {
            SDL_RenderFillRect(renderer, &segment);
        }


    // Draw apples
    SDL_SetRenderDrawColor(renderer, 255,255,0,255);
    for (auto &apple : apples)
        {
            SDL_RenderFillRect(renderer, &apple);
        }
    SDL_RenderPresent(renderer);
    SDL_Delay(25);
    }
   return 0;
}