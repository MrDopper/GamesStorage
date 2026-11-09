#pragma once
#include "SDL3/SDL.h"
struct Player
{
    const float PLAYER_SPEED = 300.0f;
    const int START_LIVES = 3;
    const SDL_Color PLAYER_COLOUR = {0, 200, 0, 255};
    const SDL_Color PLAYER_DIM = {0, 80, 0, 255};
    const float PLAYER_START_X = 40.0f;
    const float PLAYER_W = 64.0f;
    const float PLAYER_H = 48.0f;
    const float LIFE_SIZE = 12.0f;
    const float LIFE_GAP = 8.0f;
    SDL_FRect rect;
    int lives;
    void updatePlayer(float delta);
};