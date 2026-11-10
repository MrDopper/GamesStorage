#include "laser.h"
#include "player.h"

void Laser::fireLaser(const SDL_FRect &playerRect)
{
    rect = {playerRect.x + playerRect.w, playerRect.y + (playerRect.h - LASER_H) / 2.0f, LASER_W, LASER_H};
    active = true;
}


