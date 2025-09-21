#include "player.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>


void init_player(Player* player, Maze* maze) {
    player->x = 0;
    player->y = 0;

    maze->cells[0][0].symbol = PLAYER;
    maze->cells[maze->height-1][maze->width-1].symbol = EXIT;
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

    int old_x = player->x;
    int old_y = player->y;

    if (move_cell(maze, old_x, old_y, new_x, new_y)) {
        player->x = new_x;
        player->y = new_y;
    }
}

bool is_exit_reached(Player player, Maze maze) {
    return player.x == maze.width-1 && player.y == maze.height-1;
}