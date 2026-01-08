#pragma once

/**
 * Configures the terminal for game input (non-canonical mode, no echo).
 * Saves the original terminal settings to be restored later.
 */
void input_init(void);

/**
 * Restores the terminal settings to their original state.
 */
void input_restore(void);

/**
 * Reads a character from standard input without blocking.
 * @return The character read, or 0 if no input is available.
 */
char get_input_non_blocking(void);