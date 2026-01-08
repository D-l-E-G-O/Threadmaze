#pragma once
#include <stdbool.h>
#include "cell.h"
#include "heap.h"
#include "maze.h"

typedef struct {
    CellCoord *cells;
    int size;
    int capacity;
} AStarPath;

typedef int (*HeuristicFunc)(int row1, int col1, int row2, int col2);

/**
 * Solves the maze using the A* algorithm to find the shortest path.
 * @param maze Pointer to the maze structure (read-only).
 * @param start_row Starting row coordinate.
 * @param start_col Starting column coordinate.
 * @param end_row Target row coordinate.
 * @param end_col Target column coordinate.
 * @param path_out Pointer to the structure where the resulting path will be stored.
 * @param heuristic Function pointer to the heuristic used (e.g., Manhattan distance).
 * @return true if a path was found, false otherwise.
 */
bool astar_solve(const Maze *maze, int start_row, int start_col, int end_row, int end_col, AStarPath *path_out, HeuristicFunc heuristic);

/**
 * Initializes the A* path structure.
 * @param path Pointer to the AStarPath structure.
 * @param capacity Initial memory capacity for path steps.
 */
void astar_path_init(AStarPath *path, int capacity);

/**
 * Resizes the internal array of the A* path if capacity is reached.
 * @param path Pointer to the AStarPath structure.
 */
void astar_path_resize(AStarPath *path);

/**
 * Cleans up temporary resources used during the A* search.
 * @param visited Matrix of visited nodes.
 * @param came_from Array of parent nodes.
 * @param heap_data Array used by the priority queue.
 * @param height Height of the grid (for freeing the matrix).
 */
void astar_cleanup(bool **visited, HeapNode *came_from, HeapNode *heap_data, int height);

/**
 * Calculates the Manhattan distance between two points.
 * @param row1 Row of the first point.
 * @param col1 Column of the first point.
 * @param row2 Row of the second point.
 * @param col2 Column of the second point.
 * @return The Manhattan distance.
 */
int astar_manhattan(int row1, int col1, int row2, int col2);