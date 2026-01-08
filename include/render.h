#pragma once
#include "maze.h"

/**
 * Clears the screen and prints the current game state (timer + maze).
 * @param maze Pointer to the maze structure (read-only).
 * @param time_left Time remaining in seconds (negative if disabled).
 */
void print_game(const Maze *maze, int time_left);

/**
 * Prints the end-game message (Victory or Defeat).
 * @param victory Boolean indicating if the player won.
 * @param initial_time The initial time limit set for the game.
 * @param time_left The time remaining when the game ended.
 */
void print_game_result(bool victory, int initial_time, int time_left);