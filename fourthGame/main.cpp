#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char *argv[])
{
    constexpr Uint16 WINDOW_H = 800;
    constexpr Uint16 WINDOW_W = 600;
    constexpr float BOX = 30.0f;
    constexpr float PADDING = 10.0f;
    constexpr float STEP = BOX + PADDING;

    const SDL_Color BACKGROUND = {30, 30, 30, 255}; // dark grey
    const SDL_Color COLOURS[] = {
        {220, 50, 50, 255},  // red
        {230, 140, 30, 255}, // Orange
        {220, 140, 40, 255}, // Yellow
        {50, 180, 40, 255},  // Green
        {40, 140, 220, 255}, // blue
        {100, 60, 200, 255}, // indigo
        {170, 60, 200, 255}, // violet
        {50, 200, 180, 255}, // teal
        {200, 80, 130, 255}  // pink
    };
    const int COLOUR_COUNT = SDL_arraysize(COLOURS);

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init is not initialize: %s", SDL_GetError());
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("Cascade", WINDOW_W, WINDOW_H, 0);
    if (!window)
    {
        SDL_Log("SDL_Window failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer)
    {
        SDL_Log("SDL_renderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    // Show each frame in step with the monitor's referesh
    SDL_SetRenderVSync(renderer, 1);
    // Game loop
    bool running = true;
    Uint64 startTime = SDL_GetTicks();
    SDL_Event event;
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
        // Draw frame
        SDL_SetRenderDrawColor(renderer, BACKGROUND.r, BACKGROUND.g, BACKGROUND.b, BACKGROUND.a);
        SDL_RenderClear(renderer);
        // Shift the colours along one place every 100 miliseconds
        int offset = static_cast<int>((SDL_GetTicks() - startTime) / 100);
        // The cascade: squares marching diagonally from the top-left
        float x = PADDING;
        float y = PADDING;
        for (int i = 0;; i++)
        {
            if (x + BOX > WINDOW_W || y + BOX > WINDOW_H)
            {
                break;
            }
            // Pick this square's colour, going around the palette
            SDL_Color colour = COLOURS[(i + offset) % COLOUR_COUNT];
            SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
            SDL_FRect square = {x, y, BOX, BOX};
            SDL_RenderFillRect(renderer, &square);
            // Move along the diagonal to the next square's spot
            x += STEP;
            y += STEP;
        }
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}