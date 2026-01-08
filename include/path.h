#pragma once
#include <stdbool.h>
#include "cell.h"

#define NO_CYCLE -1
#define ERROR -2

typedef struct {
    CellCoord *cells;
    int size;
    int capacity;
    bool **visited_cells;
    int width;
    int height;
} Path;

/**
 * Initializes a path structure used for maze generation (random walk).
 * @param path Pointer to the path structure.
 * @param width Width of the grid.
 * @param height Height of the grid.
 * @param initial_capacity Initial allocation size for the path array.
 */
void path_init(Path *path, int width, int height, int initial_capacity);

/**
 * Frees memory associated with the path.
 * @param path Pointer to the path structure.
 */
void path_free(Path *path);

/**
 * Adds a cell to the path. Detects loops.
 * @param path Pointer to the path structure.
 * @param row Row of the cell.
 * @param col Column of the cell.
 * @return Index of the cycle start if a loop is detected, NO_CYCLE if successful, ERROR on failure.
 */
int path_push(Path *path, int row, int col);

/**
 * Checks if all cells in the grid have been visited.
 * @param visited_cells 2D boolean matrix of visited status.
 * @param width Grid width.
 * @param height Grid height.
 * @return true if all cells are visited, false otherwise.
 */
bool all_cells_visited(const bool **visited_cells, int width, int height);

/**
 * Truncates the path at a specific index (used when a loop is removed).
 * @param path Pointer to the path structure.
 * @param index The index after which cells should be removed.
 */
void path_truncate(Path *path, int index);