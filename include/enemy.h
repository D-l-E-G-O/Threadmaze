#pragma once
#include <pthread.h>
#include "maze.h"

typedef struct {
    int x;
    int y;
    pthread_t thread;
} Enemy;

void enemy_init(Enemy *enemy, char **maze);
void *enemy_loop(void *arg);
int enemy_check_collision(int player_x, int player_y, Enemy *enemies, int count);