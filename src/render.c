#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "render.h"
#include "utils.h"
#include "maze.h"


void print_game(const GameContext *ctx) {
    if (!ctx) return;

    // 1. Clear Screen & Info Header
    printf(CLEAR); // ANSI escape code equivalent to system("clear");

    int time_left = 0;
    if (ctx->config->time_limit > 0) {
        time_left = timer_get_remaining((Timer*)&ctx->main_timer);
        char *bold = time_left % 2 == 0 ? BOLD : REGULAR;
        char *color = time_left < 10 ? RED : BLUE;
        printf("Time left:%s%s %02d:%02d\n" RESET, bold, color, time_left / 60, time_left % 60);
    }

    const Maze *maze = &ctx->maze;

    // ASCII display dimensions (2x + 1 to include walls)
    int ascii_height = 2 * maze->height + 1;
    int ascii_width  = 2 * maze->width + 1;

    // 2. Draw Maze
    for (int i = 0; i < ascii_height; i++) {
        for (int j = 0; j < ascii_width; j++) {
            // --- CASE 1 : Exterior walls ---
            if (i == 0 || j == 0 || i == ascii_height - 1 || j == ascii_width - 1) {
                printf(WALL);
                continue;
            }

            // ASCII coords conversion -> Logical coords (Game)
            // i represents the height (Y), j represents the width (X)
            int y = i / 2;
            int x = j / 2;

            // --- CASE 2 : Center of a cell (Game content) ---
            if (i % 2 == 1 && j % 2 == 1) {
                // Layer 1: Player
                if (x == ctx->player.x && y == ctx->player.y) {
                    printf(PLAYER);
                    continue;
                }

                // Layer 2: Enemies
                bool enemy_here = false;
                if (ctx->enemies) {
                    for (int k = 0; k < ctx->enemy_count; k++) {
                        if (ctx->enemies[k].active && ctx->enemies[k].x == x && ctx->enemies[k].y == y) {
                            printf(ENEMY);
                            enemy_here = true;
                            break;
                        }
                    }
                }
                if (enemy_here) continue;

                // Layer 3: Exit
                if (x == maze->width - 1 && y == maze->height - 1) {
                    printf(EXIT);
                    continue;
                }

                // Layer 4: Content (Hint, empty, etc.)
                printf("%s", maze->cells[y][x].symbol);
            }
            // --- CASE 3 : Vertical Wall ---
            else if (i % 2 == 1 && j % 2 == 0) {
                Cell left = maze->cells[y][x - 1];
                Cell right = maze->cells[y][x];
                if (left.right && right.left)
                    printf(SPACE);
                else
                    printf(WALL);
            }
            // --- CASE 4 : Horizontal WALL ---
            else if (i % 2 == 0 && j % 2 == 1) {
                Cell top = maze->cells[y - 1][x];
                Cell bottom = maze->cells[y][x];
                if (top.down && bottom.up)
                    printf(SPACE);
                else
                    printf(WALL);
            }
            // --- CASE 5 : Corners ---
            else {
                printf(WALL);
            }
        }
        printf("\n");
    }
}

void print_game_result(bool victory, int initial_time, int time_left) {
    if (victory) {
        printf(BOLD GREEN "=== VICTORY! ===\n");
        if (initial_time > 0)
            printf("You won in %d seconds!\n", initial_time - time_left);
        else
            printf("You won!\n");
    } else {
        printf(BOLD RED "=== GAME OVER ===\n");
        if (initial_time > 0 && time_left <= 0) {
            printf("Time run out!\n");
        } else {
            printf("You were caught!\n");
        }
    }
    printf(RESET);
}