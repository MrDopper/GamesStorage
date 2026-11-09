#pragma once
#include "SDL3/SDL.h"

struct Enimes
{
    const float ALIEN_SIZE = 48.0f;
    const float ALIEN_MIN_SPD = 120.0f;
    const float ALEIN_MAX_SPD = 300.0f;
    const float SPAWN_SPREAD = 200.0f;
    static constexpr int MAX_ALIENS = 12;
    const float LIFE_SIZE = 12.0f;
    const float LIFE_GAP = 8.0f;
    const SDL_Color ALIEN_COLOR = {200, 60, 60, 255};
    SDL_FRect rect;
    float speed;
    bool active;
};