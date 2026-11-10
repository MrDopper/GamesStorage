#pragma once
#include "SDL3/SDL.h"
struct Laser
{
    const float LASER_W = 24.0f;
    const float LASER_H = 4.0f;
    static constexpr int MAX_LASERS = 3;
    const float LASER_SPEED = 500.0f;
    const SDL_Color LASER_COLOUR = {120, 255, 120, 255};
    SDL_FRect rect;
    bool active;
    void fireLaser(const SDL_FRect &playerRect);
};