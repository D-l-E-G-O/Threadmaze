#pragma once
#include "maze.h"
#include "player.h"
#include "timer.h"

void game_start(int width, int height, int time_limit);
bool game_should_continue(Player player, Maze maze, Timer timer, int time_limit);