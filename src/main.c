#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "maze.h"
#include "player.h"
#include "enemy.h"
#include "timer.h"
#include "utils.h"


int main(int argc, char* argv[]) {
    int width = 10;
    int height = 5;
    Maze* maze;
    Player* player;
    init_maze(maze, width, height);
    init_player(player, maze);
    print_maze(*maze);
    printf("x:%d, y:%d\n", player->x, player->y);
    while (!is_exit_reached(*player, *maze)) {
        game_loop(maze, player);
    }
    free_maze(maze);
}

void game_loop(Maze* maze, Player* player) {
    char input = get_input_non_blocking();
    if (!input) {
        return;
    }
    system("clear");
    move_player(player, input, maze);
    print_maze(*maze);
    printf("x:%d, y:%d\n", player->x, player->y);
}

void end_game(bool victory) {

}