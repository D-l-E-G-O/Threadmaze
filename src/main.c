#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include "game.h"
#include "timer.h"
#include "utils.h"
#include "config.h"
#include "menu.h"

int main(int argc, char *argv[]) {
    GameConfig config;
    config_load(&config);

    int opt;
    while ((opt = getopt(argc, argv, "w:h:t:")) != -1) {
        switch (opt) {
            case 'w':
                config.width = atoi(optarg);
                break;
            case 'h':
                config.height = atoi(optarg);
                break;
            case 't':
                config.time_limit = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s [-w width] [-h height] [-t seconds]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Size security check
    if (MAX_MAZE_WIDTH <= 0 || MAX_MAZE_HEIGHT <= 0) {
        printf("Your terminal's size is insufficient to display the labyrinth.\n");
        return EXIT_SUCCESS;
    }

    // Display the menu
    if (run_main_menu(&config)) {
        // Start the game with the config
        game_start(&config); 
    }
    return EXIT_SUCCESS;
}


