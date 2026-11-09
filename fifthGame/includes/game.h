#pragma once
#include "player.h"
#include "enimes.h"
#include "laser.h"
struct Game
{
    static constexpr Uint16 WINDOW_W = 600;
    static constexpr Uint16 WINDOW_H = 800;
    const SDL_Color BACKGROUND = {10, 10, 30, 255};
    const SDL_Color GAME_OVER_BG = {60, 0, 0, 255};
    Player player;
    Laser lasers[Laser::MAX_LASERS];
    Enimes enimes[Enimes::MAX_ALIENS];
    int score;
    int highScore;
    bool gameOver;
    // Put everything back the way it is at the start of a game
    void resetGame(Game &game);
    // Draw the frame: the background, then the aliens, ship, and lives
    void drawGame(SDL_Renderer *renderer, const Game &game);
};