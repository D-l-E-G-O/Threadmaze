#pragma once
#include "maze.h"
#include "player.h"

typedef struct {
    int x;
    int y;
    bool active;
} Enemy;

/**
 * Initializes an array of enemies at random positions (away from player).
 * @param enemies Array of Enemy structures.
 * @param count Number of enemies.
 * @param maze Pointer to the maze (to find valid spots).
 * @param player Pointer to the player (to avoid spawning on top).
 */
void enemies_init(Enemy *enemies, int count, const Maze *maze, const Player *player);

/**
 * Moves a single enemy towards the player.
 * Simple AI: tries to reduce horizontal or vertical distance.
 * @param enemy Pointer to the enemy.
 * @param maze Pointer to the maze (collision check).
 * @param player Pointer to the player (target).
 */
void enemy_move(Enemy *enemy, const Maze *maze, const Player *player);