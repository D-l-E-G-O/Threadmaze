#pragma once
#include <stdbool.h>

#define MAX_MAZE_WIDTH get_terminal_width()/4 - 1
#define MIN_MAZE_WIDTH 1

#define MAX_MAZE_HEIGHT get_terminal_height()/2 - 2
#define MIN_MAZE_HEIGHT 1

#define MAX_TIME_LIMIT 3599
#define MIN_TIME_LIMIT 0

#define MAX_HINT_DURATION 3599
#define MIN_HINT_DURATION 1

typedef struct GameConfig {
    // --- Maze Settings ---
    int width;
    int height;
    
    // --- Gameplay Settings ---
    int time_limit;         // 0 = disabled
    int hint_duration;      // Duration of the hint in seconds

    // --- Controls (Keybindings) ---
    char key_up;
    char key_down;
    char key_left;
    char key_right;
    char key_hint;
} GameConfig;

/**
 * Sets the configuration with default values.
 */
void config_set_defaults(GameConfig* config);

/**
 * Loads configuration or loads the default values if the config file doesn't exist.
 * Overwrites values in the config if they are found in the config file.
 */
void config_load(GameConfig* config);

/**
 * Saves the current config in the config file.
 */
void config_save(const GameConfig* config);

/**
 * Clamps the config values to assert safety.
 */
void clamp_config_values(GameConfig *config);
