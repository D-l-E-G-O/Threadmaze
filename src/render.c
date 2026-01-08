#include <stdio.h>
#include <stdlib.h>
#include "render.h"
#include "utils.h"

void print_game(const Maze *maze, int time_left) {
    system("clear");
    if (time_left >= 0) {
        char *bold = time_left % 2 == 0 ? BOLD : "";
        char *color = time_left < 10 ? RED : BLUE;
        printf("Time left:%s%s %02d:%02d\n",bold, color, time_left / 60, time_left % 60);
        printf(RESET);
    }
    maze_display(maze);
}

void print_game_result(bool victory, int initial_time, int time_left) {
    if (victory) {
        printf(BOLDGREEN);
        if (initial_time > 0)
            printf("You won in %d seconds!\n", initial_time - time_left);
        else
            printf("You won!\n");
    } else {
        printf(BOLDRED);
        printf("You lost!\n");
    }
    printf(RESET);
}