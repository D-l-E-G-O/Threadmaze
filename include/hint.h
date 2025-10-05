#pragma once
#include <stdbool.h>
#include "maze.h"
#include "player.h"
#include "astar.h"

typedef struct {
    AStarPath path;
    bool active;
    int duration;
    int time_left;
} Hint;

void init_hint(Hint* hint, int max_duration, int maze_width, int maze_height);
void generate_hint_path(Maze* maze, Player* player, AStarPath* path);
void activate_hint(Hint* hint, Maze* maze, Player* player);
void deactivate_hint(Hint* hint, Maze* maze);
void hint_tick(Hint* hint, Maze* maze);
void apply_hint(Hint* hint, Maze* maze, char* symbol);
