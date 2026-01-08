#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <fcntl.h>
#include "input.h"

// Global terminal state
static struct termios oldt;
static int old_flags;

void input_init(void) {
    struct termios newt;

    // Save current terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Disable canonical mode & echo
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Save and set non-blocking mode
    old_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, old_flags | O_NONBLOCK);
}

void input_restore(void) {
    // Restore blocking + terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, old_flags);
}

char get_input_non_blocking(void) {
    char ch = 0;
    if (read(STDIN_FILENO, &ch, 1) <= 0) {
        return 0; // no input
    }
    return ch;
}