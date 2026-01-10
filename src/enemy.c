#include "enemy.h"
#include "utils.h"
#include "astar.h"
#include <stdlib.h>
#include <sys/param.h> // For MAX

void enemies_init(Enemy *enemies, int count, const Maze *maze, const Player *player) {
    if (!enemies || !maze || !player) return;

    int player_spawn_distance = MAX(maze->width, maze->height) / 3;

    for (int i = 0; i < count; i++) {
        enemies[i].active = true;
        
        // Find a random spot far enough from player
        int attempts = 0;
        do {
            enemies[i].x = random_int(0, maze->width - 1);
            enemies[i].y = random_int(0, maze->height - 1);
            attempts++;
            // Avoid spawning the enemy on the player or too close to the player
        } while (attempts < 100 && (abs(enemies[i].x - player->x) + abs(enemies[i].y - player->y) < player_spawn_distance));

        enemies[i].origin_x = enemies[i].x;
        enemies[i].origin_y = enemies[i].y;
    }
}

void enemy_move(Enemy *enemy, const Maze *maze, const Player *player, int detection_range, int patrol_range) {
    if (!enemy || !enemy->active || !maze || !player) return;

    // Manhattan distance to the player
    int dist_to_player = abs(enemy->x - player->x) + abs(enemy->y - player->y);

    // --- CASE 1 : PURSUIT MODE (A*) ---
    if (dist_to_player <= detection_range) {
        
        AStarPath path;
        astar_path_init(&path, maze->width * maze->height);

        // A* from the enemy to the player
        if (astar_solve(maze, enemy->y, enemy->x, player->y, player->x, &path, astar_manhattan)) {
            
            // If a path exists and there is at least 2 steps (Start + 1 step)
            if (path.size >= 2) {
                // Analysing the order of the path
                // We check if the first cell of the path correpsonds to the current position of the enemy
                
                int target_x, target_y;

                if (path.cells[0].col == enemy->x && path.cells[0].row == enemy->y) {
                    // The path is in order : [Start, Step 1, Step 2, ..., End]
                    // We need to go to Index 1
                    target_x = path.cells[1].col;
                    target_y = path.cells[1].row;
                } else {
                    // The path is in reverse order : [End, ..., Step 1, Start]
                    // We need to go to the second to last index
                    target_x = path.cells[path.size - 2].col;
                    target_y = path.cells[path.size - 2].row;
                }

                // Movement
                enemy->x = target_x;
                enemy->y = target_y;
            }
        }
        
        // Free pathfinding memory
        free(path.cells);
    } 
    
    // --- CASE 2 : PATROL MODE ---
    else {
        // Try to move randomly
        // 0: Up, 1: Down, 2: Left, 3: Right
        int dir = random_int(0, 3);
        int nx = enemy->x;
        int ny = enemy->y;

        if (dir == 0) ny--;
        if (dir == 1) ny++;
        if (dir == 2) nx--;
        if (dir == 3) nx++;

        // Security check (Walls)
        if (is_cell_valid(maze, enemy->x, enemy->y, nx, ny)) {
            
            // Check if we don't move too far from the patrol zone
            int dist_from_origin = abs(nx - enemy->origin_x) + abs(ny - enemy->origin_y);
            
            // We allow movement only if we are in the patrol radius
            // Or if we are outside the radius but going back to the origin
            int current_dist = abs(enemy->x - enemy->origin_x) + abs(enemy->y - enemy->origin_y);
            
            if (dist_from_origin <= patrol_range || dist_from_origin < current_dist) {
                enemy->x = nx;
                enemy->y = ny;
            }
        }
    }
}