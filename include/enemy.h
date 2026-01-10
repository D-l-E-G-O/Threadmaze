#pragma once
#include "maze.h"
#include "player.h"

typedef struct {
    int x;
    int y;
    int origin_x;
    int origin_y;
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
 * Moves an enemy. Uses A* if player is close, random patrol otherwise.
 * @param enemy Pointer to the enemy to move.
 * @param maze Pointer to the maze.
 * @param player Pointer to the player (target).
 * @param detection_range Distance to trigger pursuit.
 * @param patrol_range Max distance from origin allowed when patrolling.
 */
void enemy_move(Enemy *enemy, const Maze *maze, const Player *player, int detection_range, int patrol_range);