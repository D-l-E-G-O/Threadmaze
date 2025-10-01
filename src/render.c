#include <stdio.h>
#include <stdlib.h>
#include "render.h"

void print_game(Maze* maze, int time_left) {
    system("clear");
    if (time_left >= 0) {
        printf("Time left: %02d:%02d\n", time_left / 60, time_left % 60);
    }
    print_maze(maze);
}

void end_game(bool victory, int initial_time, int time_left) {
    printf("\n");
    if (victory) {
        if (initial_time > 0)
            printf("You won in %d seconds!\n", initial_time - time_left);
        else
            printf("You won!\n");
    } else {
        printf("You lost!\n");
    }
}