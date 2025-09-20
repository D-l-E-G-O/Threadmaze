#include "maze.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/*
┌─┐	█ ▄ ▀
│ │
└─┘	
*/

void init_maze(Maze* maze, int width, int height) {
    char** grid = malloc(height * sizeof(char*));

    for (int i = 0; i < height; i++) {
        grid[i] = malloc(width * sizeof(char));
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            grid[i][j] = '=';
        }
    }

    maze->grid = grid;
    maze->width = width;
    maze->height = height;
}

void free_maze(Maze* maze) {
    for (int i = 0; i < maze->height; i++) {
        free(maze->grid[i]);
    }
    free(maze->grid);
    maze = NULL;
}

void print_maze(Maze maze) {
    printf("▄");
    for (int j = 0; j < maze.width; j++) {
        printf("▄");
    }
    printf("▄\n");
    
    for (int i = 0; i < maze.height; i++) {
        printf("█");
        for (int j = 0; j < maze.width; j++) {
            printf("%c", maze.grid[i][j]);
        }
        printf("█\n");
    }

    printf("▀");
    for (int j = 0; j < maze.width; j++) {
        printf("▀");
    }
    printf("▀\n");
    
}

bool is_cell_valid(Maze maze, int x, int y) {
    bool x_valid = 0 <= x && x < maze.width;
    bool y_valid = 0 <= y && y < maze.height;
    if (x_valid && y_valid){
        bool empty_cell = maze.grid[y][x] != WALL;
        return empty_cell;
    }
    return false;
}

void set_cell(Maze* maze, int x, int y, char value) {
    maze->grid[y][x] = value;
}

