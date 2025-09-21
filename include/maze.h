#pragma once
#include <stdbool.h>

#define WALL "██"
#define SPACE "  "
#define CELL "  "
#define EXIT "><"
#define PLAYER "()"

typedef struct {
    bool up;
    bool down;
    bool left;
    bool right;
    char* symbol;
} Cell;

typedef struct {
    int row;
    int col;
} CellCoord;

typedef struct {
    CellCoord* cells;
    int size;
    int capacity;
    bool** in_path;
    int width;
    int height;
} Path;

typedef struct {
    Cell** cells;
    int width;
    int height;
} Maze;

void init_maze(Maze* maze, int width, int height);
void free_maze(Maze* maze);
void fill_maze(Maze* maze);
void print_maze(const Maze* maze);
bool is_cell_valid(Maze maze, int old_x, int old_y, int new_x, int new_y);
bool move_cell(Maze* maze, int old_x, int old_y, int new_x, int new_y);

bool** init_bool_matrix(int width, int height);
void free_bool_matrix(bool** m, int height);
void carve_passage(Maze* maze, int r1, int c1, int r2, int c2);
void generate_maze_wilson(Maze* maze);
void init_path(Path* path, int width, int height, int initial_capacity);
void free_path(Path* path);
int push_path(Path* path, int row, int col);
bool all_cells_visited(bool** visited, int width, int height);
void truncate_path(Path* path, int index);
