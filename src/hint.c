#include "hint.h"
#include <string.h>

void hint_init(Hint *hint, Timer *timer, int duration) {
    if (!hint || !timer) return;
    
    hint->active = false;
    hint->duration = duration;
    hint->timer = timer;
    
    // We initialize the path structure with 0 capacity for now; 
    // it will be allocated when needed.
    hint->path.cells = NULL;
    hint->path.size = 0;
    hint->path.capacity = 0;

    timer_init(hint->timer, duration);
}

void hint_free(Hint *hint) {
    if (!hint) return;
    
    if (hint->path.cells) {
        free(hint->path.cells);
        hint->path.cells = NULL;
    }
    hint->path.size = 0;
    hint->path.capacity = 0;
}

void hint_generate_path(Maze *maze, Player *player, AStarPath *path) {
    if (!maze || !player || !path) return;

    int start_row = player->y;
    int start_col = player->x;
    int end_row = maze->height - 1;
    int end_col = maze->width - 1;

    // Reset path if needed (or reallocate if NULL)
    if (path->cells == NULL) {
        astar_path_init(path, maze->width * maze->height);
    } else {
        path->size = 0; 
    }

    if (!astar_solve(maze, start_row, start_col, end_row, end_col, path, astar_manhattan)) {
        // Fallback if no path found (should not happen in a perfect maze)
        path->size = 0;
    }
}

void hint_activate(Hint *hint, Maze *maze, Player *player) {
    if (!hint || !maze || !player) return;

    // 1. Clean up old visuals if it was already active
    if (hint->active) {
        hint_apply_visuals(hint, maze, CELL); 
    }

    // 2. Generate new path from current position
    hint_generate_path(maze, player, &hint->path);
    hint->active = true;

    // 3. Show path
    hint_apply_visuals(hint, maze, HINT);

    // 4. Start (or Restart) Timer
    timer_start(hint->timer);
}

void hint_deactivate(Hint *hint, Maze *maze) {
    if (!hint || !maze) return;
    if (!hint->active) return;

    // 1. Restore maze visuals
    hint_apply_visuals(hint, maze, CELL);

    // 2. Reset state
    hint->active = false;
    
    // 3. Stop timer
    timer_stop(hint->timer);
}

void hint_apply_visuals(Hint *hint, Maze *maze, char *symbol) {
    if (!hint || !maze || !hint->path.cells) return;
    
    // We iterate through all the path cells
    for (int i = 0; i < hint->path.size; i++) {
        CellCoord c = hint->path.cells[i];
        
        // Safety check bounds
        if (c.row >= 0 && c.row < maze->height && c.col >= 0 && c.col < maze->width) {
            char* current_symbol = maze->cells[c.row][c.col].symbol;
            
            // We skip if the player and exit symbols
            if (strcmp(current_symbol, PLAYER) == 0 || strcmp(current_symbol, EXIT) == 0) {
                continue; 
            }

            // We apply the symbol
            maze->cells[c.row][c.col].symbol = symbol;
        }
    }
}

void hint_update(Hint *hint, Maze *maze, Player *player) {
    if (!hint || !maze || !player || !hint->active) return;

    // 1. Erase the old visual path
    hint_apply_visuals(hint, maze, CELL);

    // 2. Recalculate path from the new player position
    hint_generate_path(maze, player, &hint->path);

    // 3. Draw the new visual path
    hint_apply_visuals(hint, maze, HINT);
}