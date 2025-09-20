#pragma once
#include <stdbool.h>

#define WALL '█'
#define EMPTY ' '
#define EXIT 'X'
#define PLAYER 'O'

typedef struct {
    char** grid;
    int width;
    int height;
} Maze;

void init_maze(Maze* maze, int width, int height);
void free_maze(Maze* maze);
void print_maze(Maze maze);
bool is_cell_valid(Maze maze, int x, int y);
void set_cell(Maze* maze, int x, int y, char value);