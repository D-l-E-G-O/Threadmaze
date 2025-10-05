#include <signal.h>
#include <stdlib.h>
#include "game.h"
#include "render.h"
#include "input.h"
#include "hint.h"

static volatile sig_atomic_t tick = 0;
static volatile sig_atomic_t hint_tick = 0;

static void on_maze_timer_tick(int signal) {
    (void)signal;
    tick = 1;
}

static void on_hint_timer_tick(int signal) {
    (void)signal;
    hint_tick = 1;
}

void game_start(int width, int height, int time_limit) {
    struct sigaction maze_timer_act, hint_timer_act;

    sigemptyset(&hint_timer_act.sa_mask);
    hint_timer_act.sa_flags = 0;
    hint_timer_act.sa_handler = on_hint_timer_tick;
    sigaction(SIGUSR2, &hint_timer_act, NULL);

    Maze maze;
    Player player;
    Hint hint;
    Timer hint_timer = (Timer){0};

    init_maze(&maze, width, height);
    generate_maze_wilson(&maze);
    init_hint(&hint, &hint_timer, HINT_DURATION, getpid(), SIGUSR2);
    init_player(&player, &maze);
    init_input();

    Timer maze_timer = (Timer){0};
    if (time_limit > 0) {
        sigemptyset(&maze_timer_act.sa_mask);
        maze_timer_act.sa_flags = 0;
        maze_timer_act.sa_handler = on_maze_timer_tick;
        sigaction(SIGUSR1, &maze_timer_act, NULL);
        init_timer(&maze_timer, time_limit, getpid(), SIGUSR1);
        start_timer(&maze_timer);
    }

    print_game(&maze, get_time_left(maze_timer));

    while (game_should_continue(player, maze, maze_timer, time_limit)) {
        char input = get_input_non_blocking();
        bool refresh = false;
        if (input) {
            if (input == 'H' || input == 'h') {
                if (hint.active) {
                    deactivate_hint(&hint, &maze, true);
                } else {
                    activate_hint(&hint, &maze, &player, true);
                }
            } else {
                if (hint.active) {
                    deactivate_hint(&hint, &maze, false);
                    move_player(&player, input, &maze);
                    activate_hint(&hint, &maze, &player, false);
                } else {
                    move_player(&player, input, &maze);
                }
            }
            refresh = true;
        }
        if (tick) {
            tick = 0;
            refresh = true;
        }
        if (hint_tick && get_time_left(hint_timer) == 0) {
            hint_tick = 0;
            deactivate_hint(&hint, &maze, true);
            refresh = true;
        }
        if (refresh) {
            print_game(&maze, get_time_left(maze_timer));
        }
    }
    stop_timer(&maze_timer);
    print_game(&maze, get_time_left(maze_timer));
    print_game_result(is_exit_reached(player, maze), time_limit, get_time_left(maze_timer));
    if (hint.timer->active) {
        stop_timer(hint.timer);
    }
    deactivate_hint(&hint, &maze, true);
    free_hint(&hint);
    free_maze(&maze);
    restore_input();
}

bool game_should_continue(Player player, Maze maze, Timer timer, int time_limit){
    if (is_exit_reached(player, maze)) return false;
    if (time_limit > 0 && get_time_left(timer) <= 0) return false;
    return true;
}