#include "enemy.h"
#include "game.h"

void Enemy::spawnEnemy(Enemy &enemy)
{
    enemy.rect = {Game::WINDOW_W + randomBetween(0.0f, SPAWN_SPREAD), randomBetween(0.0f, Game::WINDOW_H - ENEMY_SIZE), ENEMY_SIZE, ENEMY_SIZE};
    enemy.speed = randomBetween(ENEMY_MIN_SPD, ENEMY_MAX_SPD);
    enemy.active = true;
}

void Enemy::updateEnemy(float delta)
{
    if (!active)
    {
        // Passing the current enemy
        spawnEnemy(*this);
    }
    // Update its position and move it toward the player
    rect.x -= speed * delta;
    if (rect.x + rect.w < 0.0f) // If the enemy got past: free the slot.
    {
        active = false;
    }
}
