#pragma once
#include "SDL3/SDL.h"
struct Laser
{
    const float LASER_W = 24.0f;
    const float LASER_H = 4.0f;
    static constexpr int MAX_LASERS = 3;
    const int POINTS_PER_HIT = 10;
    SDL_FRect rect;
    bool active;
};