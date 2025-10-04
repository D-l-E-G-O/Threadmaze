#include <stdlib.h>
#include "game.h"
#include "timer.h"
#include "utils.h"
#include <stdio.h>

#define MAX_WIDTH get_terminal_width()/4 - 1
#define MAX_HEIGHT get_terminal_height()/2 - 2
#define MAX_TIME_LIMIT 3599

int main(int argc, char* argv[]) {
    int width = MAX_WIDTH;
    int height = MAX_HEIGHT;
    int time_limit = -1;

    int opt;
    while ((opt = getopt(argc, argv, "w:h:t:")) != -1) {
        switch (opt) {
            case 'w':
                width = clamp_int(atoi(optarg), 1, width);
                break;
            case 'h':
                height = clamp_int(atoi(optarg), 1, height);
                break;
            case 't':
                time_limit = clamp_int(atoi(optarg), 0, MAX_TIME_LIMIT);
                break;
            default:
                fprintf(stderr, "Usage: %s [-w width] [-h height] [-t seconds]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    game_start(width, height, time_limit);
    return EXIT_SUCCESS;
}


