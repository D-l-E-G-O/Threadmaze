#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "utils.h"

void change_key(char *key_to_change, const char *name) {
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

void show_options_menu(GameConfig *config) {
    bool in_options = true;
    while (in_options) {
        system("clear");
        printf(BOLD "=== OPTIONS ===\n" RESET);
        printf("1. Maze width           [%d]    (0 = AUTO)  (Between %d and %d)\n", config->width, MIN_MAZE_WIDTH, MAX_MAZE_WIDTH);
        printf("2. Maze height          [%d]    (0 = AUTO)  (Between %d and %d)\n", config->height, MIN_MAZE_HEIGHT, MAX_MAZE_HEIGHT);
        printf("3. Time limit           [%d s]  (0 = OFF)   (Between %d and %d)\n", config->time_limit, MIN_TIME_LIMIT, MAX_TIME_LIMIT);
        printf("4. Hint duration        [%d s]  (0 = OFF)   (Between %d and %d)\n", config->hint_duration, MIN_HINT_DURATION, MAX_HINT_DURATION);
        printf("5. Mutation frequency   [%d s]  (0 = OFF)   (Between %d and %d)\n", config->mutation_interval, MIN_MUTATION_INTERVAL, MAX_MUTATION_INTERVAL);
        printf("----------------\n");
        printf("6. Up    [%c]\n", config->key_up);
        printf("7. Down  [%c]\n", config->key_down);
        printf("8. Left  [%c]\n", config->key_left);
        printf("9. Right [%c]\n", config->key_right);
        printf("----------------\n");
        printf("10. SAVE AND GO BACK\n");
        printf("0. CANCEL\n");
        printf("Your choice : ");

        int choice = read_int();
        char buf[20];

        switch(choice) {
            case 1: 
                printf("New width : "); 
                if(fgets(buf, 20, stdin)) config->width = atoi(buf); 
                break;
            case 2: 
                printf("New height : "); 
                if(fgets(buf, 20, stdin)) config->height = atoi(buf); 
                break;
            case 3: 
                printf("New time limit : "); 
                if(fgets(buf, 20, stdin)) config->time_limit = atoi(buf); 
                break;
            case 4: 
                printf("New hint duration : "); 
                if(fgets(buf, 20, stdin)) config->hint_duration = atoi(buf); 
                break;
            case 5:
                printf("New mutation frequency : "); 
                if(fgets(buf, 20, stdin)) config->mutation_interval = atoi(buf); 
                break;
            case 6: change_key(&config->key_up, "UP"); break;
            case 7: change_key(&config->key_down, "DOWN"); break;
            case 8: change_key(&config->key_left, "LEFT"); break;
            case 9: change_key(&config->key_right, "RIGHT"); break;
            case 10:
                config_save(config);
                in_options = false;
                break;
            case 0:
                config_load(config);
                in_options = false;
                break;
        }

        // Security clamp
        config_clamp_values(config);
    }
}

bool run_main_menu(GameConfig *config) {
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