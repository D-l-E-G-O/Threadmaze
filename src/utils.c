#include "utils.h"


int random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

int clamp_int(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

