#pragma once
#include <stdbool.h>

#define MAX_MAZE_WIDTH get_terminal_width()/4 - 1
#define MIN_MAZE_WIDTH 1

#define MAX_MAZE_HEIGHT get_terminal_height()/2 - 2
#define MIN_MAZE_HEIGHT 1

#define MAX_TIME_LIMIT 300
#define MIN_TIME_LIMIT 0

#define MAX_HINT_DURATION 60
#define MIN_HINT_DURATION 0

#define MAX_MUTATION_INTERVAL 60
#define MIN_MUTATION_INTERVAL 0

#define MAX_MUTATION_AMOUNT 20
#define MIN_MUTATION_AMOUNT 1

typedef struct GameConfig {
    // --- Maze Settings ---
    int width;
    int height;
    
    // --- Gameplay Settings ---
    int time_limit;         // 0 = disabled
    int hint_duration;      // Duration of the hint in seconds
    int mutation_interval;  // Time in seconds between mutations (0 = disabled)
    int mutation_amount;    // Number of walls to change during one mutation

    // --- Controls (Keybindings) ---
    char key_up;
    char key_down;
    char key_left;
    char key_right;
    char key_hint;
} GameConfig;

/**
 * Sets default values for the game configuration.
 * @param config Pointer to the configuration structure.
 */
void config_set_defaults(GameConfig *config);

/**
 * Loads the game configuration from a file.
 * Applies default values if the file cannot be read.
 * @param config Pointer to the configuration structure to populate.
 * @param filename The path to the configuration file.
 */
void config_load(GameConfig *config);

/**
 * Saves the current game configuration to a file.
 * @param config Pointer to the configuration structure (read-only).
 * @param filename The path to the configuration file.
 */
void config_save(const GameConfig *config);

/**
 * Ensures all configuration values are within valid ranges.
 * @param config Pointer to the configuration structure.
 */
void config_clamp_values(GameConfig *config);
