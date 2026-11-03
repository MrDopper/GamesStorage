#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char* argv[]){
    constexpr Uint16 WINDOW_H = 800;
    constexpr Uint16 WINDOW_W = 600;
    constexpr float BOX = 30.0f;
    constexpr float PADDING = 2.0f;
    constexpr float STEP = BOX + PADDING; 

    const SDL_Color BACKGROUND = {30, 30, 30, 255}; // dark grey
    const SDL_Color COLOURS[] ={
        {220, 50, 50, 255}, //red
        {230, 140, 30, 255}, //Orange
        {220, 140, 40, 255}, // Yellow
        {50, 180, 40, 255}, //Green 
        {40, 140, 220, 255}, //blue
        {100, 60, 200, 255}, // indigo
        {170, 60, 200, 255}, //violet
        {50, 200, 180, 255}, //teal 
        {200, 80, 130, 255} // pink
    };
    const int COLOUR_COUNT = SDL_arraysize(COLOURS);

    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("SDL_Init is not initialize: %s", SDL_GetError());
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow("Casade", WINDOW_W, WINDOW_H, 0);
    if(!window){
        SDL_Log("SDL_Window failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if(!renderer){
        SDL_Log("SDL_renderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    //Show each frame in step with the monitor's referesh
    SDL_SetRenderVSync(renderer, 1);
    //Game loop
    bool running = true;
    SDL_Event event;
    while(running){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_EVENT_QUIT){
                running = false;
            }
            if(event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE){
                running = false;
            }
        }
    }

    return 0;
}