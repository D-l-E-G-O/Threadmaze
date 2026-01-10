#pragma once
#include <stdbool.h>
#include "utils.h"
#include "path.h"
#include "cell.h"

#define WALL    REGULAR GRAY "██" RESET
#define SPACE   "  "
#define CELL    "  "
#define EXIT    BOLD RED "██" RESET
#define PLAYER  BOLD GREEN "██" RESET
#define HINT    BOLD BLUE "**" RESET
#define ENEMY   BOLD PURPLE "██" RESET

typedef struct {
    Cell **cells;
    int width;
    int height;
} Maze;

/**
 * Allocates memory for the maze grid.
 * @param maze Pointer to the maze structure.
 * @param width Width of the maze.
 * @param height Height of the maze.
 */
void maze_init(Maze *maze, int width, int height);

/**
 * Frees the memory allocated for the maze grid.
 * @param maze Pointer to the maze structure.
 */
void maze_free(Maze *maze);

/**
 * Resets all cells in the maze to their default state (walls/unvisited).
 * @param maze Pointer to the maze structure.
 */
void maze_fill(Maze *maze);

/**
 * Checks if a move from (old_x, old_y) to (new_x, new_y) is valid (no walls).
 * @param maze Pointer to the maze structure (read-only).
 * @param old_x Current X coordinate.
 * @param old_y Current Y coordinate.
 * @param new_x Target X coordinate.
 * @param new_y Target Y coordinate.
 * @return true if the move is valid, false otherwise.
 */
bool is_cell_valid(const Maze *maze, int old_x, int old_y, int new_x, int new_y);

/**
 * Moves a cell content from one position to another (logically).
 * Used to update the player's position symbol in the grid.
 * @param maze Pointer to the maze structure.
 * @param old_x Current X coordinate.
 * @param old_y Current Y coordinate.
 * @param new_x Target X coordinate.
 * @param new_y Target Y coordinate.
 * @return true if the move was successful.
 */
bool move_cell(Maze *maze, int old_x, int old_y, int new_x, int new_y);

/**
 * Carves a passage (removes walls) between two adjacent cells.
 * @param maze Pointer to the maze structure.
 * @param r1 Row of the first cell.
 * @param c1 Column of the first cell.
 * @param r2 Row of the second cell.
 * @param c2 Column of the second cell.
 */
void carve_passage(Maze *maze, int r1, int c1, int r2, int c2);

/**
 * Generates a perfect maze using Wilson's algorithm.
 * @param maze Pointer to the initialized maze structure.
 */
void generate_maze_wilson(Maze *maze);

/**
 * Mutates the maze by swapping a wall with a passage while maintaining connectivity.
 * This ensures the maze remains "perfect" (no loops, no isolated areas).
 * @param maze Pointer to the maze structure.
 */
void maze_mutate(Maze *maze);

/**
 * Removes internal walls randomly to create cycles (loops) in the maze.
 * This makes the maze "imperfect" and playable with enemies.
 * @param maze Pointer to the maze structure.
 * @param probability Percentage chance (0-100) to remove a wall.
 */
void maze_braid(Maze *maze, int probability);
