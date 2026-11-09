#pragma once
#include "SDL3/SDL.h"
struct Player
{
    const float PLAYER_SPEED = 300.0f;
    static constexpr int START_LIVES = 3;
    const SDL_Color PLAYER_COLOUR = {0, 200, 0, 255};
    const SDL_Color LASER_COLOUR = {120, 255, 120, 255};
    static constexpr float PLAYER_START_X = 40.0f;
    static constexpr float PLAYER_W = 64.0f;
    static constexpr float PLAYER_H = 48.0f;
    SDL_FRect rect;
    int lives;
};