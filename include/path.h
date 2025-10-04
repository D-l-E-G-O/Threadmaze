#pragma once
#include <stdbool.h>
#include "cell.h"

#define NO_CYCLE -1
#define ERROR -2

typedef struct {
    CellCoord* cells;
    int size;
    int capacity;
    bool** visited_cells;
    int width;
    int height;
} Path;

void init_path(Path* path, int width, int height, int initial_capacity);
void free_path(Path* path);
int push_path(Path* path, int row, int col);
bool all_cells_visited(bool** visited_cells, int width, int height);
void truncate_path(Path* path, int index);