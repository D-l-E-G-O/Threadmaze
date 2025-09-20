#pragma once
#include <pthread.h>
#include "maze.h"

typedef struct {
    int x;
    int y;
    pthread_t thread;
} Enemy;

void init_enemy(Enemy* enemy, char** maze);
void* enemy_loop(void* arg);
int check_collision(int player_x, int player_y, Enemy* enemies, int count);