#include "player.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>


void init_player(Player* player, Maze* maze) {
    int x = 0;
    int y = 0;

    player->x = x;
    player->y = y;

    maze->grid[y][x] = PLAYER;
}

void move_player(Player* player, char input, Maze* maze) {
    int new_x = player->x;
    int new_y = player->y;

    switch(input) {
        case 'z': new_y--; break;
        case 's': new_y++; break;
        case 'q': new_x--; break;
        case 'd': new_x++; break;
        default: return;
    }

    if (is_cell_valid(*maze, new_x, new_y)) {
        int old_x = player->x;
        int old_y = player->y;
        maze->grid[old_y][old_x] = EMPTY;
        player->x = new_x;
        player->y = new_y;
        maze->grid[new_y][new_x] = PLAYER;
    }
}

bool is_exit_reached(Player player, Maze maze) {
    int x = player.x;
    int y = player.y;

    return maze.grid[y][x] == EXIT;
}