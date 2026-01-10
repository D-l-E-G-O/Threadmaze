#pragma once
#include "maze.h"
#include "player.h"
#include "config.h"
#include "timer.h"
#include "hint.h"
#include "enemy.h"

/**
 * GameContext holds the entire state of the game session.
 * This centralization avoids global variables and simplifies function signatures.
 */
typedef struct {
    // Game Entities
    Maze maze;
    Player player;
    Hint hint;
    Enemy *enemies;
    int enemy_count;
    
    // Timers
    Timer main_timer;       // Maze timer
    Timer hint_timer;       // Dedicated timer storage for the hint system
    Timer mutation_timer;   // Dedicated timer storage for the mutation system
    
    // Configuration & State
    GameConfig *config; // Pointer to the active configuration (read-only mostly)
    bool is_running;
    bool victory;
} GameContext;