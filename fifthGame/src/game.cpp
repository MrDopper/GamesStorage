#include "game.h"

void Game::resetGame(Game &game)
{
    game.player.rect = {Player::PLAYER_START_X, (WINDOW_H - WINDOW_W) / 2.0f, Player::PLAYER_W, Player::PLAYER_H};
    game.player.lives = player.START_LIVES;
    for (int i = 0; i < Laser::MAX_LASERS; i++)
    {
        game.lasers[i].active = false;
    }
    for (int i = 0; i < Enimes::MAX_ALIENS; i++)
    {
        game.enimes[i].active = false;
    }
    game.score = 0;
    game.gameOver = false;
}

void Game::drawGame(SDL_Renderer *renderer, const Game &game)
{
    SDL_Color backgroundColour = game.gameOver ? GAME_OVER_BG : BACKGROUND;
    SDL_SetRenderDrawColor(renderer, backgroundColour.r, backgroundColour.g, backgroundColour.b, backgroundColour.a);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}
