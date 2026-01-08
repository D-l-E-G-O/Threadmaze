#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "astar.h"
#include "matrix.h"

static int astar_get_neighbors(const Maze *maze, CellCoord current, CellCoord *neighbors) {
    int count = 0;
    int row = current.row, col = current.col;
    Cell *c = &maze->cells[row][col];

    if (row > 0 && c->up) neighbors[count++] = (CellCoord){row-1, col};
    if (row < maze->height-1 && c->down) neighbors[count++] = (CellCoord){row+1, col};
    if (col > 0 && c->left) neighbors[count++] = (CellCoord){row, col-1};
    if (col < maze->width-1 && c->right) neighbors[count++] = (CellCoord){row, col+1};

    return count;
}

static void astar_reconstruct_path(HeapNode *came_from, int width, CellCoord start, CellCoord end, AStarPath *path) {
    path->size = 0;
    CellCoord current = end;
    while (!(current.row == start.row && current.col == start.col)) {
        if (path->size >= path->capacity) astar_path_resize(path);
        path->cells[path->size++] = current;

        HeapNode prev = came_from[current.row*width + current.col];
        current.row = prev.row;
        current.col = prev.col;
    }

    // add start
    if (path->size >= path->capacity) astar_path_resize(path);
    path->cells[path->size++] = start;

    // invert path to get start->end
    for (int i = 0; i < path->size/2; i++) {
        CellCoord tmp = path->cells[i];
        path->cells[i] = path->cells[path->size-1-i];
        path->cells[path->size-1-i] = tmp;
    }
}

bool astar_solve(const Maze *maze, int start_row, int start_col, int end_row, int end_col, AStarPath *path_out, HeuristicFunc heuristic) {
    if (!maze || !path_out || !heuristic) return false;

    int width = maze->width;
    int height = maze->height;

    bool **visited = bool_matrix_init(width, height);
    HeapNode *came_from = malloc(sizeof(HeapNode) * width * height);
    for (int i = 0; i < width*height; i++) came_from[i].row = came_from[i].col = -1;

    MinHeap heap;
    heap_init(&heap, 64);
    heap_push(&heap, (HeapNode){start_row, start_col, 0, heuristic(start_row, start_col, end_row, end_col)});

    CellCoord neighbors[4];

    while (!heap_empty(&heap)) {
        HeapNode current = heap_pop(&heap);
        if (visited[current.row][current.col]) continue;
        visited[current.row][current.col] = true;

        if (current.row == end_row && current.col == end_col) {
            astar_reconstruct_path(came_from, width, (CellCoord){start_row,start_col}, (CellCoord){end_row,end_col}, path_out);
            astar_cleanup(visited, came_from, heap.data, height);
            return true;
        }

        int n_count = astar_get_neighbors(maze, (CellCoord){current.row,current.col}, neighbors);
        for (int i=0; i<n_count; i++) {
            CellCoord n = neighbors[i];
            if (visited[n.row][n.col]) continue;

            int g = current.g_cost + 1;
            int h = heuristic(n.row, n.col, end_row, end_col);
            HeapNode neighbor = {n.row, n.col, g, g+h};
            heap_push(&heap, neighbor);
            came_from[n.row*width + n.col] = current;
        }
    }

    astar_cleanup(visited, came_from, heap.data, height);
    return false;
}

void astar_path_init(AStarPath *path, int capacity) {
    if (capacity <= 0) capacity = 16;
    path->cells = malloc(sizeof(CellCoord) * capacity);
    path->size = 0;
    path->capacity = capacity;
}

void astar_path_resize(AStarPath *path) {
    if (!path) return;
    path->capacity = path->capacity ? path->capacity * 2 : 16;
    CellCoord *tmp = realloc(path->cells, sizeof(CellCoord) * path->capacity);
    if (!tmp) { 
        perror("realloc"); 
        exit(EXIT_FAILURE); 
    }
    path->cells = tmp;
}

void astar_cleanup(bool **visited, HeapNode *came_from, HeapNode *heap_data, int height) {
    if (visited) {
        for (int i = 0; i < height; i++) free(visited[i]);
        free(visited);
    }
    if (came_from) free(came_from);
    if (heap_data) free(heap_data);
}

int astar_manhattan(int row1, int col1, int row2, int col2) {
    return abs(row1 - row2) + abs(col1 - col2);
}