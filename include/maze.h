#pragma once
#include <stdbool.h>
#include "utils.h"
#include "path.h"
#include "cell.h"

#define WALL GRAY "██" RESET
#define SPACE "  "
#define CELL "  "
#define EXIT BOLDRED "██" RESET
#define PLAYER BOLDGREEN "██" RESET
#define HINT BOLDBLUE "**" RESET

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
