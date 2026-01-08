#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define RESET   "\033[0m"

// Regular Colors
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define GRAY    "\033[90m"

// Bold
#define BOLD        "\033[1m"
#define BOLDRED     "\033[1m\033[31m"
#define BOLDGREEN   "\033[1m\033[32m"
#define BOLDYELLOW  "\033[1m\033[33m"
#define BOLDBLUE    "\033[1m\033[34m"

// Global flag to signal an interruption request (CTRL+C)
// "volatile sig_atomic_t" is the only type safe to read/write in a signal handler.
extern volatile sig_atomic_t stop_requested;

/**
 * Sets up the SIGINT (CTRL+C) signal handler globally.
 */
void setup_signal_handler(void);

/**
 * Generates a random integer between min and max (inclusive).
 * @param min The lower bound.
 * @param max The upper bound.
 * @return A random integer between min and max.
 */
int random_int(int min, int max);

/**
 * Clamps a value between a minimum and a maximum.
 * @param value The value to clamp.
 * @param min The lower bound.
 * @param max The upper bound.
 * @return The clamped value.
 */
int clamp_int(int value, int min, int max);

/**
 * Gets the current width of the terminal window.
 * @return The width in characters, or a default value if retrieval fails.
 */
int get_terminal_width(void);

/**
 * Gets the current height of the terminal window.
 * @return The height in characters, or a default value if retrieval fails.
 */
int get_terminal_height(void);

/**
 * Reads a single character from stdin.
 * @return The character read.
 */
char read_char();

/**
 * Reads a line from stdin and converts it to an integer.
 * @return The integer entered by the user, or 0 if invalid.
 */
int read_int(void);