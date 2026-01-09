#pragma once
#include "context.h"

/**
 * Renders the full game state.
 * @param ctx Pointer to the game context (read-only).
 */
void print_game(const GameContext *ctx);

/**
 * Prints the end-game message (Victory or Defeat).
 * @param victory Boolean indicating if the player won.
 * @param initial_time The initial time limit set for the game.
 * @param time_left The time remaining when the game ended.
 */
void print_game_result(bool victory, int initial_time, int time_left);