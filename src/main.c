#include <stdlib.h>
#include "game.h"
#include "timer.h"


int main(int argc, char* argv[]) {
    int width = 5, height = 5, time_limit = 0;

    if (argc >= 3) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
    }
    if (argc == 4) {
        time_limit = atoi(argv[3]);
    }

    game_start(width, height, time_limit);
    return EXIT_SUCCESS;
}


