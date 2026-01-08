#pragma once
#include "maze.h"
#include "player.h"
#include "timer.h"
#include "config.h"

/**
 * Starts the main game loop.
 * Initializes the maze, player, and timers based on the provided configuration.
 * @param config Pointer to the game configuration.
 */
void game_start(GameConfig *config);