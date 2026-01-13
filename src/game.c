#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <time.h> // For nanosleep, time
#include <ctype.h>  // for tolower()
#include "game.h"
#include "context.h"
#include "render.h"
#include "input.h"
#include "hint.h"
#include "menu.h"

// --- Internal Helper Functions ---

/**
 * Converts a raw character input into a game Direction based on configuration.
 * @param input The character read from stdin.
 * @param config Pointer to the game configuration.
 * @return The corresponding Direction or DIR_NONE.
 */
static Direction get_direction_from_input(char input, const GameConfig *config) {
    if (input == config->key_up)    return DIR_UP;
    if (input == config->key_down)  return DIR_DOWN;
    if (input == config->key_left)  return DIR_LEFT;
    if (input == config->key_right) return DIR_RIGHT;
    return DIR_NONE;
}

/**
 * Initializes the game context, allocating resources and setting up entities.
 * @param ctx Pointer to the context to initialize.
 * @param config Pointer to the configuration to use.
 */
static void game_context_init(GameContext *ctx, GameConfig *config) {
    if (!ctx || !config) return;

    ctx->config = config;
    ctx->is_running = true;
    ctx->victory = false;

    // Initialize entities
    maze_init(&ctx->maze, config->width, config->height);
    generate_maze_wilson(&ctx->maze);
    player_init(&ctx->player, &ctx->maze);

    // Make maze imperfect (braiding)
    if (config->braid_probability > 0) {
        maze_braid(&ctx->maze, config->braid_probability);
    }

    // Init Enemies
    ctx->enemy_count = config->enemy_count;
    ctx->enemies = NULL;
    if (ctx->enemy_count > 0) {
        ctx->enemies = calloc(ctx->enemy_count, sizeof(Enemy));
        enemies_init(ctx->enemies, ctx->enemy_count, &ctx->maze, &ctx->player);
    }

    // Initialize Timers
    hint_init(&ctx->hint, &ctx->hint_timer, config->hint_duration);
    timer_init(&ctx->main_timer, config->time_limit);
    timer_init(&ctx->main_timer, config->time_limit);
    timer_init(&ctx->mutation_timer, config->mutation_interval);
    if (config->mutation_interval > 0) {
        timer_start(&ctx->mutation_timer);
    }

    // Setup input mode
    input_init();
}

/**
 * Cleans up all resources used by the game context.
 * @param ctx Pointer to the context.
 */
static void game_context_free(GameContext *ctx) {
    if (!ctx) return;

    // Stop timers first to ensure no thread tries to access freed memory
    timer_stop(&ctx->main_timer);
    hint_deactivate(&ctx->hint, &ctx->maze);
    hint_free(&ctx->hint);
    timer_stop(&ctx->mutation_timer);

    if (ctx->enemies) free(ctx->enemies);

    maze_free(&ctx->maze);
    input_restore();
}

/**
 * Handles user input and updates player/hint state accordingly.
 * @param ctx Pointer to the game context.
 * @return true if the game state changed (requiring a render), false otherwise.
 */
static bool process_input(GameContext *ctx) {
    // Check for Signal Interruption first
    if (stop_requested) {
        ctx->is_running = false;
        return false;
    }

    char input = tolower(get_input_non_blocking());
    if (input == 0) return false;

    bool state_changed = false;

    // Handle Hint Toggle
    if (input == ctx->config->key_hint) {
        if (ctx->hint.active) {
            hint_deactivate(&ctx->hint, &ctx->maze);
        } else {
            hint_activate(&ctx->hint, &ctx->maze, &ctx->player);
        }
        state_changed = true;
    }
    // Handle Movement
    else {
        Direction dir = get_direction_from_input(input, ctx->config);
        if (dir != DIR_NONE) {
            // 1. Move the player first
            player_move(&ctx->player, dir, &ctx->maze);

            // 2. If hint is active, just update the path (don't restart timer)
            if (ctx->hint.active) {
                hint_update(&ctx->hint, &ctx->maze, &ctx->player);
            }

            state_changed = true;
        }
    }
    return state_changed;
}

/**
 * Updates game logic (timers, victory conditions, loss conditions).
 * @param ctx Pointer to the game context.
 * @return true if the game state changed, false otherwise.
 */
static bool update_game_state(GameContext *ctx) {
    bool state_changed = false;

    // 1. Check Victory
    if (is_exit_reached(&ctx->player, &ctx->maze)) {
        ctx->victory = true;
        ctx->is_running = false;
        return true;
    }

    // 2. Check Main Timer
    if (ctx->config->time_limit > 0) {
        // Check for timeout
        if (timer_get_remaining(&ctx->main_timer) <= 0) {
            ctx->victory = false;
            ctx->is_running = false;
            return true;
        }
        // Check for tick event (to refresh UI every second)
        if (timer_consume_tick(&ctx->main_timer)) {
            state_changed = true;
        }
    }

    // 3. Check Hint Timer
    if (ctx->hint.active) {
        if (timer_get_remaining(&ctx->hint_timer) <= 0) {
            hint_deactivate(&ctx->hint, &ctx->maze);
            state_changed = true;
        }
    }

    // 4. Check Mutation Timer
    if (ctx->config->mutation_interval > 0) {
        // If timer reached 0
        if (timer_get_remaining(&ctx->mutation_timer) <= 0) {
            
            // Perform mutation N times
            for (int i = 0; i < ctx->config->mutation_amount; i++) {
                maze_mutate(&ctx->maze);
            }
            
            // If Hint is active, it's now invalid because the maze changed
            // We must update the path.
            if (ctx->hint.active) {
                hint_update(&ctx->hint, &ctx->maze, &ctx->player);
            }
            
            // Restart mutation timer
            timer_start(&ctx->mutation_timer);
            state_changed = true;
        }
    }

    // 5. Enemy Logic
    static int enemy_tick_accumulator = 0;
    enemy_tick_accumulator += 10; // +10ms

    if (ctx->enemy_count > 0 && enemy_tick_accumulator >= ctx->config->enemy_speed_ms) {
        enemy_tick_accumulator = 0;
        
        // Move all enemies
        for (int i = 0; i < ctx->enemy_count; i++) {
            enemy_move(&ctx->enemies[i], &ctx->maze, &ctx->player, ctx->config->enemy_detection_range, ctx->config->enemy_patrol_range);
            
            // Collision Check
            if (ctx->enemies[i].x == ctx->player.x && ctx->enemies[i].y == ctx->player.y) {
                ctx->is_running = false;
                ctx->victory = false;
                return true;
            }
        }
        state_changed = true;
    }

    return state_changed;
}

// --- Main Entry Point ---

void game_start(GameConfig *config) {
    // Generate a random seed for this session
    unsigned int game_seed = (unsigned int)time(NULL);
    
    bool stay_in_game_loop = true;

    while (stay_in_game_loop && !stop_requested) {
        // Apply the seed before generating anything.
        // If we replay, we use the saùe seed. If we came from main menu, it's a new seed.
        srand(game_seed);

        GameContext ctx;

        // 1. Initialize
        game_context_init(&ctx, config);

        // Start timer if enabled
        if (config->time_limit > 0) {
            timer_start(&ctx.main_timer);
        }

        // Initial render
        print_game(&ctx);

        // Setup nanosleep structure for 10ms
        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 10 * 1000000; // 10 milliseconds

        // 2. Game Loop
        while (ctx.is_running) {
            // IMMEDIATE CHECK: If CTRL+C was pressed during the sleep or render
            if (stop_requested) {
                ctx.is_running = false;
                break; // Exit loop immediately to proceed to cleanup
            }

            bool needs_render = false;

            if (process_input(&ctx)) needs_render = true;
            if (update_game_state(&ctx)) needs_render = true;

            if (needs_render && ctx.is_running) {
                print_game(&ctx);
            }

            // CPU Saver : Sleep for 10ms
            // If nanosleep is interrupted by a signal, it returns -1 and sets errno to EINTR.
            // We don't strictly need to handle it because the loop will check stop_requested right after.
            nanosleep(&ts, NULL);
        }

        // 3. End Game Screen
        // Clear/Stop timers before final print
        timer_stop(&ctx.main_timer);
    
        // Only print result if it wasn't a forced quit
        if (stop_requested) {
            // Forced quit
            printf("\n" BOLD YELLOW "Game interrupted by user. Exiting..." RESET "\n");
            game_context_free(&ctx);
            break;
        }

        int final_time = (config->time_limit > 0) ? timer_get_remaining(&ctx.main_timer) : 0;
        // Final render to ensure user sees the end state
        print_game(&ctx); 
        print_game_result(ctx.victory, config->time_limit, final_time);

        // 4. Cleanup
        game_context_free(&ctx);

        // 5. Show End Menu
        int choice = show_end_game_menu();

        switch (choice) {
            case 1: // Replay
                // We do not change 'game_seed'. 
                // The loop continues, srand(game_seed) will be called again, 
                // generating the exact same maze.
                break; 
            
            case 2: // Main Menu
                stay_in_game_loop = false;
                // We break the loop, function returns to main.c
                break;

            case 3: // Quit
                stay_in_game_loop = false;
                stop_requested = 1; // Signal global stop to main.c
                break;
        }
    }
}