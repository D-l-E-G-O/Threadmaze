#pragma once
#include "maze.h"

void print_game(Maze* maze, int time_left);
void end_game(bool victory, int initial_time, int time_left);