#pragma once
#include "maze.h"

typedef struct {
    int x;
    int y;
} Player;

void init_player(Player* player, Maze* maze);
void move_player(Player* player, char input, Maze* maze);
bool is_exit_reached(Player player, Maze maze);