/* Name: Whack-a-Mole
Project description: Have you ever get furious because your garden is infested with moles?
This game will help you relief your stress by whacking a mole.
Created by: Jackson Vi
Version: V.0.0.1
Libraries: SDL3, SDL3_Image
*/
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

// The moles, and the round
const float MOLE_W = 120.0f;        // mole.png is 120 to 140
const float MOLE_OFFSET_X = 40.0f;  // the mole's left edge, from the hole's
const float MOLE_BASE_Y = 70.0f;    // Its bottom edge, hidden by the lip
const float MOLE_MAX_SHOW = 130.0f; // pixels showing when it's fully up
const float RISE_SPEED = 520.0f;    // pixels per second. going up and down
const float ROUND_TIME = 60.0f;     // second in a round
const float START_UP_TIME = 1.2f;   // how long a mole waits, at the start
const float END_UP_TIME = 0.55;     // and by the end of the round
const float START_SPAWN_GAP = 0.9f; // seconds between moles, at the start
const float END_SPAWN_GAP = 0.45f;  // and by the end
const int SPAWN_TRIES = 10;         // random holes to try for a new mole

// The five things a mole can be doing
enum class MoleState
{
    Hidden,
    Rising,
    Up,
    Bonked,
    Sinking
};
// ONe hole, and the mole that lives in it
struct Hole
{
    float x; // The hole's top-left corner on screen
    float y;
    MoleState state;
    float shown;      // How many pixels of the mole are showing
    float timer;      // Seconds left while it's Up or Boned
    float startTimer; // second left on this hole's star
    float facingLeft; // true to draw the mole flipped
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

// Everything that changes while you play
struct Game
{
    Hole holes[HOLE_COUNT];
    int score;
    float timeLeft;
    float spawnTimer; // seconds until the next mole comes up
    float swingTimer; // seconds left in the mallet's swing
    bool gameOver;
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

// Put everything hol in its place, send every mole down, and reset
void resetGame(Game &game)
{
    for (int i = 0; i < HOLE_COUNT; i++)
    {
        Hole &hole = game.holes[i];
        int col = i % HOLE_COLS;
        int row = i / HOLE_COLS;
        hole.x = FIRST_HOLE_X + col * HOLE_GAP_X;
        hole.y = FIRST_HOLE_Y + row * HOLE_GAP_Y;
        hole.state = MoleState::Hidden;
        hole.shown = 0.0f;
        hole.timer = 0.0f;
        hole.startTimer = 0.0f;
        hole.facingLeft = false;
    }
    game.score = 0;
    game.timeLeft = ROUND_TIME;
    game.spawnTimer = START_SPAWN_GAP;
    game.swingTimer = 0.0f;
    game.gameOver = false;
}

// A number partway from a to b: a when t is 0, and b when t is 1
/*lerp is short for linear interpolation, finds the number
that's a fraction t on the way from a to b. When t is 0, the answer is a,
and when it's 1, the answer is b.
In between, b - a is the whole distance, t times that is the part
of it we've convered, and adding a starts up from the right place.

precondition: Halfway through the round
lerp(1.2f, 0.55f, 0.5f) is 0.875, so the mole waits for 0.875 seconds
*/
float lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

// Where the showing part of a hole's mole is, on screen
SDL_FRect moleRect(const Hole &hole)
{
    SDL_FRect rect;
    rect.x = hole.x + MOLE_OFFSET_X;
    rect.y = hole.y + MOLE_BASE_Y - hole.shown;
    rect.w = MOLE_W;
    rect.h = hole.shown;
    return rect;
}
// send a mole up out of a random empty hole, if one turns up quickly
void spawnMole(Game &game)
{
    for (int attempt = 0; attempt < SPAWN_TRIES; attempt++)
    {
        Hole &hole = game.holes[SDL_rand(HOLE_COUNT)];
        if (hole.state == MoleState::Hidden)
        {
            hole.state = MoleState::Rising;
            hole.facingLeft = (SDL_rand(2) == 0);
            return;
        }
    }
}

// Move one mole through its life: up, a wait (or a bonk), and down again
void updateHole(Hole &hole, float delta, float upTime)
{
    switch (hole.state)
    {
    case MoleState::Hidden:
        break;

    case MoleState::Rising:
        hole.shown += RISE_SPEED * delta;
        if (hole.shown >= MOLE_MAX_SHOW)
        {
            hole.shown = MOLE_MAX_SHOW;
            hole.state = MoleState::Up;
            hole.timer = upTime;
        }
        break;
    case MoleState::Up:
    case MoleState::Bonked:
        hole.timer -= delta;
        if (hole.timer <= 0.0f)
            hole.state = MoleState::Sinking;
        break;

    case MoleState::Sinking:
        hole.shown -= RISE_SPEED * delta;
        if (hole.shown <= 0.0f)
        {
            hole.shown = 0.0f;
            hole.state = MoleState::Hidden;
        }
        break;
    }
    if (hole.startTimer > 0.0f)
    {
        hole.startTimer -= delta;
    }
}
// Draw one hole: its back, then its mole, then its front lip
void drawHole(SDL_Renderer *renderer, const Textures &t, const Hole &hole)
{
    SDL_FRect holeRect = {hole.x, hole.y, HOLE_W, HOLE_H};
    SDL_RenderTexture(renderer, t.holeBack, nullptr, &holeRect);
    if (hole.shown > 0.0f)
    {
        SDL_FRect src = {0.0f, 0.0f, MOLE_W, hole.shown};
        SDL_FRect dst = moleRect(hole);

        bool bonked = hole.state == MoleState::Bonked;
        SDL_Texture *picture = bonked ? t.moleBonked : t.mole;
        SDL_FlipMode flip = hole.facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        SDL_RenderTextureRotated(renderer, picture, &src, &dst, 0.0, nullptr, flip);
    }
    SDL_RenderTexture(renderer, t.holdFront, nullptr, &holeRect);
}

// Move the whole game on by one frame
void updateGame(Game &game, float delta)
{
    if (game.swingTimer > 0.0f)
    {
        game.swingTimer -= delta;

        // How far through the round we are: 0 at the start, and 1 at the end
        float progress = 1.0f - game.timeLeft / ROUND_TIME;
        float upTime = lerp(START_UP_TIME, END_UP_TIME, progress);

        for (int i = 0; i < HOLE_COUNT; i++)
        {
            updateHole(game.holes[i], delta, upTime);
        }
        if (game.gameOver)
        {
            return; // The moles finish what they're doing, but no more come
        }
        game.timeLeft -= delta;
        if (game.timeLeft <= 0.0f)
        {
            game.timeLeft = 0.0f;
            game.gameOver = true;
            return;
        }
        game.spawnTimer -= delta;
        if (game.spawnTimer <= 0.0f)
        {
            spawnMole(game);
            game.spawnTimer = lerp(START_SPAWN_GAP, END_SPAWN_GAP, progress);
        }
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
        // The whole game, in its starting state
        Game game{}; // creates the gam with every member set to zero
        resetGame(game);
        Uint64 now = SDL_GetTicks();
        float delta = (now - lastTime) / 1000.0f;
        lastTime = now;
        // Precondition: if detla time is greater than tenth of second
        if (delta > MAX_DELTA)
        {
            // Capped it.
            delta = MAX_DELTA;
        }
        updateGame(game, delta);
        // Draw the frame, starting with the grass.
        // This function draws texture over and over.
        // first nullptr means drawing all of it. second nullptr means drawing the whole window.
        SDL_RenderTextureTiled(renderer, textures.grass, nullptr, 1.0f, nullptr);
        for (int i = 0; i < HOLE_COUNT; i++)
            drawHole(renderer, textures, game.holes[i]);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}