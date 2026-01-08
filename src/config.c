#include "config.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONFIG_FILE "settings.cfg"

void config_set_defaults(GameConfig *config) {
    // Default Settings
    config->width = MAX_MAZE_WIDTH;
    config->height = MAX_MAZE_HEIGHT;
    config->time_limit = 0;
    config->hint_duration = 2;
    config->mutation_interval = 5;

    // Default Controls (WASD/ZQSD friendly defaults, or specific)
    config->key_up    = 'z';
    config->key_down  = 's';
    config->key_left  = 'q';
    config->key_right = 'd';
    config->key_hint  = 'h';
}

void config_save(const GameConfig *config) {
    FILE* f = fopen(CONFIG_FILE, "w");
    if (!f) return;
    
    fprintf(f, "WIDTH=%d\n", config->width);
    fprintf(f, "HEIGHT=%d\n", config->height);
    fprintf(f, "TIME=%d\n", config->time_limit);
    fprintf(f, "HINT=%d\n", config->hint_duration);
    fprintf(f, "MUTATION=%d\n", config->mutation_interval);
    fprintf(f, "KEY_UP=%c\n", config->key_up);
    fprintf(f, "KEY_DOWN=%c\n", config->key_down);
    fprintf(f, "KEY_LEFT=%c\n", config->key_left);
    fprintf(f, "KEY_RIGHT=%c\n", config->key_right);
    fprintf(f, "KEY_HINT=%c\n", config->key_hint);
    
    fclose(f);
}

void config_load(GameConfig *config) {
    config_set_defaults(config); // We start with the default values
    
    FILE* f = fopen(CONFIG_FILE, "r");
    if (!f) return; // If there is no file we start with the default values

    char line[100];
    char key_str[50];
    char val_str[50];
    int val_int;
    char val_char;

    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%[^=]=%s", key_str, val_str) == 2) {
            val_int = atoi(val_str);
            val_char = val_str[0];

            if (strcmp(key_str, "WIDTH") == 0) config->width = val_int;
            else if (strcmp(key_str, "HEIGHT") == 0) config->height = val_int;
            else if (strcmp(key_str, "TIME") == 0) config->time_limit = val_int;
            else if (strcmp(key_str, "HINT") == 0) config->hint_duration = val_int;
            else if (strcmp(key_str, "MUTATION") == 0) config->mutation_interval = val_int;
            else if (strcmp(key_str, "KEY_UP") == 0) config->key_up = val_char;
            else if (strcmp(key_str, "KEY_DOWN") == 0) config->key_down = val_char;
            else if (strcmp(key_str, "KEY_LEFT") == 0) config->key_left = val_char;
            else if (strcmp(key_str, "KEY_RIGHT") == 0) config->key_right = val_char;
            else if (strcmp(key_str, "KEY_HINT") == 0) config->key_hint = val_char;
        }
    }
    fclose(f);
}

void config_clamp_values(GameConfig *config) {
    int max_width = MAX_MAZE_WIDTH;
    int max_height = MAX_MAZE_HEIGHT;
    if (config->width < MIN_MAZE_WIDTH || config->width > max_width) config->width = max_width;
    if (config->height < MIN_MAZE_HEIGHT || config->height > max_height) config->height = max_height;
    config->time_limit = clamp_int(config->time_limit, MIN_TIME_LIMIT, MAX_TIME_LIMIT);
    config->hint_duration = clamp_int(config->hint_duration, MIN_HINT_DURATION, MAX_HINT_DURATION);
    config->mutation_interval = clamp_int(config->mutation_interval, MIN_MUTATION_INTERVAL, MAX_MUTATION_INTERVAL);
}