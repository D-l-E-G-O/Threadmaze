#pragma once
#include <stdbool.h>
#include "cell.h"
#include "heap.h"
#include "maze.h"

typedef struct {
    CellCoord* cells;
    int size;
    int capacity;
} AStarPath;

typedef int (*HeuristicFunc)(int row1, int col1, int row2, int col2);

bool astar_solve(Maze* maze, int start_row, int start_col, int end_row, int end_col, AStarPath* path_out, HeuristicFunc heuristic);
void init_astar_path(AStarPath* path, int capacity);
void astar_path_resize(AStarPath* path);
void astar_cleanup(bool** visited, HeapNode* came_from, HeapNode* heap_data, int height);
int astar_manhattan(int row1, int col1, int row2, int col2);