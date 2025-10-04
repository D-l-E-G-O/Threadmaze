#pragma once
#include <stdbool.h>
#include "maze.h"

typedef struct {
    int duration;
    bool active;
    Maze* maze;
    CellCoord* cells;
} Hint;

void init_hint(Hint* hint, Maze* maze, int duration);
void show_hint(Hint* hint);
void hide_hint(Hint* hint);
void free_hint(Hint* hint);
