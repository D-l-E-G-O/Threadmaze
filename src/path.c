#include <stdlib.h>
#include <stdio.h>
#include "path.h"
#include "matrix.h"

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
    path->visited_cells = init_bool_matrix(width, height);
}

void free_path(Path* path) {
    if (!path) return;
    if (path->cells) free(path->cells);
    if (path->visited_cells) free_bool_matrix(path->visited_cells, path->height);
    path->cells = NULL;
    path->visited_cells = NULL;
    path->size = 0;
    path->capacity = 0;
}

void clear_path(Path* path) {
    if (!path) return;
    for (int i = 0; i < path->height; i++)
        for (int j = 0; j < path->width; j++)
            path->visited_cells[i][j] = false;
    path->size = 0;
}

int push_path(Path* path, int row, int col) {
    if (!path) return ERROR;
    // bounds check
    if (row < 0 || row >= path->height || col < 0 || col >= path->width) return ERROR;

    // if already in path, find its index
    if (path->visited_cells[row][col]) {
        for (int i = 0; i < path->size; i++) {
            if (path->cells[i].row == row && path->cells[i].col == col) {
                return i; // cycle detected at index i
            }
        }
        // Shouldn't happen: visited_cells true but not found -> treat as error
        return ERROR;
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

    // add new cell
    path->cells[path->size].row = row;
    path->cells[path->size].col = col;
    path->visited_cells[row][col] = true;
    path->size++;
    return NO_CYCLE;
}

void truncate_path(Path* path, int index) {
    if (!path) return;
    if (index < 0) {
        clear_path(path);
        return;
    }
    if (index >= path->size - 1) return; // nothing to truncate
    for (int i = index + 1; i < path->size; i++) {
        int row = path->cells[i].row;
        int col = path->cells[i].col;
        path->visited_cells[row][col] = false;
    }
    path->size = index + 1;
}

void set_path_cells(Path* path, CellCoord* new_cells, int new_size) {
    if (!path || !new_cells || new_size <= 0) return;
    clear_path(path);
    for (int i = 0; i < new_size; i++) {
        push_path(path, new_cells[i].row, new_cells[i].col);
    }
}

bool all_cells_visited(bool **visited, int width, int height) {
    if (!visited) return false;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (!visited[i][j]) return false;
        }
    }
    return true;
}