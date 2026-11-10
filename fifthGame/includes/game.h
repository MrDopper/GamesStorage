#pragma once
#include "player.h"
#include "enemy.h"
#include "laser.h"
struct Game
{
    static constexpr Uint16 WINDOW_W = 600;
    static constexpr Uint16 WINDOW_H = 800;
    const SDL_Color BACKGROUND = {10, 10, 30, 255};
    const SDL_Color GAME_OVER_BG = {60, 0, 0, 255};
    Player player;
    Laser lasers[Laser::MAX_LASERS];
    Enemy enemies[Enemy::MAX_ENEMY];
    const int POINTS_PER_HIT = 10;
    int score;
    int highScore;
    bool gameOver;
    // Put everything back the way it is at the start of a game
    void resetGame();
    /// @brief: Draw the frame: the background, then the aliens, ship, and lives
    void drawGame(SDL_Renderer *renderer);
    void drawRect(SDL_Renderer *renderer, const SDL_FRect &rect, const SDL_Color &color);
    void updateEnimies(float delta);
    /// @brief This function will shoot the laser out
    void fireLaser();

    void updateLaser(float delta);
    /// @brief: Collision check
    bool rectsOverlap(const SDL_FRect &a, const SDL_FRect &b);
    /// @brief Did a laser hit an enemy? Score it, and free both slots
    void checkLaserHits();
    /// @brief Did an enemy hit the player? Reduce one health point. 
    void checkPlayerhits();
};