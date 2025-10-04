#include <stdlib.h>
#include "game.h"
#include "timer.h"
#include "utils.h"

#define MAX_WIDTH 999
#define MAX_HEIGHT 999
#define MAX_TIME_LIMIT 3599

int main(int argc, char* argv[]) {
    int width = 5, height = 5, time_limit = -1;

    if (argc >= 3) {
        width = clamp_int(atoi(argv[1]), 1, MAX_WIDTH);
        height = clamp_int(atoi(argv[2]), 1, MAX_HEIGHT);
    }
    if (argc == 4) {
        time_limit = clamp_int(atoi(argv[3]), 0, MAX_TIME_LIMIT);
    }

    game_start(width, height, time_limit);
    return EXIT_SUCCESS;
}


