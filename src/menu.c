#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "utils.h"

/**
 * Sub-menu to change a key.
 */
void change_key(char* key_to_change, const char* name) {
    printf("Press the new key for %s : ", name);
    char c = read_char();
    if (c >= 33 && c <= 126) { // Printable char
        *key_to_change = c;
        printf("Key modified !\n");
    } else {
        printf("Invalid key.\n");
    }

    printf("Press enter to continue...");
    read_char(); 
}

/**
 * Prints the options menu and allows the player the change the settings.
 */
void show_options_menu(GameConfig* config) {
    bool in_options = true;
    while (in_options) {
        system("clear");
        printf(BOLD "=== OPTIONS ===\n" RESET);
        printf("1. Maze width    [%d]       (Between %d and %d)\n", config->width, MIN_MAZE_WIDTH, MAX_MAZE_WIDTH);
        printf("2. Maze height   [%d]       (Between %d and %d)\n", config->height, MIN_MAZE_HEIGHT, MAX_MAZE_HEIGHT);
        printf("3. Time limit    [%d s]     (Between %d and %d)\n", config->time_limit, MIN_TIME_LIMIT, MAX_TIME_LIMIT);
        printf("4. Hint duration [%d s]     (Between %d and %d)\n", config->hint_duration, MIN_HINT_DURATION, MAX_HINT_DURATION);
        printf("----------------\n");
        printf("5. Up    [%c]\n", config->key_up);
        printf("6. Down  [%c]\n", config->key_down);
        printf("7. Left  [%c]\n", config->key_left);
        printf("8. Right [%c]\n", config->key_right);
        printf("----------------\n");
        printf("9. SAVE AND GO BACK\n");
        printf("0. CANCEL\n");
        printf("Your choice : ");

        char choice = read_char();
        char buf[20];

        switch(choice) {
            case '1': 
                printf("New width : "); 
                if(fgets(buf, 20, stdin)) config->width = atoi(buf); 
                break;
            case '2': 
                printf("New height : "); 
                if(fgets(buf, 20, stdin)) config->height = atoi(buf); 
                break;
            case '3': 
                printf("New time limit : "); 
                if(fgets(buf, 20, stdin)) config->time_limit = atoi(buf); 
                break;
            case '4': 
                printf("New hint duration : "); 
                if(fgets(buf, 20, stdin)) config->hint_duration = atoi(buf); 
                break;
            case '5': change_key(&config->key_up, "UP"); break;
            case '6': change_key(&config->key_down, "DOWN"); break;
            case '7': change_key(&config->key_left, "LEFT"); break;
            case '8': change_key(&config->key_right, "RIGHT"); break;
            case '9':
                config_save(config);
                in_options = false;
                break;
            case '0':
                config_load(config);
                in_options = false;
                break;
        }

        // Security clamp
        clamp_config_values(config);
    }
}

/**
 * Prints the main menu.
 * Returns true if the player wants to play, false if he wants to quit.
 */
bool run_main_menu(GameConfig* config) {
    while(true) {
        system("clear");
        printf(BOLDBLUE "=== THREADMAZE ===\n" RESET);
        printf("1. Play\n");
        printf("2. Settings\n");
        printf("3. Quit\n");
        printf("\nChoice : ");
        
        char c = read_char();
        if (c == '1') return true;
        if (c == '2') show_options_menu(config);
        if (c == '3') return false;
    }
}