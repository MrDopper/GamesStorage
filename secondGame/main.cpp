#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char *argv[])
{

    constexpr Uint16 WINDOW_WIDTH = 600;
    constexpr Uint16 WINDOW_HEIGHT = 700;
    constexpr float BALL_RADIUS = 30.0f;
    constexpr float BALL_SIZE = BALL_RADIUS * 2.0f;
    constexpr float START_VEL_X = 300.0f;                 // start velocity of x
    constexpr float START_VEL_Y = 250.0f;                 // start velocity of y
    constexpr SDL_Color BACKGROUND = {20, 24, 40, 255};   // Deep blue black
    constexpr SDL_Color BALL_COLOUR = {255, 70, 70, 255}; // bright red

    // Start SDL, then make the window and the renderer
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_init failed: %s", SDL_GetError());
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("bouncing ball", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window)
    {
        SDL_Log("SDL_Window failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer)
    {
        SDL_Log("SDL_init failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    // Show each frame in step with the monitor's refresh
    SDL_SetRenderVSync(renderer, 1);
    // Ball's location
    float ball_x = (WINDOW_WIDTH - BALL_SIZE) / 2.0f;
    float ball_y = (WINDOW_HEIGHT - BALL_SIZE) / 2.0f;
    float BallVelX = START_VEL_X;
    float BallVelY = START_VEL_Y;
    bool running = true;
    // The time at the last frame, in miliseconds
    Uint64 lastTime = SDL_GetTicks();
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
        // The time at the current moment, in miliseconds
        Uint64 now = SDL_GetTicks();
        float deltaTime = (now - lastTime) / 1000.0f;
        lastTime = now;
        // Move the ball: velocity times the time that has passed
        ball_x += BallVelX * deltaTime;
        ball_y += BallVelY * deltaTime;
        // Bounce off the left and right walls
        if (ball_x - BALL_RADIUS < 0.0f)
        {
            ball_x = BALL_RADIUS; // Snap back inside
            BallVelX = -BallVelX; //...then rerverse
        }
        if (ball_x + BALL_RADIUS > WINDOW_WIDTH)
        {
            ball_x = WINDOW_WIDTH - BALL_RADIUS;
            BallVelX = -BallVelX;
        }
        if (ball_y - BALL_RADIUS < 0.0f)
        {
            ball_y = BALL_RADIUS;
            BallVelY = -BallVelY;
        }
        if (ball_y + BALL_RADIUS > WINDOW_HEIGHT)
        {
            ball_y = WINDOW_HEIGHT - BALL_RADIUS;
            BallVelY = -BallVelY;
        }
        SDL_SetRenderDrawColor(renderer, BACKGROUND.r, BACKGROUND.g, BACKGROUND.b, BACKGROUND.a);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, BALL_COLOUR.r, BALL_COLOUR.g, BALL_COLOUR.b, BALL_COLOUR.a);
        SDL_FRect BallRect = {ball_x - BALL_RADIUS, ball_y - BALL_RADIUS, BALL_SIZE, BALL_SIZE};
        SDL_RenderFillRect(renderer, &BallRect);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}