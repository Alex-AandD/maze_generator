#include <SDL2/SDL.h>
#include "maze.hh"
#include <iostream>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window;
    window = SDL_CreateWindow("My SDL Window",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              600, 600,
                              SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Maze maze(21, 21, 6, renderer);
    maze.generate();

    bool quit { false } ;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type)
            {
                case SDL_QUIT: quit = true;
                break;
            }
        }
        SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor( renderer, 255, 0, 255, 255);
        maze.drawGrid(600, 600);
        maze.drawMaze();

        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
