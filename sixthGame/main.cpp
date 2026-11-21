#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <string>

const int WINDOW_W = 800;
const int WINDOW_H = 600;
const float MAX_DELTA = 0.1f; // The longest frame we'll allow, in seconds

// Every picture in the game, each one starting as "not loaded yet"
struct Textures
{
    SDL_Texture *grass = nullptr;
    SDL_Texture *holeBack = nullptr;
    SDL_Texture *holdFront = nullptr;
    SDL_Texture *mole = nullptr;
    SDL_Texture *moleBonked = nullptr;
    SDL_Texture *mallet = nullptr;
    SDL_Texture *star = nullptr;
    SDL_Texture *title = nullptr;
    SDL_Texture *timesUp = nullptr;
};

SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *path)
{
    SDL_Texture *texture = IMG_LoadTexture(renderer, path);
    if (texture == nullptr)
        SDL_Log("Couldn't load %s: %s", path, SDL_GetError());
    return texture;
}

// Load every picture, and report whether they all loaded
bool loadTextures(SDL_Renderer *renderer, Textures &t)
{
    t.grass = loadTexture(renderer, "assets/grass.png");
    t.holeBack = loadTexture(renderer, "assets/hole_back.png");
    t.holdFront = loadTexture(renderer, "assets/hole_front.png");
    t.mole = loadTexture(renderer, "assets/mole.png");
    t.moleBonked = loadTexture(renderer, "assets/moleBonked.png");
    t.mallet = loadTexture(renderer, "assets/mallet.png");
    t.star = loadTexture(renderer, "assets/star.png");
    t.title = loadTexture(renderer, "assets/title.png");
    t.timesUp = loadTexture(renderer, "assets/timesUp.png");
    return t.grass && t.holdFront && t.holeBack && t.mole && t.mallet && t.moleBonked && t.star && t.title && t.timesUp;
}
void destroyTexxture(SDL_Texture *&texture)
{
    if (texture != nullptr)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}
int main(int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Cannot initialize SDL: %s", SDL_GetError());
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("Whack a Mole", WINDOW_W, WINDOW_H, 0);
    if (!window)
    {
        SDL_Log("Cannot create window: %s", SDL_GetError());
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
    SDL_SetRenderVSync(renderer, 1);
    bool running = true;
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
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}