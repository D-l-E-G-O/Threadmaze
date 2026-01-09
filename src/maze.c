#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "maze.h"
#include "matrix.h"
#include "astar.h"

void maze_init(Maze *maze, int width, int height) {
    Cell **cells = malloc(sizeof(Cell*) * height);
    if (!cells) { 
        perror("malloc"); 
        exit(EXIT_FAILURE); 
    }
    for (int i = 0; i < height; i++) {
        cells[i] = malloc(sizeof(Cell) * width);
        if (!cells[i]) { 
            perror("malloc"); 
            exit(EXIT_FAILURE); 
        }
    }
    maze->cells = cells;
    maze->width = width;
    maze->height = height;
}

void maze_free(Maze *maze) {
    if (!maze || !maze->cells) return;
    for (int i = 0; i < maze->height; i++) {
        if (maze->cells[i] != NULL) {
            free(maze->cells[i]);
            maze->cells[i] = NULL;
        }
    }
    free(maze->cells);
    maze->cells = NULL;
    maze->width = 0;
    maze->height = 0;
}

void maze_fill(Maze *maze) {
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            maze->cells[i][j].up = false;
            maze->cells[i][j].down = false;
            maze->cells[i][j].left = false;
            maze->cells[i][j].right = false;
            maze->cells[i][j].symbol = CELL;
        }
    }
}

bool is_cell_valid(const Maze *maze, int old_x, int old_y, int new_x, int new_y) {
    bool x_valid = 0 <= new_x && new_x < maze->width;
    bool y_valid = 0 <= new_y && new_y < maze->height;
    if (x_valid && y_valid) {
        bool is_not_blocked = false;
        if (old_x < new_x) { // Moved right
            is_not_blocked = maze->cells[old_y][old_x].right;
        }
        else if (old_x > new_x) { // Moved left
            is_not_blocked = maze->cells[old_y][old_x].left;
        }
        if (old_y < new_y) { // Moved down
            is_not_blocked = maze->cells[old_y][old_x].down;
        }
        else if (old_y > new_y) { // Moved up
            is_not_blocked = maze->cells[old_y][old_x].up;
        }
        return is_not_blocked;
    }
    return false;
}

bool move_cell(Maze *maze, int old_x, int old_y, int new_x, int new_y) {
    if (is_cell_valid(maze, old_x, old_y, new_x, new_y)) {
        maze->cells[new_y][new_x].symbol = maze->cells[old_y][old_x].symbol;
        maze->cells[old_y][old_x].symbol = CELL;
        return true;
    }
    return false;
}

void carve_passage(Maze *maze, int r1, int c1, int r2, int c2) {
    // bounds check
    if (r1 < 0 || r1 >= maze->height || r2 < 0 || r2 >= maze->height ||
        c1 < 0 || c1 >= maze->width  || c2 < 0 || c2 >= maze->width) {
        return;
    }

    if (r2 == r1 - 1 && c2 == c1) { // up (r2 above r1)
        maze->cells[r1][c1].up = true;
        maze->cells[r2][c2].down = true;
    } else if (r2 == r1 + 1 && c2 == c1) { // down (r2 below r1)
        maze->cells[r1][c1].down = true;
        maze->cells[r2][c2].up = true;
    } else if (r2 == r1 && c2 == c1 - 1) { // left (c2 left to c1)
        maze->cells[r1][c1].left = true;
        maze->cells[r2][c2].right = true;
    } else if (r2 == r1 && c2 == c1 + 1) { // right (c2 right to c1)
        maze->cells[r1][c1].right = true;
        maze->cells[r2][c2].left = true;
    } else {
        // not adjacent -> ignore
    }
}

void generate_maze_wilson(Maze *maze) {
    if (!maze) return;
    maze_fill(maze);

    int width = maze->width;
    int height = maze->height;

    // visited[row][col] : is cell already in the final maze
    bool **visited = bool_matrix_init(width, height); // init_bool_matrix(width,height) returns [height][width]
    if (!visited) { 
        perror("init_bool_matrix"); 
        exit(EXIT_FAILURE); 
    }

    // seed RNG once
    srand((unsigned)time(NULL));

    // pick initial cell to be in the tree (e.g., (0,0))
    int start_row = random_int(0, height - 1);
    int start_col = random_int(0, width - 1);
    visited[start_row][start_col] = true;

    // direction offsets (row change, col change)
    const int dir_row[4] = {-1, 1, 0, 0}; // up, down, left, right
    const int dir_col[4] = {0, 0, -1, 1};

    // main loop until all visited
    while (!all_cells_visited((const bool**)visited, width, height)) {

        // pick a random cell not yet in the tree
        int cell_row, cell_col;
        do {
            cell_row = random_int(0, height - 1);
            cell_col = random_int(0, width - 1);
        } while (visited[cell_row][cell_col]);

        // init path structure (path uses row/col)
        Path path;
        path_init(&path, width, height, (width * height) / 4 + 4);

        // start random walk from (cell_row,cell_col)
        int push_res = path_push(&path, cell_row, cell_col);
        if (push_res < -1) { // invalid (shouldn't happen)
            path_free(&path);
            break;
        }

        // walk until we hit a visited cell
        while (!visited[cell_row][cell_col]) {
            int d = random_int(0, 3);
            int next_row = cell_row + dir_row[d];
            int next_col = cell_col + dir_col[d];

            // clamp to bounds (prevent escape)
            next_row = clamp_int(next_row, 0, height - 1);
            next_col = clamp_int(next_col, 0, width - 1);

            int index = path_push(&path, next_row, next_col);
            switch (index) {
                case ERROR:
                    perror("push_path");
                    return;
                case NO_CYCLE:
                    // newly added, move current to next
                    cell_row = next_row;
                    cell_col = next_col;
                    break;
                default:
                    // we hit a cycle: truncate after idx
                    path_truncate(&path, index);
                    // set current position to the last element in path
                    cell_row = path.cells[path.size - 1].row;
                    cell_col = path.cells[path.size - 1].col;
                    break;
            }
        }

        // Now path leads to a visited cell; carve passages for consecutive pairs
        for (int i = 0; i + 1 < path.size; i++) {
            int r1 = path.cells[i].row;
            int c1 = path.cells[i].col;
            int r2 = path.cells[i+1].row;
            int c2 = path.cells[i+1].col;
            carve_passage(maze, r1, c1, r2, c2);
            visited[r1][c1] = true;
        }
        // Also ensure last cell in path is marked visited (the one that reached existing tree)
        if (path.size > 0) {
            int lr = path.cells[path.size - 1].row;
            int lc = path.cells[path.size - 1].col;
            visited[lr][lc] = true;
        }

        path_free(&path);
    }

    bool_matrix_free(visited, height);
}

void maze_mutate(Maze *maze) {
    if (!maze) return;

    // Try a few times to find a valid mutation candidate
    // (A wall that separates two connected cells)
    for (int attempts = 0; attempts < 20; attempts++) {
        int r = random_int(0, maze->height - 1);
        int c = random_int(0, maze->width - 1);
        
        // Pick a random direction (0: up, 1: down, 2: left, 3: right)
        int dir = random_int(0, 3);
        int nr = r, nc = c;
        bool has_wall = false;

        // Check bounds and wall existence
        if (dir == 0) { nr--; has_wall = !maze->cells[r][c].up; }
        else if (dir == 1) { nr++; has_wall = !maze->cells[r][c].down; }
        else if (dir == 2) { nc--; has_wall = !maze->cells[r][c].left; }
        else if (dir == 3) { nc++; has_wall = !maze->cells[r][c].right; }

        // If out of bounds or NO wall there (already open), skip
        if (nr < 0 || nr >= maze->height || nc < 0 || nc >= maze->width || !has_wall) {
            continue;
        }

        // 1. Find the existing path between (r,c) and (nr,nc) BEFORE removing the wall
        AStarPath path;
        astar_path_init(&path, maze->width * maze->height);
        
        // Use Manhattan heuristic
        if (astar_solve(maze, r, c, nr, nc, &path, astar_manhattan)) {
            // Path found! It has at least 2 cells (start and end).
            // We need to pick a random "edge" in this path to build a NEW wall.
            // An edge is between path.cells[i] and path.cells[i+1].
            
            if (path.size >= 2) {
                // Pick a random connection to block along the path
                int cut_idx = random_int(0, path.size - 2);
                int br = path.cells[cut_idx].row;
                int bc = path.cells[cut_idx].col;
                int b_next_r = path.cells[cut_idx + 1].row;
                int b_next_c = path.cells[cut_idx + 1].col;

                // 2. Build the new wall
                // Note: We "uncarve" passage. Logic is inverse of carve_passage.
                if (b_next_r == br - 1) { // Up
                    maze->cells[br][bc].up = false;
                    maze->cells[b_next_r][b_next_c].down = false;
                } else if (b_next_r == br + 1) { // Down
                    maze->cells[br][bc].down = false;
                    maze->cells[b_next_r][b_next_c].up = false;
                } else if (b_next_c == bc - 1) { // Left
                    maze->cells[br][bc].left = false;
                    maze->cells[b_next_r][b_next_c].right = false;
                } else if (b_next_c == bc + 1) { // Right
                    maze->cells[br][bc].right = false;
                    maze->cells[b_next_r][b_next_c].left = false;
                }

                // 3. Remove the chosen wall (r,c) <-> (nr,nc)
                carve_passage(maze, r, c, nr, nc);
                
                // Cleanup and exit success
                free(path.cells);
                return;
            }
        }
        
        free(path.cells);
    }
}

void maze_braid(Maze *maze, int probability) {
    if (!maze || probability <= 0) return;

    for (int y = 1; y < maze->height - 1; y++) {
        for (int x = 1; x < maze->width - 1; x++) {
            // Randomly remove RIGHT wall
            if (!maze->cells[y][x].right && x < maze->width - 1 && random_int(0, 100) < probability) {
                carve_passage(maze, y, x, y, x + 1);
            }
            // Randomly remove DOWN wall
            if (!maze->cells[y][x].down && y < maze->height - 1 && random_int(0, 100) < probability) {
                carve_passage(maze, y, x, y + 1, x);
            }
        }
    }
}