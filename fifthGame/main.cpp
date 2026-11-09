#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "game.h"
/**
 * This game is a one person shooting game.
 * version: 0.0.1;
 * Written: Jackson;
 */
int main(int argc, char *argv[])
{
    // Screen setup
    // Start SDL, then make the window and the renderer
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("Shooting Game", Game::WINDOW_W, Game::WINDOW_H, 0);
    if (!window)
    {
        SDL_Log("SDL_Window failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer)
    {
        SDL_Log("SDL_Renderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    // Show each frame in step with the monitor's refesh
    SDL_SetRenderVSync(renderer, 1);
    bool running = true;
    SDL_Event event;
    Game game;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)
            {
                running = false;
            }
        }
        game.drawGame(renderer, game);
    }
    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}