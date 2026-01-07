#pragma once
#include <stdbool.h>
#include "maze.h"
#include "player.h"
#include "astar.h"
#include "timer.h"

typedef struct {
    AStarPath path;
    bool active;
    int duration;
    Timer *timer;
} Hint;

/**
 * Initializes the hint system.
 * It prepares the internal timer but does not start it.
 * @param hint Pointer to the Hint structure to initialize.
 * @param timer Pointer to the Timer structure to use for the hint duration.
 * @param duration The duration in seconds for which the hint should remain active.
 */
void hint_init(Hint *hint, Timer *timer, int duration);

/**
 * Frees the resources associated with the hint (specifically the path memory).
 * @param hint Pointer to the Hint structure.
 */
void hint_free(Hint *hint);

/**
 * Generates the optimal path from the player's position to the exit.
 * This function uses the A* algorithm.
 * @param maze Pointer to the maze.
 * @param player Pointer to the player (start position).
 * @param path Pointer to the AStarPath structure where the result will be stored.
 */
void hint_generate_path(Maze *maze, Player *player, AStarPath *path);

/**
 * Activates the hint: generates the path, displays it on the maze, and starts the timer.
 * If the hint is already active, it recalculates the path and restarts the timer.
 * @param hint Pointer to the Hint structure.
 * @param maze Pointer to the maze.
 * @param player Pointer to the player.
 */
void hint_activate(Hint *hint, Maze *maze, Player *player);

/**
 * Deactivates the hint: hides the path from the maze and stops the timer.
 * @param hint Pointer to the Hint structure.
 * @param maze Pointer to the maze.
 */
void hint_deactivate(Hint *hint, Maze *maze);

/**
 * Internal helper to apply or remove the visual symbols of the hint on the maze cells.
 * @param hint Pointer to the Hint structure.
 * @param maze Pointer to the maze.
 * @param symbol The string symbol to apply (e.g., HINT or CELL).
 */
void hint_apply_visuals(Hint *hint, Maze *maze, char *symbol);

/**
 * Updates the hint path based on the new player position without resetting the timer.
 * * @param hint Pointer to the Hint structure.
 * @param maze Pointer to the maze.
 * @param player Pointer to the player.
 */
void hint_update(Hint *hint, Maze *maze, Player *player);
