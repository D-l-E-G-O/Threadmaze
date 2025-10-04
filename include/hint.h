#pragma once
#include <stdbool.h>
#include "maze.h"
#include "player.h"
#include "path.h"

typedef struct {
    Path path;
    int duration;
    bool active;
} Hint;

bool init_hint(Hint* hint, Maze* maze, Player* player);
void toggle_hint(Hint* hint, int duration);
void update_hint(Hint* hint, Maze* maze, Player* player);
void free_hint(Hint* hint);
