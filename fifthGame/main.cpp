#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "game.h"
/**
 * This game is a one person shooting game.
 * version: 0.0.1;
 * Written: Jackson;
 * To move the ship, press A,W,S,D.
 * To shoot, press space.
 * For each enemies you destroy, you'll get 10 points. If the enemy hits you, you'll lose a life.
 * You only have 3 lives so try your best to not lose.
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
    // The whole game: the {} starts every member at zero
    Game game{};
    game.resetGame();
    // The time at the last frame, in miliseconds
    Uint64 lastTime = SDL_GetTicks();
    bool running = true;
    SDL_Event event;
    while (running)
    {
        // Events: the window's X, and key presses (not held-key repeats)
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat)
            {
                if (event.key.key == SDLK_ESCAPE)
                {
                    running = false;
                }
                else if (event.key.key == SDLK_SPACE && !game.gameOver)
                {
                    game.fireLaser();
                }
                else if (event.key.key == SDLK_R && game.gameOver)
                {
                    game.resetGame();
                }
            }
        }
        Uint64 now = SDL_GetTicks();
        float delta = (now - lastTime) / 1000.0f;
        lastTime = now;
        // Update the game, unless it's over
        if (!game.gameOver)
        {
            game.player.updatePlayer(delta);
            game.updateEnimies(delta);
            game.updateLaser(delta);
            game.checkLaserHits();
            game.checkPlayerhits();
        }
        game.drawGame(renderer);
    }
    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
