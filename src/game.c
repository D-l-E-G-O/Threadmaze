#include <signal.h>
#include <stdlib.h>
#include "game.h"
#include "render.h"
#include "input.h"

static volatile sig_atomic_t tick = 0;

static void on_tick(int signal) {
    (void)signal;
    tick = 1;
}

void game_start(int width, int height, int time_limit) {
    struct sigaction act;

    Maze maze;
    Player player;

    init_maze(&maze, width, height);
    generate_maze_wilson(&maze);
    init_player(&player, &maze);
    init_input();

    Timer timer = (Timer){0};
    if (time_limit > 0) {
        act.sa_handler = on_tick;
        sigaction(SIGUSR1, &act, NULL);
        init_timer(&timer, time_limit, getpid(), SIGUSR1);
        start_timer(&timer);
    }

    print_game(&maze, get_time_left(timer));

    while (game_should_continue(player, maze, timer, time_limit)) {
        char input = get_input_non_blocking();
        if (input) {
            move_player(&player, input, &maze);
            print_game(&maze, get_time_left(timer));
        }
        if (tick) {
            tick = 0;
            print_game(&maze, get_time_left(timer));
        }
    }
    stop_timer(&timer);
    print_game(&maze, get_time_left(timer));
    print_game_result(is_exit_reached(player, maze), time_limit, get_time_left(timer));
    free_maze(&maze);
    restore_input();
}

bool game_should_continue(Player player, Maze maze, Timer timer, int time_limit){
    if (is_exit_reached(player, maze)) return false;
    if (time_limit > 0 && get_time_left(timer) <= 0) return false;
    return true;
}