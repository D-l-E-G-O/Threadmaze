#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "maze.h"
#include "player.h"
#include "enemy.h"
#include "timer.h"
#include "utils.h"

void game_loop(Maze* maze, Player* player);
void end_game(bool victory, int initial_time);
void print_game(Maze* maze);
static void *_clock(void* args);

struct argsTimer{
    int* time_left;
    Maze* maze;
};

int time_left;

int main(int argc, char* argv[]) {
    int width = 5;
    int height = 5;
    int initial_time;

    if (argc >= 3) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
    }
    if (argc == 4) {
        time_left = atoi(argv[3]);
        initial_time = time_left;
    }

    srand(time(NULL));
    init_input();

    Maze* maze = &((Maze){0});
    Player* player = &((Player){0});

    init_maze(maze, width, height);
    generate_maze_wilson(maze);
    init_player(player, maze);
    
    pthread_t timer;
    if (argc == 4) {
        struct argsTimer args;
        args.time_left = &time_left;
        args.maze = maze;
        if (pthread_create(&timer, NULL, _clock, (void*) &args)) {
            perror("pthread_create");
            return EXIT_FAILURE;
        }
    }

    system("clear");
    print_game(maze);

    while (!is_exit_reached(*player, *maze) && time_left) {
        game_loop(maze, player);
    }

    if (argc == 4) {
        pthread_cancel(timer);
    }
    system("clear");
    print_game(maze);

    end_game(time_left > 0, initial_time);

    free_maze(maze);
    restore_input();
    return EXIT_SUCCESS;
}

void game_loop(Maze* maze, Player* player) {
    char input = get_input_non_blocking();
    if (!input) {
        return;
    }
    move_player(player, input, maze);
    print_game(maze);
}

void end_game(bool victory, int initial_time) {
    if (victory) {
        printf("You won in %d seconds !\n", initial_time - time_left);
    }
    else {
        printf("You lost !\n");
    }
    return;
}

void print_game(Maze* maze) {
    system("clear");
    int minutes = time_left / 60;
    int seconds = time_left % 60;
    printf("Time left: %02d:%02d\n", minutes, seconds);
    print_maze(maze);
}

static void *_clock(void* args){
    int* time_left = ((struct argsTimer*)args)->time_left;
    Maze* maze = ((struct argsTimer*)args)->maze;
    (*time_left)++;
    while((*time_left)--){
        print_game(maze);
        sleep(1);
    }
    return NULL;
}