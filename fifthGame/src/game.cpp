#include "game.h"

void Game::resetGame()
{
    player.rect = {player.PLAYER_START_X, (WINDOW_H - player.PLAYER_H) / 2.0f, player.PLAYER_W, player.PLAYER_H};
    player.lives = player.START_LIVES;
    for (int i = 0; i < Laser::MAX_LASERS; i++)
    {
        lasers[i].active = false;
    }
    for (int i = 0; i < Enimes::MAX_ALIENS; i++)
    {
        enimes[i].active = false;
    }
    score = 0;
    gameOver = false;
}

void Game::drawGame(SDL_Renderer *renderer)
{
    SDL_Color backgroundColour = gameOver ? GAME_OVER_BG : BACKGROUND;
    SDL_SetRenderDrawColor(renderer, backgroundColour.r, backgroundColour.g, backgroundColour.b, backgroundColour.a);
    SDL_RenderClear(renderer);
    SDL_Color shipColour = gameOver ? player.PLAYER_DIM : player.PLAYER_COLOUR;
    drawRect(renderer,player.rect, shipColour);
    // one small square for each life left, int the top left corner
    for (int i = 0; i < player.lives; i++)
    {
        SDL_FRect life = {player.LIFE_GAP + i * (player.LIFE_SIZE + player.LIFE_GAP), player.LIFE_GAP, player.LIFE_SIZE, player.LIFE_SIZE};
        drawRect(renderer, life, player.PLAYER_COLOUR);
    }
    SDL_RenderPresent(renderer);
}

void Game::drawRect(SDL_Renderer *renderer, const SDL_FRect &rect, const SDL_Color &color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

// void Game::updatePLayer(Player &player, float delta)
// {
//     const auto *key = SDL_GetKeyboardState(nullptr);
//     if (key[SDL_SCANCODE_W])
//     {
//         player.rect.y -= player.PLAYER_SPEED * delta;
//     }
//     if (key[SDL_SCANCODE_S])
//     {
//         player.rect.y += player.PLAYER_SPEED * delta;
//     }
//     if (key[SDL_SCANCODE_D])
//     {
//         player.rect.x += player.PLAYER_SPEED * delta;
//     }
//     if (key[SDL_SCANCODE_A])
//     {
//         player.rect.x -= player.PLAYER_SPEED * delta;
//     }
//     player.rect.x = SDL_clamp(player.rect.x, 0.0f, WINDOW_W - player.PLAYER_W);
//     player.rect.y = SDL_clamp(player.rect.y, 0.0f, WINDOW_H - player.PLAYER_H);
// }