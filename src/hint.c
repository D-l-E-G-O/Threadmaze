#include "hint.h"

void init_hint(Hint* hint, int max_duration, int maze_width, int maze_height) {
    if (!hint) return;
    init_astar_path(&hint->path, maze_width * maze_height);
    hint->active = false;
    hint->duration = max_duration;
    hint->time_left = 0;
}

void generate_hint_path(Maze* maze, Player* player, AStarPath* path) {
    if (!maze || !player || !path) return;

    int start_row = player->y;
    int start_col = player->x;
    int end_row = maze->height - 1;
    int end_col = maze->width - 1;

    init_astar_path(path, maze->width * maze->height);

    if (!astar_solve(maze, start_row, start_col, end_row, end_col, path, astar_manhattan)) {
        path->size = 0;
        perror("astar_solve");
    }
}

void activate_hint(Hint* hint, Maze* maze, Player* player) {
    if (!hint || !maze || !player) return;

    generate_hint_path(maze, player, &hint->path);
    hint->active = true;
    hint->time_left = hint->duration;

    apply_hint(hint, maze, HINT);
}

void deactivate_hint(Hint* hint, Maze* maze) {
    if (!hint || !maze) return;
    if (!hint->active) return;

    apply_hint(hint, maze, CELL);

    hint->active = false;
    hint->time_left = 0;
    hint->path.size = 0;
}

void hint_tick(Hint* hint, Maze* maze) {
    if (!hint || !hint->active) return;
    hint->time_left--;
    if (hint->time_left <= 0) {
        deactivate_hint(hint, maze);
    }
}

void apply_hint(Hint* hint, Maze* maze, char* symbol) {
    if (!hint || !maze || !hint->active) return;
    for (int i = 1; i < hint->path.size - 1; i++) {
        CellCoord c = hint->path.cells[i];
        maze->cells[c.row][c.col].symbol = symbol;
    }
}