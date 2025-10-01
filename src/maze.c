#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "maze.h"


void init_maze(Maze* maze, int width, int height) {
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

void free_maze(Maze* maze) {
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

void fill_maze(Maze* maze) {
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

void print_maze(const Maze* maze) {
    int ascii_height = 2 * maze->height + 1;
    int ascii_width  = 2 * maze->width + 1;

    for (int i = 0; i < ascii_height; i++) {
        for (int j = 0; j < ascii_width; j++) {
            // Exterior walls
            if (i == 0 || j == 0 || i == ascii_height - 1 || j == ascii_width - 1) {
                printf(WALL);
                continue;
            }

            // Coordinates in the logical maze
            int cell_col = i / 2;
            int cell_row = j / 2;

            if (i % 2 == 1 && j % 2 == 1) {
                // Cell
                printf("%s", maze->cells[cell_col][cell_row].symbol);
            }
            else if (i % 2 == 1 && j % 2 == 0) {
                // Vertical wall between 2 cells
                Cell left  = maze->cells[cell_col][cell_row - 1];
                Cell right = maze->cells[cell_col][cell_row];
                if (left.right && right.left)
                    printf(SPACE);
                else
                    printf(WALL);
            }
            else if (i % 2 == 0 && j % 2 == 1) {
                // Horizontal wall between 2 cells
                Cell top = maze->cells[cell_col - 1][cell_row];
                Cell bottom = maze->cells[cell_col][cell_row];
                if (top.down && bottom.up)
                    printf(SPACE);
                else
                    printf(WALL);
            }
            else {
                printf(WALL); // Corner
            }
        }
        printf("\n");
    }
}

bool is_cell_valid(Maze maze, int old_x, int old_y, int new_x, int new_y) {
    bool x_valid = 0 <= new_x && new_x < maze.width;
    bool y_valid = 0 <= new_y && new_y < maze.height;
    if (x_valid && y_valid) {
        bool is_not_blocked = false;
        if (old_x < new_x) { // Moved right
            is_not_blocked = maze.cells[old_y][old_x].right;
        }
        else if (old_x > new_x) { // Moved left
            is_not_blocked = maze.cells[old_y][old_x].left;
        }
        if (old_y < new_y) { // Moved down
            is_not_blocked = maze.cells[old_y][old_x].down;
        }
        else if (old_y > new_y) { // Moved up
            is_not_blocked = maze.cells[old_y][old_x].up;
        }
        return is_not_blocked;
    }
    return false;
}

bool move_cell(Maze* maze, int old_x, int old_y, int new_x, int new_y) {
    if (is_cell_valid(*maze, old_x, old_y, new_x, new_y)) {
        maze->cells[new_y][new_x].symbol = maze->cells[old_y][old_x].symbol;
        maze->cells[old_y][old_x].symbol = CELL;
        return true;
    }
    return false;
}

bool **init_bool_matrix(int width, int height) {
    if (width <= 0 || height <= 0) return NULL;
    bool **m = calloc(height, sizeof(*m));
    if (!m) {
        perror("calloc"); 
        exit(EXIT_FAILURE); 
    }

    for (int i = 0; i < height; i++) {
        m[i] = calloc(width, sizeof(**m));
        if (!m[i]) {
            perror("malloc");
            // cleanup on allocation failure
            free_bool_matrix(m, i);
            exit(EXIT_FAILURE);
        }
    }
    return m;
}

void free_bool_matrix(bool **m, int height) {
    if (!m) return;
    for (int i = 0; i < height; i++) {
        free(m[i]);
    }
    free(m);
}

void carve_passage(Maze* maze, int r1, int c1, int r2, int c2) {
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

void init_path(Path* path, int width, int height, int initial_capacity) {
    if (initial_capacity <= 0) initial_capacity = 16;
    path->cells = malloc(sizeof(CellCoord) * initial_capacity);
    if (!path->cells) { 
        perror("malloc"); 
        exit(EXIT_FAILURE);
    }
    path->size = 0;
    path->capacity = initial_capacity;
    path->width = width;
    path->height = height;
    path->in_path = init_bool_matrix(width, height);
}

void free_path(Path* path) {
    if (!path) return;
    free(path->cells);
    free_bool_matrix(path->in_path, path->height);
    path->cells = NULL;
    path->in_path = NULL;
    path->size = 0;
    path->capacity = 0;
}

int push_path(Path* path, int row, int col) {
    // bounds check
    if (row < 0 || row >= path->height || col < 0 || col >= path->width) {
        return -2; // invalid coordinate
    }

    // if already in path, find its index
    if (path->in_path[row][col]) {
        for (int i = 0; i < path->size; i++) {
            if (path->cells[i].row == row && path->cells[i].col == col) {
                return i; // cycle detected at index i
            }
        }
        // Shouldn't happen: in_path true but not found -> treat as error
        return -2;
    }

    // enlarge if necessary
    if (path->size >= path->capacity) {
        int newcap = path->capacity * 2;
        CellCoord *tmp = realloc(path->cells, sizeof(CellCoord) * newcap);
        if (!tmp) { 
            perror("realloc"); 
            exit(EXIT_FAILURE); 
        }
        path->cells = tmp;
        path->capacity = newcap;
    }

    // add
    path->cells[path->size].row = row;
    path->cells[path->size].col = col;
    path->in_path[row][col] = true;
    path->size++;
    return -1; // ok, added, no cycle
}

void truncate_path(Path* path, int index) {
    if (index < 0) {
        path->size = 0;
        // clear all in_path
        for (int i = 0; i < path->height; i++)
            for (int j = 0; j < path->width; j++)
                path->in_path[i][j] = false;
        return;
    }
    if (index >= path->size - 1) return; // nothing to truncate
    for (int i = index + 1; i < path->size; i++) {
        int rr = path->cells[i].row;
        int cc = path->cells[i].col;
        path->in_path[rr][cc] = false;
    }
    path->size = index + 1;
}

bool all_cells_visited(bool **visited, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (!visited[i][j]) return false;
        }
    }
    return true;
}

void generate_maze_wilson(Maze* maze) {
    if (!maze) return;
    fill_maze(maze);

    int width = maze->width;
    int height = maze->height;

    // visited[row][col] : is cell already in the final maze
    bool **visited = init_bool_matrix(width, height); // init_bool_matrix(width,height) returns [height][width]
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
    while (!all_cells_visited(visited, width, height)) {

        // pick a random cell not yet in the tree
        int cell_row, cell_col;
        do {
            cell_row = random_int(0, height - 1);
            cell_col = random_int(0, width - 1);
        } while (visited[cell_row][cell_col]);

        // init path structure (path uses row/col)
        Path path;
        init_path(&path, width, height, (width * height) / 4 + 4);

        // start random walk from (cell_row,cell_col)
        int push_res = push_path(&path, cell_row, cell_col);
        if (push_res < -1) { // invalid (shouldn't happen)
            free_path(&path);
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

            int idx = push_path(&path, next_row, next_col);
            if (idx >= 0) {
                // we hit a cycle: truncate after idx
                truncate_path(&path, idx);
                // set current position to the last element in path
                cell_row = path.cells[path.size - 1].row;
                cell_col = path.cells[path.size - 1].col;
            } else if (idx == -1) {
                // newly added, move current to next
                cell_row = next_row;
                cell_col = next_col;
            } else {
                // idx == -2 means invalid coord: break to avoid infinite loop
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

        free_path(&path);
    }

    free_bool_matrix(visited, height);
}