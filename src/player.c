#include "player.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>


void player_init(Player *player, Maze *maze) {
    if (!player || !maze) return;
    player->x = 0;
    player->y = 0;
}

void player_move(Player *player, Direction direction, Maze *maze) {
    if (!player || !maze) return;

    int new_x = player->x;
    int new_y = player->y;

    switch (direction) {
        case DIR_UP:    new_y--; break;
        case DIR_DOWN:  new_y++; break;
        case DIR_LEFT:  new_x--; break;
        case DIR_RIGHT: new_x++; break;
        default: return;
    }

    // We use the previous coordinates to determine the direction logic inside move_cell
    if (move_cell(maze, player->x, player->y, new_x, new_y)) {
        player->x = new_x;
        player->y = new_y;
    }
}

bool is_exit_reached(const Player *player, const Maze *maze) {
    if (!player || !maze) return false;
    return player->x == maze->width-1 && player->y == maze->height-1;
}