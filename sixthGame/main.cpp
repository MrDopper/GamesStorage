#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <string>

const int WINDOW_W = 800;
const int WINDOW_H = 600;
const float MAX_DELTA = 0.1f; // The longest frame we'll allow, in seconds

// The holes: three rows of three
const int HOLE_COLS = 3;
const int HOLE_ROWS = 3;
const int HOLE_COUNT = HOLE_COLS * HOLE_ROWS;
const float HOLE_W = 200.0f; // Giving pixels
const float HOLE_H = 100.0f;
// Set up the position for the first hole
const float FIRST_HOLE_X = 70.0f;
const float FIRST_HOLE_Y = 176.0f;
const float HOLE_GAP_X = 230.0f;
const float HOLE_GAP_Y = 160.0f;

//The five things a mole can be doing
enum class MoleState{
    Hidden,
    Rising,
    Up,
    Bonked,
    Sinking
};
//ONe hole, and the mole that lives in it
struct Hole{
    float x; //The hole's top-left corner on screen
    float y;
    MoleState state;
    float shown; //How many pixels of the mole are showing
    float timer; //Seconds left while it's Up or Boned
    float startTimer; //second left on this hole's star
    float facingLeft; //true to draw the mole flipped
};
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
void destroyTexture(SDL_Texture *&texture)
{
    if (texture != nullptr)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}
// Destroy every picture: safe even if some of them never loaded
void destroyTextures(Textures &t)
{
    destroyTexture(t.grass);
    destroyTexture(t.holeBack);
    destroyTexture(t.holdFront);
    destroyTexture(t.mole);
    destroyTexture(t.moleBonked);
    destroyTexture(t.mallet);
    destroyTexture(t.star);
    destroyTexture(t.star);
    destroyTexture(t.title);
    destroyTexture(t.timesUp);
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
    // Show each frame in step with the monitor's refresh
    SDL_SetRenderVSync(renderer, 1);

    // Let see-through colours belnd with whatever is behind them
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // The window's incon is a surface: a picture in ordinary memory
    SDL_Surface *icon = IMG_Load("assets/icon.png");
    if (icon != nullptr)
    {
        SDL_SetWindowIcon(window, icon);
        SDL_DestroySurface(icon); // The window keeps its own copy
    }

    // Load every pciture, or tidy up and stop if any of them is missing
    Textures textures;
    if (!loadTextures(renderer, textures))
    {
        destroyTextures(textures); // The ones that did load
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    Uint64 lastTime = SDL_GetTicks();
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
            if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat)
            {
                if (event.key.key == SDLK_ESCAPE)
                {
                    running = false;
                }
            }
        }
        Uint64 now = SDL_GetTicks();
        float delta = (now - lastTime) / 1000.0f;
        lastTime = now;
        // Precondition: if detla time is greater than tenth of second
        if (delta > MAX_DELTA)
        {
            // Capped it.
            delta = MAX_DELTA;
        }
        // Draw the frame, starting with the grass.
        // This function draws texture over and over.
        // first nullptr means drawing all of it. second nullptr means drawing the whole window.
        SDL_RenderTextureTiled(renderer, textures.grass, nullptr, 1.0f, nullptr);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}