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
    while (in_options && !stop_requested) {
        printf(CLEAR); // ANSI escape code equivalent to system("clear");
        printf(BOLD GREEN "=== OPTIONS ===\n" RESET);
        printf("1. Maze width           [%d]    (0 = AUTO)  (Between %d and %d)\n", config->width, MIN_MAZE_WIDTH, MAX_MAZE_WIDTH);
        printf("2. Maze height          [%d]    (0 = AUTO)  (Between %d and %d)\n", config->height, MIN_MAZE_HEIGHT, MAX_MAZE_HEIGHT);
        printf("3. Time limit           [%d s]  (0 = OFF)   (Between %d and %d)\n", config->time_limit, MIN_TIME_LIMIT, MAX_TIME_LIMIT);
        printf("4. Hint duration        [%d s]  (0 = OFF)   (Between %d and %d)\n", config->hint_duration, MIN_HINT_DURATION, MAX_HINT_DURATION);
        printf("5. Mutation frequency   [%d s]  (0 = OFF)   (Between %d and %d)\n", config->mutation_interval, MIN_MUTATION_INTERVAL, MAX_MUTATION_INTERVAL);
        printf("6. Mutation amount      [%d walls]          (Between %d and %d)\n", config->mutation_amount, MIN_MUTATION_AMOUNT, MAX_MUTATION_AMOUNT);
        printf("7. Braid percentage     [%d %c]              (Between %d and %d)\n", config->braid_probability, '%', MIN_BRAID_PROBABILITY, MAX_BRAID_PROBABILITY);
        printf("-------------------------------------------------------------------------\n");
        printf("8. Enemy count              [%d]            (Between %d and %d)\n", config->enemy_count, MIN_ENEMY_COUNT, MAX_ENEMY_COUNT);
        printf("9. Enemy movement delay     [%d ms]         (Between %d and %d)\n", config->enemy_speed_ms, MIN_ENEMY_SPEED, MAX_ENEMY_SPEED);
        printf("10. Enemy detection range   [%d cells]      (Between %d and %d)\n", config->enemy_detection_range, MIN_ENEMY_DETECTION_RANGE, MAX_ENEMY_DETECTION_RANGE);
        printf("11. Enemy patrol range      [%d cells]      (Between %d and %d)\n", config->enemy_patrol_range, MIN_ENEMY_PATROL_RANGE, MAX_ENEMY_PATROL_RANGE);
        printf("-------------------------------------------------------------------------\n");
        printf("12. Up    [%c]\n", config->key_up);
        printf("13. Down  [%c]\n", config->key_down);
        printf("14. Left  [%c]\n", config->key_left);
        printf("15. Right [%c]\n", config->key_right);
        printf("-------------------------------------------------------------------------\n");
        printf("16. SAVE AND GO BACK\n");
        printf("0. CANCEL\n");
        printf("Your choice : ");

        int choice = read_int();
        // If CTRL+C was pressed during read_int/fgets :
        if (stop_requested) break;

        switch(choice) {
            case 1: 
                printf("New width : "); 
                config->width = read_int();
                break;
            case 2: 
                printf("New height : "); 
                config->height = read_int();
                break;
            case 3: 
                printf("New time limit : "); 
                config->time_limit = read_int();
                break;
            case 4: 
                printf("New hint duration : "); 
                config->hint_duration = read_int();
                break;
            case 5:
                printf("New mutation frequency : "); 
                config->mutation_interval = read_int();
                break;
            case 6:
                printf("New mutation amount : "); 
                config->mutation_amount = read_int();
                break;
            case 7:
                printf("New braid percentage : "); 
                config->braid_probability = read_int();
                break;
            case 8:
                printf("New enemy count : "); 
                config->enemy_count = read_int();
                break;
            case 9:
                printf("New enemy movement delay : "); 
                config->enemy_speed_ms = read_int();
                break;
            case 10: 
                printf("Detection range (1-50): "); 
                config->enemy_detection_range = read_int();
                break;
            case 11: 
                printf("Patrol radius (0-20): "); 
                config->enemy_patrol_range = read_int();
                break;
            case 12: change_key(&config->key_up, "UP"); break;
            case 13: change_key(&config->key_down, "DOWN"); break;
            case 14: change_key(&config->key_left, "LEFT"); break;
            case 15: change_key(&config->key_right, "RIGHT"); break;
            case 16:
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
        if (stop_requested) return false;

        printf(CLEAR); // ANSI escape code equivalent to system("clear");
        printf(BOLD BLUE "=== THREADMAZE ===\n" RESET);
        printf("1. Play\n");
        printf("2. Settings\n");
        printf("3. Quit\n");
        printf("\nChoice : ");
        
        char c = read_char();

        if (stop_requested) return false;

        if (c == '1') return true;
        if (c == '2') show_options_menu(config);
        if (c == '3') return false;
    }
}