#include "hint.h"

void init_hint(Hint* hint, Timer* timer, int max_duration, pid_t pid_caller, int callback_signal) {
    if (!hint) return;
    hint->active = false;
    hint->duration = max_duration;
    hint->timer = timer;
    hint->timer->initialized = false;
    init_timer(timer, max_duration, pid_caller, callback_signal);
}

void free_hint(Hint* hint) {
    if (!hint) return;
    if (hint->path.cells) {
        free(hint->path.cells);
        hint->path.cells = NULL;
    }
    hint->path.size = 0;
    hint->path.capacity = 0;
}

void generate_hint_path(Maze* maze, Player* player, AStarPath* path) {
    if (!maze || !player || !path) return;

    int start_row = player->y;
    int start_col = player->x;
    int end_row = maze->height - 1;
    int end_col = maze->width - 1;

    if (path->cells) {
        free(path->cells);
        path->cells = NULL;
    }

    init_astar_path(path, maze->width * maze->height);

    if (!astar_solve(maze, start_row, start_col, end_row, end_col, path, astar_manhattan)) {
        path->size = 0;
        perror("astar_solve");
    }
}

void activate_hint(Hint* hint, Maze* maze, Player* player, bool restart_timer) {
    if (!hint || !maze || !player) return;

    generate_hint_path(maze, player, &hint->path);
    hint->active = true;

    apply_hint(hint, maze, HINT);
    if (restart_timer) {
        if (hint->timer->active) {
            stop_timer(hint->timer);
        }
        init_timer(hint->timer, hint->duration, hint->timer->pid_caller, hint->timer->callback_signal);
        start_timer(hint->timer);
    }
}

void deactivate_hint(Hint* hint, Maze* maze, bool end_timer) {
    if (!hint || !maze) return;
    if (!hint->active) return;

    apply_hint(hint, maze, CELL);

    hint->active = false;
    hint->path.size = 0;
    if (end_timer) {
        stop_timer(hint->timer);
    }
}

void apply_hint(Hint* hint, Maze* maze, char* symbol) {
    if (!hint || !maze || !hint->active) return;
    for (int i = 1; i < hint->path.size - 1; i++) {
        CellCoord c = hint->path.cells[i];
        maze->cells[c.row][c.col].symbol = symbol;
    }
}