#include "enemy.h"
#include "utils.h"
#include <stdlib.h>
#include <math.h>

void enemies_init(Enemy *enemies, int count, const Maze *maze, const Player *player) {
    if (!enemies || !maze || !player) return;

    for (int i = 0; i < count; i++) {
        enemies[i].active = true;
        
        // Find a random spot far enough from player
        int attempts = 0;
        do {
            enemies[i].x = random_int(0, maze->width - 1);
            enemies[i].y = random_int(0, maze->height - 1);
            attempts++;
            // Avoid spawning the enemy on the player or too close to the player (manhattan distance < 10)
        } while (attempts < 100 && (abs(enemies[i].x - player->x) + abs(enemies[i].y - player->y) < 10));
    }
}

void enemy_move(Enemy *enemy, const Maze *maze, const Player *player) {
    if (!enemy || !enemy->active || !maze || !player) return;

    int dx = player->x - enemy->x;
    int dy = player->y - enemy->y;
    int next_x = enemy->x;
    int next_y = enemy->y;

    // Determine intended move (Manhattan approach)
    // We prioritize the axis with the largest distance
    if (abs(dx) > abs(dy)) {
        // Try moving Horizontal
        next_x += (dx > 0) ? 1 : -1;
        
        // If blocked, try vertically
        if (!is_cell_valid(maze, enemy->x, enemy->y, next_x, next_y)) {
            next_x = enemy->x;
            next_y += (dy > 0) ? 1 : -1;
        }
    } else {
        // Try moving Vertical
        next_y += (dy > 0) ? 1 : -1;
        // If blocked, try horizontally
        if (!is_cell_valid(maze, enemy->x, enemy->y, next_x, next_y)) {
            next_y = enemy->y;
            next_x += (dx > 0) ? 1 : -1;
        }
    }

    // Final check and commit
    if (is_cell_valid(maze, enemy->x, enemy->y, next_x, next_y)) {
        enemy->x = next_x;
        enemy->y = next_y;
    }
}