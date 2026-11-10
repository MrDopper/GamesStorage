#include "game.h"

void Game::resetGame()
{
    player.rect = {player.PLAYER_START_X, (WINDOW_H - player.PLAYER_H) / 2.0f, player.PLAYER_W, player.PLAYER_H};
    player.lives = player.START_LIVES;
    for (int i = 0; i < Laser::MAX_LASERS; i++)
    {
        lasers[i].active = false;
    }
    for (int i = 0; i < Enemy::MAX_ENEMY; i++)
    {
        enemies[i].active = false;
    }
    score = 0;
    gameOver = false;
}

void Game::drawGame(SDL_Renderer *renderer)
{
    SDL_Color backgroundColour = gameOver ? GAME_OVER_BG : BACKGROUND;
    SDL_SetRenderDrawColor(renderer, backgroundColour.r, backgroundColour.g, backgroundColour.b, backgroundColour.a);
    SDL_RenderClear(renderer);
    for (Enemy &enemy : enemies)
    {
        if (enemy.active)
        {
            drawRect(renderer, enemy.rect, enemy.ENEMY_COLOR);
        }
    }
    for (Laser &laser : lasers)
    {
        if (laser.active)
        {
            drawRect(renderer, laser.rect, laser.LASER_COLOUR);
        }
    }
    SDL_Color playerColour = gameOver ? player.PLAYER_DIM : player.PLAYER_COLOUR;
    drawRect(renderer, player.rect, playerColour);
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
// Generate enemy
void Game::updateEnimies(float delta)
{
    for (Enemy &enemy : enemies)
    {
        enemy.updateEnemy(delta); // Think about enemies[0].updateEnemy(delta);
    }
}

void Game::fireLaser()
{
    for (Laser &laser : lasers)
    {
        if (!laser.active)
        {
            laser.fireLaser(player.rect);
            return; // One laser per press
        }
    }
}

void Game::updateLaser(float delta)
{
    for (Laser &laser : lasers)
    {
        if (!laser.active)
        {
            continue;
        }
        laser.rect.x += laser.LASER_SPEED * delta;
        if (laser.rect.x > WINDOW_W)
        {
            laser.active = false;
        }
    }
}

bool Game::rectsOverlap(const SDL_FRect &a, const SDL_FRect &b)
{

    bool overlapX = (a.x < b.x + b.w) && (a.x + b.w > b.x);
    bool overlapY = (a.y < b.y + b.h) && (a.y + b.h > b.y);
    return overlapX && overlapY;
}

void Game::checkLaserHits()
{
    for (Laser &laser : lasers)
    {
        if (!laser.active)
        {
            continue;
        }
        for (Enemy &enemy : enemies)
        {
            if (enemy.active && rectsOverlap(laser.rect, enemy.rect))
            {
                enemy.active = false;
                laser.active = false;
                score += POINTS_PER_HIT;
                SDL_Log("Hit! Score: %d", score);
                break;
            }
        }
    }
}

void Game::checkPlayerhits()
{
    for (Enemy &enemy : enemies)
    {
        if (enemy.active && rectsOverlap(enemy.rect, player.rect))
        {
            enemy.active = false;
            player.lives--;
            SDL_Log("Lives left: %d", player.lives);
            if (player.lives <= 0)
            {
                gameOver = true;
                if (score > highScore)
                {
                    highScore = score;
                }
                SDL_Log("Game over! Score: %d, best: %d. Press R to restart!", score, highScore);
                return;
            }
        }
    }
}
