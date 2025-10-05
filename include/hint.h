#pragma once
#include <stdbool.h>
#include "maze.h"
#include "player.h"
#include "astar.h"
#include "timer.h"

#define HINT_DURATION 2

typedef struct {
    AStarPath path;
    bool active;
    int duration;
    Timer* timer;
} Hint;

void init_hint(Hint* hint, Timer* timer, int max_duration, pid_t pid_caller, int callback_signal);
void free_hint(Hint* hint);
void generate_hint_path(Maze* maze, Player* player, AStarPath* path);
void activate_hint(Hint* hint, Maze* maze, Player* player, bool restart_timer);
void deactivate_hint(Hint* hint, Maze* maze, bool end_timer);
void apply_hint(Hint* hint, Maze* maze, char* symbol);
