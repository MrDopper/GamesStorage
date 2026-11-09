#include "player.h"
#include "game.h"

void Player::updatePlayer(float delta)
{
    const auto *key = SDL_GetKeyboardState(nullptr);
    const float speed = PLAYER_SPEED * delta;
    if (key[SDL_SCANCODE_W])
    {
        rect.y -= speed;
    }
    if (key[SDL_SCANCODE_S])
    {
        rect.y += speed;
    }
    if (key[SDL_SCANCODE_D])
    {
        rect.x += speed;
    }
    if (key[SDL_SCANCODE_A])
    {
        rect.x -= speed;
    }
    rect.x = SDL_clamp(rect.x, 0.0f, Game::WINDOW_W - rect.w);
    rect.y = SDL_clamp(rect.y, 0.0f, Game::WINDOW_H - rect.h);
}
