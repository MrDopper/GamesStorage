#pragma once
#include "SDL3/SDL.h"

struct Enemy
{
    const float ENEMY_SIZE = 48.0f;
    const float ENEMY_MIN_SPD = 120.0f;
    const float ENEMY_MAX_SPD = 300.0f;
    const float SPAWN_SPREAD = 200.0f;
    static constexpr int MAX_ENEMY = 12;
    const SDL_Color ENEMY_COLOR = {200, 60, 60, 255};
    SDL_FRect rect;
    float speed;
    bool active;
    // A random number from low up to high
    float randomBetween(float low, float high)
    {
        return low + SDL_randf() * (high - low);
    }
    void spawnEnemy(Enemy &enemy);
    void updateEnemy(float delta);
};