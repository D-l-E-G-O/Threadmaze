#include "utils.h"
#include <sys/ioctl.h>


int random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

int clamp_int(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

int get_terminal_width(void) {
    struct winsize w;
    return (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) ? 80 : w.ws_col;
}

int get_terminal_height(void) {
    struct winsize w;
    return (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) ? 24 : w.ws_row;
}

