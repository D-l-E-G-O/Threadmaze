#pragma once
#include "maze.h"

typedef struct {
    int x;
    int y;
} Player;

void init_player(Player *player, char **maze);
void move_player(Player *player, char input, char **maze);
int is_exit_reached(Player *player, char **maze);