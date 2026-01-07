#pragma once
#include "config.h"

/**
 * Sub-menu to change a key.
 */
void change_key(char* key_to_change, const char* name);

/**
 * Prints the options menu and allows the player the change the settings.
 */
void show_options_menu(GameConfig* config);

/**
 * Prints the main menu.
 * Returns true if the player wants to play, false if he wants to quit.
 */
bool run_main_menu(GameConfig* config);