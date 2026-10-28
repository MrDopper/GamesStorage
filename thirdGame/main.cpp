/*
    Square Invader

    A green player slides along the bottom of the window with A and D,
    or the arrow keys, and Space fires a yellow bullet. A red invader
    marches across the top, dropping a row at each wall. Shoot it to
    score, but let it reach your row and you lose a life. Lose all
    three, and the game is over. Escape, or the window's X, quits.
*/
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char *argv[])
{
    constexpr Uint16 WINDOWN_WIDTH = 800;
    constexpr Uint16 WINDOWN_H = 600;
    constexpr float PLAYER_SIZE = 40.0f;
    constexpr float BULLET_SIZE = 10.0f;
    constexpr float ENEMY_SIZE = 40.0f;
    constexpr float EDGE_GAP = 10.0f;
    constexpr float PLAYER_SPEED = 300.0f;
    constexpr float ENEMY_SPEED = 250.0f;
    constexpr float BULLET_SPEED = 600.0f;
    constexpr Uint8 LIVES = 3;

    constexpr SDL_Color BACKGROUND = {20, 20, 30, 255};     // near-black
    constexpr SDL_Color GAME_OVER_BG = {80, 0, 0, 255};     // dark red
    constexpr SDL_Color PLAYER_COLOUR = {0, 200, 0, 255};   // green
    constexpr SDL_Color PLAYER_DIM = {0, 80, 0, 255};       // dim green
    constexpr SDL_Color BULLET_COLOUR = {255, 255, 0, 255}; // yellow
    constexpr SDL_Color ENEMY_COLOUR = {200, 0, 0, 255};    // Red

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init cannot be initialize: %s", SDL_GetError());
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("Square Invader", WINDOWN_WIDTH, WINDOWN_H, 0);
    if (!window)
    {
        SDL_Log("SDL_Window cannot be intitalize: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer)
    {
        SDL_Log("SDL_Renderer cannot be initialize: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    // Show each frame in step with the monitor's refresh
    SDL_SetRenderVSync(renderer, 1);
    // The player: its top-left corner, centered near the bottom
    float playerX = (WINDOWN_WIDTH - PLAYER_SIZE) / 2.0f;
    float playerY = WINDOWN_H - PLAYER_SIZE - EDGE_GAP;

    // the bullet: only one can be flying at a time
    float bulletX = 0.0f;
    float bulletY = 0.0f;
    bool bulletActive = false;

    // The enemy: starts in the top-left corner, heading right
    float enemyX = 0.0f;
    float enemyY = EDGE_GAP;
    float enemySpeed = ENEMY_SPEED; // + is right, - is left

    int score = 0;
    int lives = LIVES;
    bool gameOver = false;
    bool running = true;
    Uint64 lastTime = SDL_GetTicks();
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                running = false;
                break;

            case SDL_EVENT_KEY_DOWN:
                if (event.key.key == SDLK_ESCAPE)
                {
                    running = false;
                }
                else if ((event.key.key == SDLK_SPACE) && !bulletActive && !gameOver)
                {
                    // Fire: start the bullet just above the player
                    bulletX = playerX + (PLAYER_SIZE - BULLET_SIZE) / 2.0f;
                    bulletY = playerY - bulletY;
                    bulletActive = true;
                }
                break;
            }
        }
        // Getting deltaTime
        Uint64 now = SDL_GetTicks();
        float delta = (now - lastTime) / 1000.0f;
        lastTime = now;
        // The player: slide while A or D, or an arrow key, is held
        if (!gameOver)
        {
            const auto *keys = SDL_GetKeyboardState(nullptr);
            if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
            {
                playerX -= PLAYER_SPEED * delta;
            }
            if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
            {
                playerX += PLAYER_SPEED * delta;
            }
            // Keep the whole player inside the window
            playerX = SDL_clamp(playerX, 0.0f, WINDOWN_WIDTH - PLAYER_SIZE);
        }
        // The enemy: march sideways, dropping a row at each wall
        if (!gameOver)
        {
            enemyX += enemySpeed * delta;
            if (enemyX + ENEMY_SIZE > WINDOWN_WIDTH)
            {
                enemyX = WINDOWN_WIDTH - ENEMY_SIZE;
                enemyY += ENEMY_SIZE;      // Drop a row
                enemySpeed = -ENEMY_SPEED; // Head left
            }
            else if (enemyX < 0.0f)
            {
                enemyX = 0.0f;
                enemyY += ENEMY_SIZE;
                enemySpeed = ENEMY_SPEED; // head right
            }
            if (enemyY + ENEMY_SIZE >= playerY)
            {
                lives--;
                SDL_Log("The enemy got through! Lives left: %d", lives);
                // reset
                enemyX = 0.0f;
                enemyY = EDGE_GAP;
                enemySpeed = ENEMY_SPEED;

                gameOver = (lives <= 0);
                if (gameOver)
                    SDL_Log("Game over! Final score: %d", score);
            }
        }
        // bullet: fly up, and. vanish off the top or on a hit
        if (bulletActive && !gameOver)
        {
            bulletY -= BULLET_SPEED * delta;
            if (bulletY + BULLET_SIZE < 0.0f)
            {
                bulletActive = false; // gone off the top
            }
            // Checking if the bullet and the enemy overlap on both axes.
            bool overlapX = (bulletX < enemyX + ENEMY_SIZE) && (bulletX + BULLET_SIZE > enemyX);
            bool overlapY = (bulletY < enemyY + ENEMY_SIZE) && (bulletY + BULLET_SIZE > enemyY);
            if (overlapX && overlapY)
            {
                score++;
                SDL_Log("Hit score is: %d", score);
                bulletActive = false;
                enemyX = 0.0f;
                enemyY = EDGE_GAP;
                enemySpeed = ENEMY_SPEED;
            }
        }
        // Drawing frames
        SDL_Color backgrounColor = gameOver ? GAME_OVER_BG : BACKGROUND;
        SDL_SetRenderDrawColor(renderer, backgrounColor.r, backgrounColor.g, backgrounColor.b, backgrounColor.a);
        SDL_RenderClear(renderer);
        // Create Player
        SDL_Color playerColor = gameOver ? PLAYER_DIM : PLAYER_COLOUR;
        SDL_SetRenderDrawColor(renderer, playerColor.r, playerColor.g, playerColor.b, playerColor.a);
        SDL_FRect playerRect = {playerX, playerY, PLAYER_SIZE, PLAYER_SIZE};
        SDL_RenderFillRect(renderer, &playerRect);
        // Create Enemy
        SDL_SetRenderDrawColor(renderer, ENEMY_COLOUR.r, ENEMY_COLOUR.g, ENEMY_COLOUR.b, ENEMY_COLOUR.a);
        SDL_FRect enemyRect = {enemyX, enemyY, ENEMY_SIZE, ENEMY_SIZE};
        SDL_RenderFillRect(renderer, &enemyRect);
        // The bullet, only while it's flying
        if (bulletActive)
        {
            SDL_SetRenderDrawColor(renderer, BULLET_COLOUR.r, BULLET_COLOUR.g, BULLET_COLOUR.b, BULLET_COLOUR.a);
            SDL_FRect bulletRect = {bulletX, bulletY, BULLET_SIZE, BULLET_SIZE};
            SDL_RenderFillRect(renderer, &bulletRect);
        }
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}