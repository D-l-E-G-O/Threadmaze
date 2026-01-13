#pragma once
#include "config.h"

/**
 * Sub-menu to change a specific key binding.
 * @param key_to_change Pointer to the key variable to modify.
 * @param name The display name of the action (e.g., "UP").
 */
void change_key(char *key_to_change, const char *name);

/**
 * Displays the options menu to configure game settings.
 * @param config Pointer to the game configuration structure.
 */
void show_options_menu(GameConfig *config);

/**
 * Displays the main menu and handles user interaction (Play, Options, Quit).
 * @param config Pointer to the game configuration structure.
 * @return true if the player chose to start the game, false if they chose to quit.
 */
bool run_main_menu(GameConfig *config);

/**
 * Displays the end-of-game menu allowing the player to replay, go back, or quit.
 * @return 1 for Replay, 2 for Main Menu, 3 for Quit.
 */
int show_end_game_menu(void);