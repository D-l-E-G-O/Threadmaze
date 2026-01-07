#pragma once
#include "maze.h"
#include "player.h"
#include "timer.h"
#include "config.h"

void game_start(GameConfig *config);
bool game_should_continue(Player player, Maze maze, Timer timer, int time_limit);