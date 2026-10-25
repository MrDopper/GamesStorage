#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
int main(int argc, char *argv[])
{
    // setup
    const Uint16 WINDOW_W = 1280;
    const Uint16 WINDOW_H = 720;
    const float SQUARE_SZ = 80.0f; // square_size
    const float SPEED = 600.0f;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Int failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("controlLableSquare", WINDOW_W, WINDOW_H, 0);
    if (!window)
    {
        SDL_Log("SDL_CreateWindow is failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer)
    {
        SDL_Log("SDL_CreateRenderer is failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    // Show each frame in step with the monitor's refresh
    SDL_SetRenderVSync(renderer, 1); // It will wait for the monitor to be ready before rendering new frame
    float x = (WINDOW_W - SQUARE_SZ) / 2.0f;
    float y = (WINDOW_H - SQUARE_SZ) / 2.0f;
    Uint64 lastTime = SDL_GetTicks(); // Get time at the last frame, in milliseconds
    bool running = true;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            // If clicking window's x => terminal
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
            // If clicking esc => terminal
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)
            {
                running = false;
            }
        }
        // Delta time: how many seconds the last frame took
        Uint64 now = SDL_GetTicks();
        float delta = (now - lastTime) / 1000.0f;
        lastTime = now;
        // Movement: check which keys are held down
        const auto *keys = SDL_GetKeyboardState(nullptr);
        if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])
        {
            y -= SPEED * delta;
        }
        if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])
        {
            y += SPEED * delta;
        }
        if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
        {
            x -= SPEED * delta;
        }
        if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
        {
            x += SPEED * delta;
        }
        // Keeping the square inside the box
        x = SDL_clamp(x, 0.0f, WINDOW_W - SQUARE_SZ);
        y = SDL_clamp(y, 0.0f, WINDOW_H - SQUARE_SZ);
        // Draw Frames
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 140, 200, 0, 255);
        SDL_FRect square = {x, y, SQUARE_SZ, SQUARE_SZ};
        SDL_RenderFillRect(renderer, &square);
        SDL_RenderPresent(renderer);
    }
    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}