#pragma once
#include "maze.h"

/**
 * Enum representing possible movement directions.
 */
typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_NONE
} Direction;

typedef struct {
    int x;
    int y;
} Player;

/**
 * Initializes the player position at (0,0) and sets the start/exit symbols.
 * @param player Pointer to the player structure.
 * @param maze Pointer to the maze.
 */
void player_init(Player *player, Maze *maze);

/**
 * Moves the player in the specified direction if the move is valid.
 * @param player Pointer to the player structure.
 * @param direction The direction to move towards.
 * @param maze Pointer to the maze for collision checking.
 */
void player_move(Player *player, Direction direction, Maze *maze);

/**
 * Checks if the player has reached the exit of the maze.
 * @param player Pointer to the player structure.
 * @param maze Pointer to the maze structure.
 * @return true if the player is at the exit coordinates, false otherwise.
 */
bool is_exit_reached(const Player *player, const Maze *maze);