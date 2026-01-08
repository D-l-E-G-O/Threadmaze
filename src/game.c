#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <time.h> // For nanosleep
#include "game.h"
#include "context.h"
#include "render.h"
#include "input.h"
#include "hint.h"

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

    maze_free(&ctx->maze);
    input_restore();
}

/**
 * Handles user input and updates player/hint state accordingly.
 * @param ctx Pointer to the game context.
 * @return true if the game state changed (requiring a render), false otherwise.
 */
static bool process_input(GameContext *ctx) {
    char input = get_input_non_blocking();
    if (input == 0) return false;

    // Check for Quit (Optional hardcoded safety key, e.g., ESC or 'x')
    // if (input == 27) { ctx->is_running = false; return false; }

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

    return state_changed;
}

/**
 * Renders the current frame.
 * @param ctx Pointer to the game context.
 */
static void render_game(const GameContext *ctx) {
    int time_display = -1;
    if (ctx->config->time_limit > 0) {
        time_display = timer_get_remaining((Timer*)&ctx->main_timer); 
        // Cast simply to satisfy const correctness if timer_get_remaining doesn't take const
    }
    // Note: We cast away const for 'maze' because print_game might not expect const currently,
    // though print_maze should ideally take const Maze*.
    print_game((Maze*)&ctx->maze, time_display);
}

// --- Main Entry Point ---

void game_start(GameConfig *config) {
    GameContext ctx;

    // 1. Initialize
    game_context_init(&ctx, config);

    // Start timer if enabled
    if (config->time_limit > 0) {
        timer_start(&ctx.main_timer);
    }

    // Initial render
    render_game(&ctx);

    // Setup nanosleep structure for 10ms
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 10 * 1000000; // 10 milliseconds

    // 2. Game Loop
    while (ctx.is_running) {
        bool needs_render = false;

        if (process_input(&ctx)) needs_render = true;
        if (update_game_state(&ctx)) needs_render = true;

        if (needs_render && ctx.is_running) {
            render_game(&ctx);
        }

        // CPU Saver : Sleep for 10ms
        nanosleep(&ts, NULL);
    }

    // 3. End Game Screen
    // Clear/Stop timers before final print
    timer_stop(&ctx.main_timer);
    
    int final_time = (config->time_limit > 0) ? timer_get_remaining(&ctx.main_timer) : 0;
    
    // Final render to ensure user sees the end state
    render_game(&ctx); 
    print_game_result(ctx.victory, config->time_limit, final_time);

    // 4. Cleanup
    game_context_free(&ctx);
}